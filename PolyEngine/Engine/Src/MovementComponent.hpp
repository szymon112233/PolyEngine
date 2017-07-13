#pragma once

#include "ComponentBase.hpp"
#include "MovementSystem.hpp"
#include "Vector.hpp"

namespace Poly {

	class ENGINE_DLLEXPORT MovementComponent : public ComponentBase
	{
		friend void MovementSystem::MovementUpdatePhase(World*);
	public:
		MovementComponent();

		void SetMaxMovementSpeed(float value) { MaxMovementSpeed = value; }
		void SetMovementSpeed(float value) { MovementSpeed = value; }
		void SetMovementAcc(float value) { MovementAcc = value; }
		void SetDirection(Vector dir) 
		{
			if (dir.Length2() != 0 && dir.Length2() != 1)
					dir.Normalize();
			Direction = dir;
		}

		float GetMaxMovementSpeed() const { return MaxMovementSpeed; }
		Vector GetDirection() const { return Direction; }
		float GetMovementSpeed() const { return MovementSpeed; }
		float GetMovementAcc() const { return MovementAcc; }

	private:
		float MaxMovementSpeed = 1.0f;
		float MovementSpeed = 0.0f;
		float MovementAcc = 0.0f;
		Vector Direction = {0.0f, 0.0f, 0.0f};
		float DistanceTotal = 0.0f;
		float DistanceODO = 0.0f;
	};
}
