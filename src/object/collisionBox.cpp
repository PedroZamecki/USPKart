#include "collisionBox.hpp"

#include <GL/glew.h>
#include <glm/ext/matrix_transform.hpp>

#include "shader/shader.hpp"
#include "utils/logger.hpp"
#include "utils/position.hpp"

CollisionBox::CollisionBox(Position *pos, float *yaw, float *pitch, float *angle, const float width, const float height,
						   const float depth) :
	pos(pos), yaw(yaw), pitch(pitch), angle(angle), width(width), height(height), depth(depth)
	{
		vertices = {
		// Front face
		-width / 2, -height / 2, depth / 2,
		width / 2, -height / 2, depth / 2,
		width / 2, height / 2, depth / 2,
		-width / 2, height / 2, depth / 2,
		// Back face
		-width / 2, -height / 2, -depth / 2,
		width / 2, -height / 2, -depth / 2,
		width / 2, height / 2, -depth / 2,
		-width / 2, height / 2, -depth / 2,
		// Left face
		-width / 2, -height / 2, -depth / 2,
		-width / 2, -height / 2, depth / 2,
		-width / 2, height / 2, depth / 2,
		-width / 2, height / 2, -depth / 2,
		// Right face
		width / 2, -height / 2, -depth / 2,
		width / 2, -height / 2, depth / 2,
		width / 2, height / 2, depth / 2,
		width / 2, height / 2, -depth / 2,
		// Top face
		-width / 2, height / 2, -depth / 2,
		width / 2, height / 2, -depth / 2,
		width / 2, height / 2, depth / 2,
		-width / 2, height / 2, depth / 2,
		// Bottom face
		-width / 2, -height / 2, -depth / 2,
		width / 2, -height / 2, -depth / 2,
		width / 2, -height / 2, depth / 2,
		-width / 2, -height / 2, depth / 2
	};

	const unsigned int indices[] = {
		0, 1, 1, 2, 2, 3, 3, 0,
		4, 5, 5, 6, 6, 7, 7, 4,
		8, 9, 9, 10, 10, 11, 11, 8,
		12, 13, 13, 14, 14, 15, 15, 12,
		16, 17, 17, 18, 18, 19, 19, 16,
		20, 21, 21, 22, 22, 23, 23, 20
	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glLog();

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(vertices.size() * sizeof(float)), vertices.data(), GL_STATIC_DRAW);

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

	glUseProgram(0);

	glLog();
}
