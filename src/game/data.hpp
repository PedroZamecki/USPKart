#ifndef DATA_HPP
#define DATA_HPP

#include <algorithm>
#include <random>
#include <vector>

#include "object/character/aiChar.hpp"
#include "object/character/player.hpp"
#include "object/object.hpp"
#include "object/track.hpp"

class Data
{
public:
	Track *track;
	Player *player;

	Data()
	{
		MapController::getInstance()->configure("assets/map/default.pgm",
									   {{{175, 508}, {90, 508}},
										{{426, 132}, {426, 73}},
										{{838, 786}, {838, 1014}},
										{{441, 438}, {441, 261}},
										{{377, 817}, {365, 1013}}
										});
		
		MapController::getInstance()->setObjects(objects);

		auto positions = std::vector<Position>{
			{-100, .25, 50},
			{-100, .25, 38},
			{-100, .25, 26},
			{-100, .25, 14},
			{-92, .25, 44},
			{-92, .25, 32},
			{-92, .25, 20},
		};

		auto colors = std::vector<glm::vec3>{
			{1, 0, 0},
			{0, 1, 0},
			{0, 0, 1},
			{1, 0.5, 0},
			{1, 0, 0.5},
			{0, 0.5, 1},
			{0.5, 0, 1},
		};

		auto angle = glm::vec3{0, 3.14159265, 0};
		auto scale = glm::vec3{1, 1, 1};

		// Randomize the positions
		std::random_device rd;
		std::mt19937 g(rd());
		std::shuffle(positions.begin(), positions.end(), g);

		objects.push_back(track = new Track());
		objects.push_back(player = new Player(positions[0], angle, scale, colors[0]));
		objects.push_back(new Character(positions[1], angle, scale, colors[1]));
		objects.push_back(new Character(positions[2], angle, scale, colors[2]));
		objects.push_back(new Character(positions[3], angle, scale, colors[3]));
		objects.push_back(new Character(positions[4], angle, scale, colors[4]));
		objects.push_back(new Character(positions[5], angle, scale, colors[5]));
		objects.push_back(new Character(positions[6], angle, scale, colors[6]));

		// After everything is set up, initialize the characters
		for (const auto &object : objects)
			object->init();
	}

	~Data()
	{
		for (const auto &object : objects) {
			if (auto character = dynamic_cast<Character*>(object)) {
				character->stopThread();
			}
			delete object;
		}
		objects.clear();
	}
	std::vector<Object *> objects;
};

#endif // DATA_HPP
