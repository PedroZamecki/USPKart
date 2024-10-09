#include "collisionBox.hpp"

bool CollisionBox::isInside(const CollisionBox *box) const {
	// TODO: Implement this function
	return box == nullptr && this->pos == nullptr;
}
