#include "collisionBox.hpp"

CollisionBox::CollisionBox() : pos(nullptr), width(0), height(0), depth(0) {}

CollisionBox::CollisionBox(Position *pos, const float width, const float height, const float depth) :
	pos(pos), width(width), height(height), depth(depth)
{
}

bool CollisionBox::isInside(const CollisionBox *box) const {
	// TODO: Implement this function
	return box == nullptr && this->pos == nullptr;
}
