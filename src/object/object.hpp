#ifndef OBJECT_HPP
#define OBJECT_HPP

#include "collisionBox.hpp"
#include "graphic/utils/model.hpp"

class Object
{
	Position pos;
	Model *model;
	float width{}, height{}, depth{}, angle{};
	CollisionBox *box{};

public:
	explicit Object(const std::string &modelPath);
	Object(Position pos, const std::string &modelPath, float width, float height, float depth, CollisionBox *box);
	~Object();
	void draw(const Shader &shader, float deltaTime) const;
};

#endif // OBJECT_HPP
