#ifndef OBJECT_HPP
#define OBJECT_HPP

#include <functional>

#include "collisionBox.hpp"

typedef std::function<void(Position, float, unsigned int, float, unsigned int)> DrawingFunction;

class Object
{
public:
	Object();
	Object(Position pos, float width, float height, float depth, CollisionBox *box,
		   const DrawingFunction &drawFunction);
	~Object();
	void draw(unsigned int shaderProgram, float deltaTime, unsigned int nullTexture) const;

private:
	Position pos;
	float width{}, height{}, depth{}, angle{};
	CollisionBox *box{};
	DrawingFunction drawFunction;
};

#endif // OBJECT_HPP
