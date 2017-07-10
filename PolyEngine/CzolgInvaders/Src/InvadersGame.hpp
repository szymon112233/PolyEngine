#pragma once


#include "PlayerControllerComponent.hpp"
#include "BulletComponent.hpp"
#include "ControlSystem.hpp"
#include "GameManagerComponent.hpp"
#include "EnemyMovementComponent.hpp"

#include <CameraComponent.hpp>
#include <TransformComponent.hpp>
#include <MeshRenderingComponent.hpp>
#include <FreeFloatMovementComponent.hpp>
#include <Core.hpp>
#include <Engine.hpp>
#include <World.hpp>

namespace GameMainSystem
{
	void GameUpdate(Poly::World* world);
}

enum class eGameComponents
{
	PLAYERCONTROLLER = (int)Poly::eEngineComponents::_COUNT,
	BULLET,
	GAMEMANAGER,
	ENEMYMOVEMENT,
	MOVEMENT,
	PATHMOVEMENT,
	COLLISION,
	TANK
};

DECLARE_GAME()
class GAME_DLLEXPORT InvadersGame : public Poly::IGame {
public:
	void RegisterEngine(Poly::Engine* engine) override { }
	
	void Init() override;
	void Deinit() override;

private:
	Poly::UniqueID Camera;
	Poly::UniqueID GameManager;

	Poly::Dynarray<Poly::UniqueID> GameEntities;
	Poly::MeshResource* BulletMesh;
};