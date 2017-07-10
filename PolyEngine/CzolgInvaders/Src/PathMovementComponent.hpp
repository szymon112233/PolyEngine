#pragma once

#include <ComponentBase.hpp>
#include <Vector.hpp>
#include <Quaternion.hpp>
#include <Dynarray.hpp>

#include "MovementSystem.hpp"

namespace Invaders
{
	namespace MovementSystem
	{

		class PathMovementComponent : public Poly::ComponentBase
		{
		public:
			PathMovementComponent(Poly::Dynarray<Poly::Vector>& points, double velocity, double acceleration, bool isRepeat = false) :
				Points(points),
				Velocity(velocity),
				Acceleration(acceleration),
				PathOffset({ 0.0, 0.0, 0.0 }),
				IsRepeat(isRepeat)
			{}
			PathMovementComponent(Poly::Dynarray<Poly::Vector>& points, double velocity, double acceleration, Poly::Vector& offset, bool isRepeat = false) :
				Points(points),
				Velocity(velocity),
				Acceleration(acceleration),
				PathOffset(offset),
				IsRepeat(isRepeat)
			{}
			PathMovementComponent() {}
			~PathMovementComponent() {}
			double const GetVelocity() { return Velocity; }
			double const GetAcceleration() { return Acceleration; }
			bool const GetIsRepeat() { return IsRepeat; }
			int GetCurrentPoint() { return CurrentPoint; }
			void SetCurrentPoint(int value) { CurrentPoint = value; }
			Poly::Vector const GetPathOffset() { return PathOffset; }
			void SetPathOffset(Poly::Vector new_offset) { PathOffset = new_offset; }

			Poly::Dynarray<Poly::Vector> Points;

		private:
			double Velocity;
			double Acceleration;
			int CurrentPoint = 0;
			Poly::Vector PathOffset;
			bool IsRepeat;
		};
	}
}