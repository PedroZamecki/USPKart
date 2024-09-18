#ifndef DATA_HPP
#define DATA_HPP

#include <list>

#include "object/object.hpp"

class Data
{
public:
	Data() = default;
	~Data()
	{
		objects.clear();
		for (const auto &object : objects)
			delete object;
		delete &objects;
	}
	std::pmr::list<Object *> objects;
};

#endif // DATA_HPP
