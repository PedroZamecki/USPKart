#include "collisionBox.hpp"

#include <GL/glew.h>
#include <algorithm>
#include <glm/ext/matrix_transform.hpp>

#include "shader/shader.hpp"
#include "utils/logger.hpp"
#include "utils/position.hpp"

CollisionBox::CollisionBox(Position *pos, glm::vec3 *angle, const float width, const float height, const float depth) :
	pos(pos), angle(angle), width(width), height(height), depth(depth)
{
	vertices = {// Front face
				-width / 2, -height / 2, depth / 2, width / 2, -height / 2, depth / 2, width / 2, height / 2, depth / 2,
				-width / 2, height / 2, depth / 2,
				// Back face
				-width / 2, -height / 2, -depth / 2, width / 2, -height / 2, -depth / 2, width / 2, height / 2,
				-depth / 2, -width / 2, height / 2, -depth / 2,
				// Left face
				-width / 2, -height / 2, -depth / 2, -width / 2, -height / 2, depth / 2, -width / 2, height / 2,
				depth / 2, -width / 2, height / 2, -depth / 2,
				// Right face
				width / 2, -height / 2, -depth / 2, width / 2, -height / 2, depth / 2, width / 2, height / 2, depth / 2,
				width / 2, height / 2, -depth / 2,
				// Top face
				-width / 2, height / 2, -depth / 2, width / 2, height / 2, -depth / 2, width / 2, height / 2, depth / 2,
				-width / 2, height / 2, depth / 2,
				// Bottom face
				-width / 2, -height / 2, -depth / 2, width / 2, -height / 2, -depth / 2, width / 2, -height / 2,
				depth / 2, -width / 2, -height / 2, depth / 2};

	const unsigned int indices[] = {0,	1,	1,	2,	2,	3,	3,	0,	4,	5,	5,	6,	6,	7,	7,	4,
									8,	9,	9,	10, 10, 11, 11, 8,	12, 13, 13, 14, 14, 15, 15, 12,
									16, 17, 17, 18, 18, 19, 19, 16, 20, 21, 21, 22, 22, 23, 23, 20};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glLog();

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(vertices.size() * sizeof(float)), vertices.data(),
				 GL_STATIC_DRAW);

	glLog();

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
	glEnableVertexAttribArray(0);

	glLog();

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glLog();

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glLog();
}

void CollisionBox::draw(const Shader &shader, const glm::mat4 &model) const
{
	shader.use();
	shader.setMat4("model", model);

	glBindVertexArray(VAO);
	glLineWidth(2);
	glDrawElements(GL_LINES, 72, GL_UNSIGNED_INT, nullptr);
	glBindVertexArray(0);

	glLog();
}

std::vector<glm::vec3> CollisionBox::get3DProjectedVertices() const
{
	glm::mat4 projection{1};
	projection = glm::translate(projection, pos->toVec3());
	projection = glm::rotate(projection, angle->x, glm::vec3{1, 0, 0});
	projection = glm::rotate(projection, angle->y, glm::vec3{0, 1, 0});
	projection = glm::rotate(projection, angle->z, glm::vec3{0, 0, 1});

	std::vector<glm::vec3> projectedVertices;
	for (size_t i = 0; i < vertices.size(); i += 3)
	{
		const auto vertex = glm::vec4(vertices[i], vertices[i + 1], vertices[i + 2], 1.0f);
		const auto projectedVertex = projection * vertex;
		projectedVertices.emplace_back(projectedVertex.x, projectedVertex.y, projectedVertex.z);
	}

	return projectedVertices;
}

CollisionData CollisionBox::getCollisionForce(const CollisionBox &other) const
{
	const auto projection = [](float max, float min) -> float { return std::max(0.0f, std::min(max, min)); };
	const auto doesCollide = [](float min1, float max1, float min2, float max2) -> bool
	{ return max1 >= min2 && max2 >= min1; };
	const auto getOverlap = [](float min1, float max1, float min2, float max2) -> float
	{
		const auto overlap1 = max1 - min2;
		const auto overlap2 = max2 - min1;
		return std::min(overlap1, overlap2);
	};

	const auto vertices1 = get3DProjectedVertices();
	const auto vertices2 = other.get3DProjectedVertices();

	CollisionData collisionData;
	collisionData.penetration = std::numeric_limits<float>::max();

	for (size_t i = 0; i < vertices1.size(); i += 4)
	{
		const auto normal =
			glm::normalize(glm::cross(vertices1[i + 1] - vertices1[i], vertices1[i + 2] - vertices1[i]));
		const auto minMax1 =
			std::minmax_element(vertices1.begin(), vertices1.end(), [&normal](const glm::vec3 &a, const glm::vec3 &b)
								{ return glm::dot(a, normal) < glm::dot(b, normal); });
		const auto minMax2 =
			std::minmax_element(vertices2.begin(), vertices2.end(), [&normal](const glm::vec3 &a, const glm::vec3 &b)
								{ return glm::dot(a, normal) < glm::dot(b, normal); });

		const auto min1 = glm::dot(*minMax1.first, normal);
		const auto max1 = glm::dot(*minMax1.second, normal);
		const auto min2 = glm::dot(*minMax2.first, normal);
		const auto max2 = glm::dot(*minMax2.second, normal);

		if (!doesCollide(min1, max1, min2, max2))
		{
			return {0, {}, {}};
		}

		const auto overlap = getOverlap(min1, max1, min2, max2);
		if (overlap < collisionData.penetration)
		{
			collisionData.penetration = overlap;
			collisionData.normal = normal;
			collisionData.point = vertices1[i];
		}
	}

	// Ensure the normal points from this box to the other box
	glm::vec3 centerDiff = other.pos->toVec3() - pos->toVec3();
	if (glm::dot(centerDiff, collisionData.normal) < 0)
	{
		collisionData.normal = -collisionData.normal;
	}

	// Zero out the Y component of the normal
	collisionData.normal.y = 0.0f;
	collisionData.normal = glm::normalize(collisionData.normal);

	return collisionData;
}
