#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "character.hpp"
#include "controls/controlsHandler.hpp"

class Player final : public Character
{
public:
	Player(Position pos, glm::vec3 angle, glm::vec3 scale,
		   glm::vec3 color = {1, 1, 1}) : Character(pos, angle, scale, color)
	{
		const auto controls = ControlsHandler::getInstance();
		controls->insertKeyCallback(GLFW_KEY_UP, [this]() -> void { checkState(); }, ALL);
		controls->insertKeyCallback(GLFW_KEY_DOWN, [this]() -> void { checkState(); }, ALL);
		controls->insertKeyCallback(GLFW_KEY_LEFT, [this]() -> void { checkState(); }, ALL);
		controls->insertKeyCallback(GLFW_KEY_RIGHT, [this]() -> void { checkState(); }, ALL);
		controls->insertKeyCallback(GLFW_KEY_T, [this]() -> void { printPlayerView(); }, PRESS);
		controls->insertKeyCallback(
			GLFW_KEY_P, [this]() -> void { Logger::getInstance()->info(std::to_string(this->score)); }, PRESS);
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
	}

	void printPlayerView()
	{
		MapController::getInstance()->saveObserverViewPPM("Teste.ppm", pos.x, pos.z, checkpointIdx, this);
	}
};

#endif // PLAYER_HPP
