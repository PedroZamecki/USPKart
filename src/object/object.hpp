#ifndef OBJECT_HPP
#define OBJECT_HPP

#include "collisionBox.hpp"
#include "model/model.hpp"

class Object
{
protected:
	Position pos{0, 0, 0};
	Model *model;
	float width{1}, height{1}, depth{1}, angle{0}, yaw{0};
	CollisionBox *box = new CollisionBox(&pos, width, height, depth);

public:
	explicit Object(const std::string &modelPath, Position pos = {0, 0, 0}, const float width = 1,
					const float height = 1, const float depth = 1, const float angle = 0, const float yaw = 0) :
		pos(pos), width(width), height(height), depth(depth), angle(angle), yaw(yaw)
	{
		box = new CollisionBox(&pos, width, height, depth);
		model = new Model(modelPath);
	}
	explicit Object(Model *model, Position pos = {0, 0, 0}, const float width = 1, const float height = 1,
					const float depth = 1, const float angle = 0) :
		pos(pos), model(model), width(width), height(height), depth(depth), angle(angle)
	{
		box = new CollisionBox(&pos, width, height, depth);
	}
	virtual ~Object() { delete box; }
	virtual void draw(const Shader &shader, float deltaTime) const;
};

#endif // OBJECT_HPP
