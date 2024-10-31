#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "character.hpp"
#include "controls/controlsHandler.hpp"

class Player final : public Character
{
public:
	Player()
	{
		const auto controls = ControlsHandler::getInstance();
		controls->insertKeyCallback(GLFW_KEY_UP, [this]() -> void { checkState(); }, ALL);
		controls->insertKeyCallback(GLFW_KEY_DOWN, [this]() -> void { checkState(); }, ALL);
		controls->insertKeyCallback(GLFW_KEY_LEFT, [this]() -> void { checkState(); }, ALL);
		controls->insertKeyCallback(GLFW_KEY_RIGHT, [this]() -> void { checkState(); }, ALL);
	}

	void checkState()
	{
		const auto controls = ControlsHandler::getInstance();
		const auto upPressed = controls->getKeyState(GLFW_KEY_UP) > GLFW_RELEASE;
		const auto downPressed = controls->getKeyState(GLFW_KEY_DOWN) > GLFW_RELEASE;
		const auto leftPressed = controls->getKeyState(GLFW_KEY_LEFT) > GLFW_RELEASE;
		const auto rightPressed = controls->getKeyState(GLFW_KEY_RIGHT) > GLFW_RELEASE;

		steerState = static_cast<CharacterSteeringState>(leftPressed - rightPressed);

		acceleratingState = static_cast<CharacterAcceleratingState>(upPressed - downPressed);

		// speed == 0 -> never break
		// speed > 0 -> break if accelerating == -1
		breakingState = static_cast<CharacterBreakingState>(getSpeed() > 0.1 && acceleratingState == -1);


		Logger::getInstance()->info("Steering: " + std::to_string(steerState) + " Accelerating: " +
									std::to_string(acceleratingState) + " Breaking: " + std::to_string(breakingState));
	}
};

#endif // PLAYER_HPP
