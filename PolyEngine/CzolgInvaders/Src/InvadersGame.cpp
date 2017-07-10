#include "InvadersGame.hpp"

#include <DeferredTaskSystem.hpp>
#include <CameraComponent.hpp>
#include <TransformComponent.hpp>
#include <MeshRenderingComponent.hpp>
#include <FreeFloatMovementComponent.hpp>
#include <ScreenSpaceTextComponent.hpp>
#include <Core.hpp>
#include <DeferredTaskSystem.hpp>
#include <ViewportWorldComponent.hpp>
#include <ResourceManager.hpp>

#include "GameManagerSystem.hpp"
#include "MovementComponent.hpp"
#include "MovementSystem.hpp"
#include "CollisionComponent.hpp"
#include "CollisionSystem.hpp"
#include "TankComponent.hpp"
#include "PathMovementComponent.hpp"

using namespace Poly;

DEFINE_GAME(InvadersGame)

void InvadersGame::Init()
{
	gEngine->RegisterComponent<PlayerControllerComponent>((int)eGameComponents::PLAYERCONTROLLER);
	gEngine->RegisterComponent<BulletComponent>((int)eGameComponents::BULLET);
	gEngine->RegisterComponent<GameManagerComponent>((int)eGameComponents::GAMEMANAGER);
	gEngine->RegisterComponent<EnemyMovementComponent>((int)eGameComponents::ENEMYMOVEMENT);
	gEngine->RegisterComponent<Invaders::MovementSystem::MovementComponent>((int)eGameComponents::MOVEMENT);
	gEngine->RegisterComponent<Invaders::MovementSystem::PathMovementComponent>((int)eGameComponents::PATHMOVEMENT);
	gEngine->RegisterComponent<Invaders::CollisionSystem::CollisionComponent>((int)eGameComponents::COLLISION);
	gEngine->RegisterComponent<Invaders::TankComponent>((int)eGameComponents::TANK);
	
	Camera = DeferredTaskSystem::SpawnEntityImmediate(gEngine->GetWorld());
	DeferredTaskSystem::AddComponentImmediate<Poly::TransformComponent>(gEngine->GetWorld(), Camera);
	DeferredTaskSystem::AddComponentImmediate<Poly::CameraComponent>(gEngine->GetWorld(), Camera, 60_deg, 1.0f, 1000.f);
	DeferredTaskSystem::AddComponentImmediate<Poly::FreeFloatMovementComponent>(gEngine->GetWorld(), Camera, 10.0f, 0.003f);

	float y_pos = gEngine->GetRenderingDevice()->GetScreenSize().Height;
	auto textDispaly = DeferredTaskSystem::SpawnEntityImmediate(gEngine->GetWorld());
	DeferredTaskSystem::AddComponentImmediate<Poly::ScreenSpaceTextComponent>(gEngine->GetWorld(), textDispaly, Vector{ 0.0f, y_pos ,0.0f }, "Fonts/Raleway/Raleway-Heavy.ttf", 32, "Kill count: 0");
	
	GameManager = DeferredTaskSystem::SpawnEntityImmediate(gEngine->GetWorld());
	DeferredTaskSystem::AddComponentImmediate<GameManagerComponent>(gEngine->GetWorld(), GameManager, textDispaly);
	GameManagerComponent* gameManagerComponent = gEngine->GetWorld()->GetComponent<GameManagerComponent>(GameManager);
	

	// Set some camera position
	Poly::TransformComponent* cameraTrans = gEngine->GetWorld()->GetComponent<Poly::TransformComponent>(Camera);
	cameraTrans->SetLocalTranslation(Vector(0.0f, 20.0f, 60.0f));
	cameraTrans->SetLocalRotation(Quaternion({ 1, 0, 0 }, -30_deg));

	Dynarray<Vector> testPath;
	//testPath.PushBack({ 0,0,0 });
	testPath.PushBack({ 10,0,0 });
	testPath.PushBack({ 0,0,10 });
	testPath.PushBack({ -10,0,0 });
	testPath.PushBack({ 0,0,-10 });
	for (int i = -1; i < 0; ++i)
	{
		for (int j = 0; j < 1; ++j)
		{
			auto ent = DeferredTaskSystem::SpawnEntityImmediate(gEngine->GetWorld());
			DeferredTaskSystem::AddComponentImmediate<Poly::TransformComponent>(gEngine->GetWorld(), ent);
			DeferredTaskSystem::AddComponentImmediate<Poly::MeshRenderingComponent>(gEngine->GetWorld(), ent, "model-tank/turret.fbx");
			Poly::TransformComponent* entTransform = gEngine->GetWorld()->GetComponent<Poly::TransformComponent>(ent);

			auto base = DeferredTaskSystem::SpawnEntityImmediate(gEngine->GetWorld());
			DeferredTaskSystem::AddComponentImmediate<Poly::TransformComponent>(gEngine->GetWorld(), base);
			DeferredTaskSystem::AddComponentImmediate<Poly::MeshRenderingComponent>(gEngine->GetWorld(), base, "model-tank/base.fbx");
			DeferredTaskSystem::AddComponentImmediate<Invaders::MovementSystem::MovementComponent>(gEngine->GetWorld(), base, Vector(5, 0, 0), Vector(0, 0, 0), 
				Quaternion(Vector(0, 0, 0), 0_deg), Quaternion(Vector(0, 0, 0), 0_deg));
			DeferredTaskSystem::AddComponentImmediate<Invaders::CollisionSystem::CollisionComponent>(gEngine->GetWorld(), base,  Vector(0, 0, 0), Vector(5.0f, 5.0f, 5.0f));
			DeferredTaskSystem::AddComponentImmediate<Invaders::TankComponent>(gEngine->GetWorld(), base,  ent, 12.0_deg, (i * j)%5 );
			Poly::TransformComponent* baseTransform = gEngine->GetWorld()->GetComponent<Poly::TransformComponent>(base);
			
			entTransform->SetParent(baseTransform);
			baseTransform->SetLocalTranslation(Vector(i * 12, 0, j * 8));
			baseTransform->SetLocalRotation(Quaternion(Vector::UNIT_Y, -90.0_deg));
			entTransform->SetLocalRotation(Quaternion(Vector::UNIT_Y, -60.0_deg));

			DeferredTaskSystem::AddComponentImmediate<Invaders::MovementSystem::PathMovementComponent>(gEngine->GetWorld(), base, testPath, 10.0f, 0.1f, true);
		}
	}
	auto player = DeferredTaskSystem::SpawnEntityImmediate(gEngine->GetWorld());
	DeferredTaskSystem::AddComponentImmediate<Poly::TransformComponent>(gEngine->GetWorld(), player);
	DeferredTaskSystem::AddComponentImmediate<Poly::MeshRenderingComponent>(gEngine->GetWorld(), player, "Models/tank2/bradle.3ds");
	DeferredTaskSystem::AddComponentImmediate<PlayerControllerComponent>(gEngine->GetWorld(), player, 10.0f);
	Poly::TransformComponent* entTransform = gEngine->GetWorld()->GetComponent<Poly::TransformComponent>(player);
	entTransform->SetLocalTranslation(Vector(0, 0, 50));
	entTransform->SetLocalScale(10);
	entTransform->SetLocalRotation(Quaternion(Vector::UNIT_Y, -90_deg) * Quaternion(Vector::UNIT_X, -90_deg));
	
	gEngine->GetWorld()->GetWorldComponent<ViewportWorldComponent>()->SetCamera(0, gEngine->GetWorld()->GetComponent<Poly::CameraComponent>(Camera));
	gEngine->RegisterGameUpdatePhase(Invaders::MovementSystem::MovementUpdatePhase);
	gEngine->RegisterGameUpdatePhase(Invaders::CollisionSystem::CollisionUpdatePhase);
	gEngine->RegisterGameUpdatePhase(GameMainSystem::GameUpdate);
	gEngine->RegisterGameUpdatePhase(ControlSystem::ControlSystemPhase);
	gEngine->RegisterGameUpdatePhase(GameManagerSystem::GameManagerSystemPhase);

	for(int x = -1; x <= 1; ++x)
		for (int z = -1; z <= 1; ++z)
		{
			const float SCALE = 4.0f;
			const float SIZE = 40.0f;

			auto ground = DeferredTaskSystem::SpawnEntityImmediate(gEngine->GetWorld());
			DeferredTaskSystem::AddComponentImmediate<Poly::TransformComponent>(gEngine->GetWorld(), ground);
			DeferredTaskSystem::AddComponentImmediate<Poly::MeshRenderingComponent>(gEngine->GetWorld(), ground, "Models/ground/ground.fbx");
			Poly::TransformComponent* groundTransform = gEngine->GetWorld()->GetComponent<Poly::TransformComponent>(ground);
			groundTransform->SetLocalTranslation(Vector(x * SCALE * SIZE, 0, z * SCALE * SIZE));
			groundTransform->SetLocalScale(SCALE);
		}

	

	// Precache bullet mesh
	BulletMesh = Poly::ResourceManager<MeshResource>::Load("Models/bullet/lowpolybullet.obj");
};

void InvadersGame::Deinit()
{
	DeferredTaskSystem::DestroyEntityImmediate(gEngine->GetWorld(), Camera);
	for(auto ent : GameEntities)
		DeferredTaskSystem::DestroyEntityImmediate(gEngine->GetWorld(), ent);
	Poly::ResourceManager<MeshResource>::Release(BulletMesh);
};

void GameMainSystem::GameUpdate(Poly::World* /*world*/)
{
	
}
