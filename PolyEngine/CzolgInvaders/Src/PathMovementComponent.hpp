#pragma once

#include <ComponentBase.hpp>
#include <Vector.hpp>
#include <Quaternion.hpp>
#include <Queue.hpp>

#include "MovementSystem.hpp"

namespace Invaders
{
	namespace MovementSystem
	{

		class PathMovementComponent : public Poly::ComponentBase
		{
		public:
			PathMovementComponent(Poly::Queue<Poly::Vector>& points, double velocity, double acceleration, bool isRepeat = false, bool isRelative = false) :
				PointsQueue(points),
				Velocity(velocity),
				Acceleration(acceleration),
				IsPathRelative(isRelative),
				IsRepeat(isRepeat)

			{
				HasStarted = false;
				if (IsRepeat)
					BackupPointsQueue = PointsQueue;
			}
			PathMovementComponent() {}
			~PathMovementComponent() {}
			double const GetVelocity() { return Velocity; }
			double const GetAcceleration() { return Acceleration; }
			bool const GetIsPathRelative() { return IsPathRelative; }
			bool const GetIsRepeat() { return IsRepeat; }
			bool const GetHasStarted() { return HasStarted; }
			void SetHasStarted(bool value) { HasStarted = value; }

			Poly::Queue<Poly::Vector> PointsQueue;
			Poly::Queue<Poly::Vector> BackupPointsQueue;
			Poly::Vector CurrentPoint;

		private:
			double Velocity;
			double Acceleration;
			bool IsPathRelative;
			bool HasStarted;
			bool IsRepeat;
		};
	}
}