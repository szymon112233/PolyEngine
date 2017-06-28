#include "MovementSystem.hpp"

#include <Vector.hpp>
#include <Quaternion.hpp>
#include <UniqueID.hpp>
#include <World.hpp>
#include <TransformComponent.hpp>
#include <TimeSystem.hpp>

#include "MovementComponent.hpp"
#include "PathMovementComponent.hpp"

void Invaders::MovementSystem::MovementUpdatePhase(Poly::World* world)
{
	float dt = Poly::TimeSystem::GetTimerDeltaTime(world, Poly::eEngineTimer::GAMEPLAY);
	MovementComponent* movement;
	Poly::TransformComponent* transform;

	
	for (auto tuple : world->IterateComponents<PathMovementComponent, MovementComponent, Poly::TransformComponent>())
	{
		movement = std::get<MovementComponent*>(tuple);
		PathMovementComponent* path = std::get<PathMovementComponent*>(tuple);
		transform = std::get<Poly::TransformComponent*>(tuple);

		double threshold2 = 1;
		double velocity = path->GetVelocity();

		if (!path->GetHasStarted())
		{
			path->CurrentPoint = path->PointsQueue.Front();
			path->PointsQueue.PopFront();
			if (path->GetIsPathRelative())
			{
				path->CurrentPoint += transform->GetLocalTranslation();
			}
			path->SetHasStarted(true);
		}

		if ((pow(transform->GetLocalTranslation().X - path->CurrentPoint.X, 2)
			+ pow(transform->GetLocalTranslation().Z - path->CurrentPoint.Z, 2)
			+ pow(transform->GetLocalTranslation().Y - path->CurrentPoint.Y, 2)) < threshold2)
		{
			if (path->PointsQueue.GetSize() != 0)
			{
				path->CurrentPoint = path->PointsQueue.Front();
				path->PointsQueue.PopFront();
				if (path->GetIsPathRelative())
				{
					path->CurrentPoint += transform->GetLocalTranslation();
				}
				Poly::Vector direction = path->CurrentPoint - transform->GetLocalTranslation();
				movement->LinearVelocity = direction.Normalize();
			}
			else
			{
				if (path->GetIsRepeat())
				{
					path->PointsQueue = path->BackupPointsQueue;
				}
				else
				{
					movement->LinearVelocity = { 0.0f,0.0f,0.0f };
					movement->LinearAcceleration = { 0.0f,0.0f,0.0f };
				}
				
			}
			
		}
		else
		{
			Poly::Vector direction = path->CurrentPoint - transform->GetLocalTranslation();
			movement->LinearVelocity = direction.Normalize() * movement->LinearVelocity.Length();
		}


		if (movement->LinearVelocity.Length2() != 0)
		{
			if (movement->LinearVelocity.Length2() > velocity  * velocity)
			{
				movement->LinearVelocity.Normalize();
				movement->LinearVelocity *= path->GetVelocity();
			}
			else if (movement->LinearVelocity.Length2() < velocity  * velocity)
			{
				movement->LinearAcceleration = movement->LinearVelocity;
				movement->LinearAcceleration.Normalize();
				movement->LinearAcceleration *= path->GetAcceleration();
			}
		}
	}
	for (auto tuple : world->IterateComponents<MovementComponent, Poly::TransformComponent>())
	{
		movement = std::get<MovementComponent*>(tuple);
		transform = std::get<Poly::TransformComponent*>(tuple);

		movement->LinearVelocity += movement->LinearAcceleration;
		movement->AngularVelocity *= movement->AngularAcceleration;

		transform->SetLocalTranslation(transform->GetLocalTranslation() + movement->LinearVelocity * dt);
		// TODO: movement->AngularVelocity * dt
		transform->SetLocalRotation(transform->GetLocalRotation() * movement->AngularVelocity);
	}
}

void Invaders::MovementSystem::SetLinearVelocity(Poly::World* world, const Poly::UniqueID& id, const Poly::Vector& vel)
{
	world->GetComponent<MovementComponent>(id)->LinearVelocity = vel;
}

const Poly::Vector& Invaders::MovementSystem::GetLinearVelocity(Poly::World* world, Poly::UniqueID id)
{
	return world->GetComponent<MovementComponent>(id)->LinearVelocity;
}

void Invaders::MovementSystem::SetLinearAcceleration(Poly::World* world, const Poly::UniqueID& id, const Poly::Vector& acc)
{
	world->GetComponent<MovementComponent>(id)->LinearAcceleration = acc;
}

const Poly::Vector& Invaders::MovementSystem::GetLinearAcceleration(Poly::World* world, Poly::UniqueID id)
{
	return world->GetComponent<MovementComponent>(id)->LinearAcceleration;
}

void Invaders::MovementSystem::SetAngularVelocity(Poly::World* world, const Poly::UniqueID& id, const Poly::Quaternion& rot)
{
	world->GetComponent<MovementComponent>(id)->AngularVelocity = rot;
}

const Poly::Quaternion& Invaders::MovementSystem::GetAngularVelocity(Poly::World* world, Poly::UniqueID id)
{
	return world->GetComponent<MovementComponent>(id)->AngularVelocity;
}

void Invaders::MovementSystem::SetAngularAcceleration(Poly::World* world, const Poly::UniqueID& id, const Poly::Quaternion& acc)
{
	world->GetComponent<MovementComponent>(id)->AngularAcceleration = acc;
}

const Poly::Quaternion& Invaders::MovementSystem::GetAngularAcceleration(Poly::World* world, Poly::UniqueID id)
{
	return world->GetComponent<MovementComponent>(id)->AngularAcceleration;
}
