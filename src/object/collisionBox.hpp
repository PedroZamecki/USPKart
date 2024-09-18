#ifndef COLLISION_BOX_HPP
#define COLLISION_BOX_HPP

#include "utils/position.hpp"

class CollisionBox
{
public:
	CollisionBox();
	CollisionBox(Position *pos, float width, float height, float depth);
	~CollisionBox() { delete pos; }
	bool isInside(const CollisionBox *box) const;

private:
	Position *pos;
	float width, height, depth;
};

#endif //COLLISION_BOX_HPP
