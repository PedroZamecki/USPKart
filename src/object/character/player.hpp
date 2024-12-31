#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "character.hpp"
#include "controls/controlsHandler.hpp"

class Player final : public Character
{
public:
	Player(std::vector<Object *> &objects, Position pos, glm::vec3 angle, glm::vec3 scale,
		   glm::vec3 color = {1, 1, 1}) : Character(objects, pos, angle, scale, color)
	{
		const auto controls = ControlsHandler::getInstance();
		controls->insertKeyCallback(GLFW_KEY_UP, [this]() -> void { checkState(); }, ALL);
		controls->insertKeyCallback(GLFW_KEY_DOWN, [this]() -> void { checkState(); }, ALL);
		controls->insertKeyCallback(GLFW_KEY_LEFT, [this]() -> void { checkState(); }, ALL);
		controls->insertKeyCallback(GLFW_KEY_RIGHT, [this]() -> void { checkState(); }, ALL);
		controls->insertKeyCallback(GLFW_KEY_T, [this]() -> void { printMapState(); }, PRESS);
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

	void printMapState()
	{
		MapController::getInstance()->saveModifiedPPM("Teste.ppm", pos.x, pos.z, checkpointIdx, this);
	}

	void updateScore() override
	{
		while (running)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
			auto mapController = MapController::getInstance();
			auto pathResult = mapController->findPath(mapController->encodeMapCoord(pos.x), mapController->encodeMapCoord(pos.z), checkpointIdx, this);
			if (pathResult.first.size() < 10) {
				checkpointIdx = (checkpointIdx + 1) % mapController->getCheckpoints().size();
				Logger::getInstance()->info("Checkpoint: " + std::to_string(checkpointIdx));
			}
			score = pathResult.second;
		}
	}
};

#endif // PLAYER_HPP
