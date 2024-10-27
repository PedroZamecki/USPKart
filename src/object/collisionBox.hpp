#ifndef COLLISION_BOX_HPP
#define COLLISION_BOX_HPP

#include <glm/fwd.hpp>
#include <vector>

class Shader;
class Position;

class CollisionBox
{
	Position *pos;
	float *yaw{}, *pitch{}, *angle{};
	float width{1}, height{1}, depth{1};
	unsigned int VAO{}, VBO{}, EBO{};
	std::vector<float> vertices;

public:
	explicit CollisionBox(Position *pos, float *yaw, float *pitch, float *angle, float width = 1, float height = 1, float depth = 1);
	void draw(const Shader &shader, const glm::mat4 &model) const;
};

#endif // COLLISION_BOX_HPP
