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
			{-100, .5, 50},
			{-100, .5, 38},
			{-100, .5, 26},
			{-100, .5, 14},
			{-92, .5, 44},
			{-92, .5, 32},
			{-92, .5, 20},
		};

		auto angle = glm::vec3{0, 3.14159265, 0};

		// Randomize the positions
		std::random_device rd;
		std::mt19937 g(rd());
		std::shuffle(positions.begin(), positions.end(), g);

		objects.push_back(track = new Track());
		objects.push_back(player = new Player(objects, positions[0], angle));
		objects.push_back(new AIChar(objects, positions[1], angle));
		objects.push_back(new AIChar(objects, positions[2], angle));
		objects.push_back(new AIChar(objects, positions[3], angle));
		objects.push_back(new AIChar(objects, positions[4], angle));
		objects.push_back(new AIChar(objects, positions[5], angle));
		objects.push_back(new AIChar(objects, positions[6], angle));
	}

	~Data()
	{
		for (const auto &object : objects)
			delete object;
		objects.clear();
	}
	std::vector<Object *> objects;
};

#endif // DATA_HPP
