#include "collisionBox.hpp"

#include <GL/glew.h>
#include <algorithm>
#include <glm/ext/matrix_transform.hpp>

#include "shader/shader.hpp"
#include "utils/logger.hpp"
#include "utils/position.hpp"

CollisionBox::CollisionBox(Position *pos, glm::vec3 *angle, glm::vec3 *scale, const float *width, const float *height,
						   const float *depth) :
	pos(pos), angle(angle), scale(scale), width(width), height(height), depth(depth)
{
	// We'll create an ellipsis in the y=0 plane to represent the collision box
	// for drawing purposes only we'll represent it as a cylinder
	const auto segments = 30;
	const auto step = 2 * M_PI / segments;
	for (auto i = 0; i <= segments; i++)
	{
		const auto angle = i * step;
		const auto x = cos(angle) * *width / 2;
		const auto z = sin(angle) * *depth / 2;

		// Bottom
		vertices.push_back(x);
		vertices.push_back(-*height / 2);
		vertices.push_back(z);

		// Top
		vertices.push_back(x);
		vertices.push_back(*height / 2);
		vertices.push_back(z);
	}

	// Indices
	for (auto i = 0; i < segments; i++)
	{
		// Bottom
		indices.push_back(i * 2);
		indices.push_back(i * 2 + 2);

		// Top
		indices.push_back(i * 2 + 1);
		indices.push_back(i * 2 + 3);

		// Sides
		indices.push_back(i * 2);
		indices.push_back(i * 2 + 1);
		indices.push_back(i * 2 + 2);
		indices.push_back(i * 2 + 3);

		// Close the cylinder
		if (i == segments - 1)
		{
			indices.push_back(0);
			indices.push_back(2 * segments);
			indices.push_back(1);
			indices.push_back(2 * segments + 1);
		}
	}

	// Create the VAO, VBO and EBO
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);

	// Unbind the VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// Unbind the VAO
	glBindVertexArray(0);

	// Check for errors
	glLog();
}

void CollisionBox::draw(const Shader &shader) const
{
	// We'll draw the collision box as a wireframe cylinder
	shader.use();
	shader.setMat4("model", getTransformMatrix());

	glBindVertexArray(VAO);

	glDrawElements(GL_LINES, indices.size(), GL_UNSIGNED_INT, indices.data());

	// Unbind the VAO
	glBindVertexArray(0);

	// Check for errors
	glLog();
}

std::vector<glm::vec2> CollisionBox::get2DProjectedVertices() const
{
	const auto matrix = getTransformMatrix();
	std::vector<glm::vec2> projectedVertices;
	for (auto i = 0; i < vertices.size(); i += 3)
		projectedVertices.emplace_back(vertices[i], vertices[i + 2]);

	for (auto &vertex : projectedVertices)
	{
		glm::vec4 vec{vertex.x, 0, vertex.y, 1};
		vec = matrix * vec;
		vertex = {vec.x, vec.z};
	}
	return projectedVertices;
}

glm::mat4 CollisionBox::getTransformMatrix() const
{
	glm::mat4 matrix{1};
	matrix = glm::translate(matrix, *pos);
	matrix = glm::scale(matrix, glm::vec3(scale->x, scale->y, scale->z));
	matrix = glm::rotate(matrix, angle->x, glm::vec3{1, 0, 0});
	matrix = glm::rotate(matrix, angle->y, glm::vec3{0, 1, 0});
	matrix = glm::rotate(matrix, angle->z, glm::vec3{0, 0, 1});
	return matrix;
}

bool CollisionBox::linesIntersect(const glm::vec2 &p1, const glm::vec2 &p2, const glm::vec2 &q1, const glm::vec2 &q2,
								  glm::vec3 &intersection) const
{
	auto s1 = p2 - p1;
	auto s2 = q2 - q1;

	float s = (-s1.y * (p1.x - q1.x) + s1.x * (p1.y - q1.y)) / (-s2.x * s1.y + s1.x * s2.y);
	float t = (s2.x * (p1.y - q1.y) - s2.y * (p1.x - q1.x)) / (-s2.x * s1.y + s1.x * s2.y);

	if (s >= 0 && s <= 1 && t >= 0 && t <= 1)
	{
		intersection = {p1.x + (t * s1.x), 0, p1.y + (t * s1.y)};
		return true;
	}

	return false;
}

CollisionData CollisionBox::getCollisionForce(const CollisionBox &other) const
{
	const auto vertices1 = get2DProjectedVertices();
	const auto vertices2 = other.get2DProjectedVertices();

	for (size_t i = 0; i < vertices1.size(); i += 2)
	{
		const auto p1 = vertices1[i];
		const auto p2 = vertices1[(i + 2) % vertices1.size()];

		for (size_t j = 0; j < vertices2.size(); j += 2)
		{
			const auto q1 = vertices2[j];
			const auto q2 = vertices2[(j + 2) % vertices2.size()];

			glm::vec3 intersection;
			if (linesIntersect(p1, p2, q1, q2, intersection))
			{
				glm::vec3 normal = glm::normalize(glm::vec3{p2.y - p1.y, 0, p1.x - p2.x});
				return {glm::length(p2 - p1), normal, intersection};
			}
		}
	}

	return {0, {}, {}};
}
