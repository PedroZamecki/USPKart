#ifndef DATA_HPP
#define DATA_HPP

#include <list>

#include "object/character/player.hpp"
#include "object/character/aiChar.hpp"
#include "object/object.hpp"
#include "object/track.hpp"

class Data
{
public:
	Track *track;
	Player *player;

	Data()
	{
		const auto ai = new AIChar({3, 0.5, 3});
		track = new Track();
		player = new Player();
		objects.push_front(track);
		objects.push_front(player);
		objects.push_front(ai);
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
