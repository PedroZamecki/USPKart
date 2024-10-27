#include "cameraStates.hpp"
#include "../camera.hpp"
#include "utils/logger.hpp"

void updateCameraPosition(Camera *camera, const Player *player, const bool inverted, const bool behind)
{
	camera->setTarget(player->getPos());
	const auto newAngle =
		player->getAngle() + M_PI * (behind ? 1 : 0.5) + player->getSteeringAngle() * 0.1f + (inverted ? M_PI : 0);
	constexpr auto heightAngle = glm::radians(30.0f);

	// Change the target dist based on the speed of the kart
	// range from 4 to 5
	// Top speed = 20
	camera->setTargetDist(4 + player->getSpeed() / 20);

	const auto dist = camera->getTargetDist();
	camera->pos = camera->target +
		Position(dist * std::sin(newAngle), dist * std::sin(heightAngle),
				 dist * std::cos(newAngle));
}

void CameraBehindState::update(Camera *camera, const Player *player)
{
	updateCameraPosition(camera, player, getInverted(), true);
}

void CameraSideState::update(Camera *camera, const Player *player)
{
	updateCameraPosition(camera, player, getInverted(), false);
}
