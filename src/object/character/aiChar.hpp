#ifndef AI_CHAR_HPP
#define AI_CHAR_HPP

#include <thread>
#include "character.hpp"

class AIChar final : public Character
{
	double actionTime = 0;
	std::thread moveThread;
	std::vector<std::pair<int, int>> path;
	std::string id;

public:
	AIChar(std::vector<Object *> &objects, const Position &pos = {0, 0.25, 0}, glm::vec3 angle = glm::vec3{0},
		   glm::vec3 scale = {1, 1, 1}, glm::vec3 color = {0, 0, 0}, std::string id = "AIChar") :
		Character(objects, pos, angle, scale, color), id(id)
	{
	}

	~AIChar() override { stopThread(); }

	void init() override
	{
		pathThread = std::thread(&AIChar::updateScore, this);
		moveThread = std::thread(&AIChar::move, this);
	}

	void stopThread() override
	{
		running = false;
		if (pathThread.joinable())
		{
			pathThread.join();
		}

		if (moveThread.joinable())
		{
			moveThread.join();
		}
	}

	// Thread function to randomly move the AI character
	void move()
	{
		while (running)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
			auto currPath = path; // Copy for thread safety

			if (currPath.size() < 10)
				continue;
			
			auto targetPoint = currPath[9];
			auto currPoint = currPath[0];
			auto targetPos = Position{float(targetPoint.first), 0.25, float(targetPoint.second)};
			auto currPos = Position{float(currPoint.first), 0.25, float(currPoint.second)};

			auto targetDir = targetPos - currPos;
			auto currDir = forward();

			auto currAngle = glm::atan(currDir.z, currDir.x);
			auto targetAngle = glm::atan(targetDir.z, targetDir.x);

			auto angle = targetAngle - currAngle;
			auto absAngle = glm::abs(angle);

			// Case 1: absAngle < 5 degrees -> only accelerate
			// Case 2: absAngle < 30 degrees -> accelerate and steer
			// Case 3: absAngle < 60 degrees -> only steer
			// Case 4: absAngle > 60 degrees -> break and steer

			if (absAngle < glm::radians(5.0f))
			{
				acceleratingState = ACCELERATING;
				steerState = NOT_STEERING;
				breakingState = NOT_BREAKING;
			}
			else if (absAngle < glm::radians(30.0f))
			{
				acceleratingState = ACCELERATING;
				steerState = angle > 0 ? STEERING_LEFT : STEERING_RIGHT;
				breakingState = NOT_BREAKING;
			}
			else if (absAngle < glm::radians(60.0f))
			{
				acceleratingState = NOT_ACCELERATING;
				steerState = angle > 0 ? STEERING_LEFT : STEERING_RIGHT;
				breakingState = NOT_BREAKING;
			}
			else
			{
				acceleratingState = NOT_ACCELERATING;
				steerState = angle > 0 ? STEERING_LEFT : STEERING_RIGHT;
				breakingState = BREAKING;
			}
		}
	}

	void updateScore() override
	{
		while (running)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
			auto mapController = MapController::getInstance();
			auto [newPath, newScore] = mapController->findPath(mapController->encodeMapCoord(pos.x), mapController->encodeMapCoord(pos.z), checkpointIdx, this);
			if (newPath.size() < 10) {
				checkpointIdx = (checkpointIdx + 1) % mapController->getCheckpoints().size();
			}
			path = newPath;
			score = newScore;
		}
	}
};

#endif // AI_CHAR_HPP
