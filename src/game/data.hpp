#ifndef DATA_HPP
#define DATA_HPP

#include <list>

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
		objects.push_front(track);
		objects.push_front(player);
	}

	~Data()
	{
		for (const auto &object : objects)
		{
			delete object;
		}
		objects.clear();
	}
	std::pmr::list<Object *> objects;
};

#endif // DATA_HPP
