#ifndef CHARACTER_HPP
#define CHARACTER_HPP

#include "../kart.hpp"
#include "utils/logger.hpp"

enum CharacterSteeringState
{
	STEERING_LEFT = -1,
	NOT_STEERING = 0,
	STEERING_RIGHT = 1
};

enum CharacterAcceleratingState
{
	BREAKING = -2,
	REVERSING = -1,
	NOT_ACCELERATING = 0,
	ACCELERATING = 1,
};

class Character : public Kart
{
protected:
	CharacterSteeringState steerState{NOT_STEERING};
	CharacterAcceleratingState acceleratingState{NOT_ACCELERATING};

public:
	Character() = default;

	void updateBicycleModel(const float deltaTime)
	{
		if (speed == 0)
			return;

		// 'this->angle' = Φ
		// 'this->steeringAngle' = δ
		// 'this->speed' = v

		const auto frontWheel = (frontWheels[0].getPos() + frontWheels[1].getPos()) * 0.5f;
		const auto backWheel = (backWheels[0].getPos() + backWheels[1].getPos()) * 0.5f;
		const auto L = glm::distance(frontWheel.toVec3(), backWheel.toVec3());

		// Calculate the new angle of the kart based on the current speed and steering angle
		float angularSpeed = speed / L * std::tan(steeringAngle);
		if (speed > 0 && acceleratingState == BREAKING)
			// Apply drifting logic
			angularSpeed *= 1.3f; // Example: increase angular speed by 30% when drifting
		const auto angleChange = angularSpeed * deltaTime;

		// Calculate the new position of the kart based on the current speed and angle
		angle += angleChange;
		const auto sinAngle = std::sin(angle);
		const auto cosAngle = std::cos(angle);
		const auto posChange = Position{speed * sinAngle, 0, speed * cosAngle} * deltaTime;

		// Update the position and angle of the kart
		pos += posChange;
	}

	virtual void update(const float deltaTime)
	{
		// Calculate the new steering angle of the kart based on the current steering state
		constexpr float maxSteeringAngle = glm::radians(30.0f);
		const float targetSteeringAngle = maxSteeringAngle * static_cast<float>(steerState);

		// Adjust the steering angle towards the target
		const float angleDifference = targetSteeringAngle - steeringAngle;

		// The bigger the angleDifference, the faster the steering
		const float steeringSpeed =
			targetSteeringAngle != 0.0f ? glm::clamp(angleDifference / targetSteeringAngle, 0.1f, 1.0f) : 1.0f;
		const float steeringAngleChange = 2 * steeringSpeed * deltaTime;

		steeringAngle += glm::clamp(angleDifference, -steeringAngleChange, steeringAngleChange);

		// Calculate the new speed of the kart based on the current accelerating state

		float acceleration{};
		float maxSpeed{};
		switch (acceleratingState)
		{
		case ACCELERATING:
			{
				acceleration = 10.0f;
				maxSpeed = 20.0f;
				break;
			}
		case NOT_ACCELERATING:
			{
				acceleration = 5.0f;
				maxSpeed = 0.0f;
				break;
			}
		case REVERSING:
			{
				acceleration = 10.0f;
				maxSpeed = -6.0f;
				break;
			}
		case BREAKING:
			{
				// Change the acceleration based on the steering angle, the bigger the angle, the slower the kart stops
				acceleration = 15.0f * (1.0f - std::abs(steeringAngle) / maxSteeringAngle);
				maxSpeed = 0.0f;
				break;
			}
		}

		const float speedChange = acceleration * deltaTime;
		const float speedDifference = maxSpeed - speed;
		speed += glm::clamp(speedDifference, -speedChange, speedChange);

		updateBicycleModel(deltaTime);
	}

	void draw(const Shader &shader, const float deltaTime, const glm::mat4 baseModel, const bool drawBoxes, const Shader &boxShader) override
	{
		update(deltaTime);
		Kart::draw(shader, deltaTime, baseModel, drawBoxes, boxShader);
	}
};

#endif // CHARACTER_HPP
