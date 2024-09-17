#ifndef CHARACTER_HPP
#define CHARACTER_HPP

#include "object.hpp"

class Character : Object
{
public:
	Character();
	~Character();

private:
	Position pos;
	CollisionBox box;
};

#endif //CHARACTER_HPP
