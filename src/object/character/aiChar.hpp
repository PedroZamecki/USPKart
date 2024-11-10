#ifndef AI_CHAR_HPP
#define AI_CHAR_HPP

#include <thread>
#include "character.hpp"

class AIChar final : public Character
{
	double actionTime = 0;

public:
	AIChar(const Position &pos = {0, 0.5, 0}, glm::vec3 angle = glm::vec3{0}, glm::vec3 scale = {1, 1, 1}) :
		Character(pos, angle, scale)
	{
		// Start the thread to move the AI character
		std::thread t(&AIChar::move, this);
		t.detach();
	}

	// Thread function to randomly move the AI character
	void move()
	{
		while (true)
		{
			// Randomly move the AI character
			steerState = static_cast<CharacterSteeringState>((rand() % 3) - 1);
			acceleratingState = static_cast<CharacterAcceleratingState>((rand() % 3) - 1);
			breakingState = static_cast<CharacterBreakingState>(rand() % 2);
			actionTime = (rand() % 1000) / 1000.0;
			std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(actionTime * 1000)));
		}
	}
};

#endif // AI_CHAR_HPP
