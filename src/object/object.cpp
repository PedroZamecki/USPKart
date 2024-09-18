#include "object.hpp"

Object::Object()
{
	pos = {0, 0, 0};
	width = 0;
	height = 0;
	depth = 0;
	box = new CollisionBox();
	drawFunction = nullptr;
}
Object::Object(const Position pos, const float width, const float height, const float depth, CollisionBox *box,
			   const DrawingFunction &drawFunction) :
	pos(pos), width(width), height(height), depth(depth), box(box), drawFunction(drawFunction)
{
}

Object::~Object()
{
	pos = {0, 0, 0};
	width = 0;
	height = 0;
	depth = 0;
	delete box;
}
void Object::draw(const unsigned int shaderProgram, const float deltaTime, const unsigned int nullTexture) const
{
	drawFunction(pos, angle, shaderProgram, deltaTime, nullTexture);
}
