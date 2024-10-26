#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "character.hpp"

class Player final : public Character
{
public:
	Player() = default;
	bool isPlayer() override { return true; }
};

#endif //PLAYER_HPP
