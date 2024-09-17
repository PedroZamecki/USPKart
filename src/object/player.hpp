#ifndef PLAYER_HPP
#define PLAYER_HPP
#include "character.hpp"

class Player : Character
{
public:
	Player();
	~Player();

private:
	Position pos;
	float width, height, depth;
	CollisionBox box;
};

#endif //PLAYER_HPP
