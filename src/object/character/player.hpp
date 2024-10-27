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
		const auto upPressed = controls->getKeyState(GLFW_KEY_UP) > GLFW_RELEASE;
		const auto downPressed = controls->getKeyState(GLFW_KEY_DOWN) > GLFW_RELEASE;
		const auto leftPressed = controls->getKeyState(GLFW_KEY_LEFT) > GLFW_RELEASE;
		const auto rightPressed = controls->getKeyState(GLFW_KEY_RIGHT) > GLFW_RELEASE;

		steerState = static_cast<CharacterSteeringState>(leftPressed - rightPressed);

		// To see the accelerating state you need to check the speed of the kart,
		// The difference between not accelerating, breaking and reversing is the direction of the speed

		// If the kart is accelerating, check if it is also reversing
		// If the kart is not accelerating, check if it is breaking or reversing
		if (upPressed == downPressed)
			acceleratingState = upPressed ? BREAKING : NOT_ACCELERATING;
		else if (upPressed)
			acceleratingState = (speed < 0) ? BREAKING : ACCELERATING;
		else if (downPressed)
			acceleratingState = (speed > 0) ? BREAKING : REVERSING;

		Character::update(deltaTime);
	}
};

#endif // PLAYER_HPP
