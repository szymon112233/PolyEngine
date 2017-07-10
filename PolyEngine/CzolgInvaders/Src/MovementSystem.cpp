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

		if ((pow(transform->GetLocalTranslation().X - (path->Points[path->GetCurrentPoint()] + path->GetPathOffset()).X, 2)
			+ pow(transform->GetLocalTranslation().Z - (path->Points[path->GetCurrentPoint()] + path->GetPathOffset()).Z, 2)
			+ pow(transform->GetLocalTranslation().Y - (path->Points[path->GetCurrentPoint()] + path->GetPathOffset()).Y, 2)) < threshold2)
		{
			if (path->Points.GetSize() - 1 > path->GetCurrentPoint())
			{
				int new_value = path->GetCurrentPoint();
				new_value++;
				path->SetCurrentPoint(new_value);
				if (path->GetPathOffset().Length2() != 0.0f)
					path->SetPathOffset(transform->GetLocalTranslation());
				Poly::Vector direction = (path->Points[path->GetCurrentPoint()] + path->GetPathOffset()) - transform->GetLocalTranslation();
				movement->LinearVelocity = direction.Normalize();
			}
			else
			{
				if (path->GetIsRepeat())
				{
					path->SetCurrentPoint(0);
					if (path->GetPathOffset().Length2() != 0.0f)
						path->SetPathOffset(transform->GetLocalTranslation());
					Poly::Vector direction = (path->Points[path->GetCurrentPoint()] + path->GetPathOffset()) - transform->GetLocalTranslation();
					movement->LinearVelocity = direction.Normalize();
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
			Poly::Vector direction = (path->Points[path->GetCurrentPoint()] + path->GetPathOffset()) - transform->GetLocalTranslation();
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
