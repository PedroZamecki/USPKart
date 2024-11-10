#ifndef COLLISION_BOX_HPP
#define COLLISION_BOX_HPP

#include <glm/fwd.hpp>
#include <glm/vec3.hpp>
#include <vector>

#include "model/bone.hpp"

class Shader;
class Position;

struct CollisionData
{
	float penetration{0};
	glm::vec3 normal{0};
	glm::vec3 point{0};
};

class CollisionBox
{
	Position *pos;
	glm::vec3 *angle{0};
	glm::vec3 *scale{};
	const float *width{};
	const float *height{};
	const float *depth{};
	unsigned int VAO{}, VBO{}, EBO{};
	std::vector<float> vertices;
	std::vector<unsigned int> indices;

	bool linesIntersect(const glm::vec2 &p1, const glm::vec2 &p2, const glm::vec2 &q1, const glm::vec2 &q2, glm::vec3 &intersection) const;

public:
	explicit CollisionBox(Position *pos, glm::vec3 *angle, glm::vec3 *scale, const float *width, const float *height,
						  const float *depth);
	void draw(const Shader &shader) const;
	std::vector<glm::vec2> get2DProjectedVertices() const;
	glm::mat4 getTransformMatrix() const;
	[[nodiscard]] CollisionData getCollisionForce(const CollisionBox &other) const;
};

#endif // COLLISION_BOX_HPP
