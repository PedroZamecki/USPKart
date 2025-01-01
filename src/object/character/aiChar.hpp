#ifndef AI_CHAR_HPP
#define AI_CHAR_HPP

#include <thread>
#include "character.hpp"

class AIChar final : public Character
{
	double actionTime = 0;
	std::string id;

public:
	AIChar(const Position &pos = {0, 0.25, 0}, glm::vec3 angle = glm::vec3{0},
		   glm::vec3 scale = {1, 1, 1}, glm::vec3 color = {0, 0, 0}, std::string id = "AIChar") :
		Character(pos, angle, scale, color), id(id)
	{
	}

	void move()
	{
		// Case 1: absAngle < 5 degrees -> only accelerate
		// Case 2: absAngle < 30 degrees -> accelerate and steer
		// Case 3: absAngle < 60 degrees -> only steer
		// Case 4: absAngle > 60 degrees -> break and steer

		// if (absAngle < glm::radians(5.0f))
		// {
		// 	acceleratingState = ACCELERATING;
		// 	steerState = NOT_STEERING;
		// 	breakingState = NOT_BREAKING;
		// }
		// else if (absAngle < glm::radians(30.0f))
		// {
		// 	acceleratingState = ACCELERATING;
		// 	steerState = angle > 0 ? STEERING_LEFT : STEERING_RIGHT;
		// 	breakingState = NOT_BREAKING;
		// }
		// else if (absAngle < glm::radians(60.0f))
		// {
		// 	acceleratingState = NOT_ACCELERATING;
		// 	steerState = angle > 0 ? STEERING_LEFT : STEERING_RIGHT;
		// 	breakingState = NOT_BREAKING;
		// }
		// else
		// {
		// 	acceleratingState = NOT_ACCELERATING;
		// 	steerState = angle > 0 ? STEERING_LEFT : STEERING_RIGHT;
		// 	breakingState = BREAKING;
		// }
	}
};

#endif // AI_CHAR_HPP
