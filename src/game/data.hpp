#ifndef DATA_HPP
#define DATA_HPP

#include <vector>
#include <algorithm>
#include <random>

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
		auto positions = std::vector<Position>{
			{-100, .25, 50},
			{-100, .25, 38},
			{-100, .25, 26},
			{-100, .25, 14},
			{-92, .25, 44},
			{-92, .25, 32},
			{-92, .25, 20},
		};

		auto angle = glm::vec3{0, 3.14159265, 0};
		auto scale = glm::vec3{1, 1, 1};

		// Randomize the positions
		std::random_device rd;
		std::mt19937 g(rd());
		std::shuffle(positions.begin(), positions.end(), g);

		objects.push_back(track = new Track());
		objects.push_back(player = new Player(objects, positions[0], angle, scale, {1, 0, 0}));
		objects.push_back(new AIChar(objects, positions[1], angle, scale, {0, 1, 0}));
		objects.push_back(new Character(objects, positions[2], angle, scale, {0, 0, 1}));
		objects.push_back(new Character(objects, positions[3], angle, scale, {1, 0.5, 0}));
		objects.push_back(new Character(objects, positions[4], angle, scale, {1, 0, 0.5}));
		objects.push_back(new Character(objects, positions[5], angle, scale, {0, 0.5, 1}));
		objects.push_back(new Character(objects, positions[6], angle, scale, {0.5, 0, 1}));

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
