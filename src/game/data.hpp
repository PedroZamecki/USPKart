#ifndef DATA_HPP
#define DATA_HPP

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
		track = new Track();
		player = new Player();
		objects.push_back(track);
		objects.push_back(player);
		objects.push_back(new AIChar({3, .5, 3}));
		objects.push_back(new AIChar({5, .5, 3}));
		objects.push_back(new AIChar({7, .5, 3}));
		objects.push_back(new AIChar({9, .5, 3}));
		objects.push_back(new AIChar({11, .5, 3}));
		// objects.push_back(new Object("assets/models/block.obj", {128, 0.5, 0}, 1, 1, 1, {0, 0, 0}, {1, 1, 256}));
		// objects.push_back(new Object("assets/models/block.obj", {-128, .5, 0}, 1, 1, 1, {0, 0, 0}, {1, 1, 256}));
		// objects.push_back(new Object("assets/models/block.obj", {0, 0.5, 128}, 1, 1, 1, {0, 0, 0}, {1, 1, 256}));
		// objects.push_back(new Object("assets/models/block.obj", {0, .5, -128}, 1, 1, 1, {0, 0, 0}, {1, 1, 256}));
	}

	~Data()
	{
		for (const auto &object : objects)
		{
			delete object;
		}
		objects.clear();
	}
	std::vector<Object *> objects;
};

#endif // DATA_HPP
