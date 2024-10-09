#ifndef COLLISION_BOX_HPP
#define COLLISION_BOX_HPP

#include "utils/position.hpp"

class CollisionBox
{
public:
	explicit CollisionBox(Position *pos, const float width = 1, const float height = 1, const float depth = 1) :
		pos(pos), width(width), height(height), depth(depth)
	{
	}
	bool isInside(const CollisionBox *box) const;

private:
	Position *pos;
	float width{1}, height{1}, depth{1};
};

#endif // COLLISION_BOX_HPP
