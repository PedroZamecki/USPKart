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
	REVERSING = -1,
	NOT_ACCELERATING = 0,
	ACCELERATING = 1,
};

enum CharacterBreakingState
{
	NOT_BREAKING = 0,
	BREAKING = 1
};

class Character : public Kart
{
protected:
	CharacterSteeringState steerState{NOT_STEERING};
	CharacterAcceleratingState acceleratingState{NOT_ACCELERATING};
	CharacterBreakingState breakingState{NOT_BREAKING};

public:
	Character() = default;

	void updateBicycleModel(const float deltaTime)
	{
		const auto speed = getSpeed();
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
		const auto angleChange = angularSpeed * deltaTime;

		// Update the angle of the kart
		angle.y += angleChange;

		velocity = forward() * getSpeed();
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
		const float steeringAngleChange = 1.5f * steeringSpeed * deltaTime;

		steeringAngle += glm::clamp(angleDifference, -steeringAngleChange, steeringAngleChange);

		// Calculate the new speed of the kart based on the current accelerating state
		float maxSpeed{30};
		float minSpeed{-12};
		float targetSpeed{0}; // Change this based on the current state
		float acceleration{5};

		if (acceleratingState == ACCELERATING)
		{
			targetSpeed = maxSpeed;
			acceleration = 15;
		}
		else if (acceleratingState == REVERSING)
		{
			targetSpeed = minSpeed;
			acceleration = -6;
		}

		if (breakingState == BREAKING)
		{
			targetSpeed = 0;
			acceleration = -20;
		}

		// Adjust the speed towards the target
		float speedDifference = targetSpeed - getSpeed();
		float speedChange = acceleration * deltaTime;


		// Update the bicycle model
		updateBicycleModel(deltaTime);

		// Update the position of the kart based on the current speed
		velocity += forward() * glm::clamp(speedDifference, -speedChange, speedChange);
		pos += Position{velocity * deltaTime};
	}

	void draw(const Shader &shader, const float deltaTime, const glm::mat4 baseModel, const bool drawBoxes,
			  const Shader &boxShader) override
	{
		update(deltaTime);
		Kart::draw(shader, deltaTime, baseModel, drawBoxes, boxShader);
	}
};

#endif // CHARACTER_HPP
