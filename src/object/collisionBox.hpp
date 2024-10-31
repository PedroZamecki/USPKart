#ifndef COLLISION_BOX_HPP
#define COLLISION_BOX_HPP

#include <glm/fwd.hpp>
#include <glm/vec3.hpp>
#include <vector>

#include "model/bone.hpp"

class Shader;
class Position;

struct CollisionData {
	float penetration{};
	glm::vec3 normal{};
	glm::vec3 point{};
};

class CollisionBox
{
	Position *pos;
	glm::vec3 *angle{0};
	float width{1}, height{1}, depth{1};
	unsigned int VAO{}, VBO{}, EBO{};
	std::vector<float> vertices;

public:
	explicit CollisionBox(Position *pos, glm::vec3 *angle, float width = 1, float height = 1, float depth = 1);
	void draw(const Shader &shader, const glm::mat4 &model) const;
	std::vector<glm::vec3> get3DProjectedVertices() const;
	[[nodiscard]] CollisionData getCollisionForce(const CollisionBox &other) const;
};

#endif // COLLISION_BOX_HPP
