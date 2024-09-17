#ifndef OBJECT_HPP
#define OBJECT_HPP

#include <functional>

#include "collisionBox.hpp"

class Object
{
public:
	Object();
	~Object();

private:
	Position pos;
	float width, height, depth;
	CollisionBox box;
	std::function<void()> drawFunction();
};

#endif // OBJECT_HPP
