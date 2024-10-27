#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "character.hpp"

class Player final : public Character
{
public:
	Player() = default;
	bool isPlayer() override { return true; }

	void update(const float deltaTime) override
	{
		const auto controls = ControlsHandler::getInstance();
		const auto wPressed = controls->getKeyState(GLFW_KEY_W) > GLFW_RELEASE;
		const auto sPressed = controls->getKeyState(GLFW_KEY_S) > GLFW_RELEASE;
		const auto aPressed = controls->getKeyState(GLFW_KEY_A) > GLFW_RELEASE;
		const auto dPressed = controls->getKeyState(GLFW_KEY_D) > GLFW_RELEASE;

		steerState = static_cast<CharacterSteeringState>(aPressed - dPressed);

		// To see the accelerating state you need to check the speed of the kart,
		// The difference between not accelerating, breaking and reversing is the direction of the speed

		// If the kart is accelerating, check if it is also reversing
		// If the kart is not accelerating, check if it is breaking or reversing
		if (wPressed == sPressed)
			acceleratingState = NOT_ACCELERATING;
		else if (wPressed)
			acceleratingState = (speed < 0) ? BREAKING : ACCELERATING;
		else if (sPressed)
			acceleratingState = (speed > 0) ? BREAKING : REVERSING;

		Character::update(deltaTime);
	}
};

#endif // PLAYER_HPP
