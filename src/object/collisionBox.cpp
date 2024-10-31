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

std::vector<glm::vec3> CollisionBox::get2DProjectedVertices() const
{
	// We won't consider the height of the object
	glm::mat4 projection{1};
	projection = rotate(projection, angle->x, glm::vec3{1, 0, 0});
	projection = rotate(projection, angle->y, glm::vec3{0, 1, 0});
	projection = rotate(projection, angle->z, glm::vec3{0, 0, 1});
	projection = translate(projection, pos->toVec3());

	std::vector<glm::vec3> projectedVertices;
	for (size_t i = 0; i < vertices.size(); i += 3)
	{
		const auto vertex = glm::vec4(vertices[i], vertices[i + 1], vertices[i + 2], 1.0f);
		const auto projectedVertex = projection * vertex;
		projectedVertices.emplace_back(projectedVertex.x, 0.0f, projectedVertex.y);
	}

	return projectedVertices;
}

CollisionData CollisionBox::getCollisionForce(const CollisionBox &other) const
{
	const auto vertices1 = get2DProjectedVertices();
	const auto vertices2 = other.get2DProjectedVertices();

	// Check if the boxes are colliding
	const auto checkCollision = [](const std::vector<glm::vec3> &v1, const std::vector<glm::vec3> &v2) -> bool
	{
		const auto [min1, max1] = std::minmax_element(v1.begin(), v1.end(), [](const auto &a, const auto &b) -> bool
													  { return a.x < b.x; });
		const auto [min2, max2] = std::minmax_element(v2.begin(), v2.end(), [](const auto &a, const auto &b) -> bool
													  { return a.x < b.x; });

		return min1->x <= max2->x && max1->x >= min2->x;
	};

	if (!checkCollision(vertices1, vertices2))
		return {};

	// Calculate the penetration
	const auto getPenetration = [](const glm::vec3 &v1, const glm::vec3 &v2) -> std::tuple<float, glm::vec3, glm::vec3>
	{
		const auto penetration = std::abs(v1.x - v2.x);
		const auto normal = glm::normalize(v1 - v2);
		const auto point = (v1 + v2) / 2.0f;
		return {penetration, normal, point};
	};

	float penetration = std::numeric_limits<float>::max();
	glm::vec3 normal{0};
	glm::vec3 point{0};

	for (const auto &vertex1 : vertices1)
	{
		for (const auto &vertex2 : vertices2)
		{
			if (const auto [pen, n, p] = getPenetration(vertex1, vertex2); pen < penetration)
			{
				penetration = pen;
				normal = n;
				point = p;
			}
		}
	}

	return {penetration, normal, point};
}
