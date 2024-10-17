#include "mesh.hpp"

#include <GL/glew.h>

#include "utils/logger.hpp"

Mesh::Mesh(const vector<Vertex> &vertices, const vector<unsigned int> &indices, const vector<Texture> &textures) :
	vertices(vertices), indices(indices), textures(textures)
{
	setupMesh();
}

void Mesh::draw(const Shader &shader) const
{
	const auto logger = Logger::getInstance();
	// bind appropriate textures
	unsigned int diffuseNr = 1;
	unsigned int specularNr = 1;
	unsigned int normalNr = 1;
	unsigned int heightNr = 1;
	for (unsigned int i = 0; i < textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i); // active proper texture unit before binding
		if (const auto error = glGetError() != GL_NO_ERROR)
		{
			logger->error("OpenGL error after glActiveTexture: (" + std::to_string(error) + ") - " +
						  reinterpret_cast<const char *>(glewGetErrorString(error)));
			throw std::runtime_error("OpenGL error after glActiveTexture: " + std::to_string(error));
		}

		// retrieve texture number (the N in diffuse_textureN)
		string number;
		string name = textures[i].getType();
		if (name == "texture_diffuse")
			number = std::to_string(diffuseNr++);
		else if (name == "texture_specular")
			number = std::to_string(specularNr++); // transfer unsigned int to string
		else if (name == "texture_normal")
			number = std::to_string(normalNr++); // transfer unsigned int to string
		else if (name == "texture_height")
			number = std::to_string(heightNr++); // transfer unsigned int to string

		// now set the sampler to the correct texture unit
		glUniform1i(glGetUniformLocation(shader.ID, (name + number).c_str()), static_cast<GLint>(i));
		if (const auto error = glGetError() != GL_NO_ERROR)
		{
			logger->error("OpenGL error after glUniform1i: (" + std::to_string(error) + ") - " +
						  reinterpret_cast<const char *>(glewGetErrorString(error)));

			throw std::runtime_error("OpenGL error after glActiveTexture: " + std::to_string(error));
		}

		// and finally bind the texture
		glBindTexture(GL_TEXTURE_2D, textures[i].getId());
		if (const auto error = glGetError() != GL_NO_ERROR)
		{
			logger->error("OpenGL error after glBindTexture: (" + std::to_string(error) + ") - " +
						  reinterpret_cast<const char *>(glewGetErrorString(error)));

			throw std::runtime_error("OpenGL error after glActiveTexture: " + std::to_string(error));
		}
	}

	// draw mesh
	glBindVertexArray(VAO);

	if (const auto error = glGetError() != GL_NO_ERROR)
	{
		logger->error("OpenGL error after glBindVertexArray: (" + std::to_string(error) + ") - " +
					  reinterpret_cast<const char *>(glewGetErrorString(error)));

		throw std::runtime_error("OpenGL error after glActiveTexture: " + std::to_string(error));
	}

	glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, nullptr);
	if (const auto error = glGetError() != GL_NO_ERROR)
	{
		logger->error("OpenGL error after glDrawElements: (" + std::to_string(error) + ") - " +
					  reinterpret_cast<const char *>(glewGetErrorString(error)));

		throw std::runtime_error("OpenGL error after glActiveTexture: " + std::to_string(error));
	}

	glBindVertexArray(0);
	if (const auto error = glGetError() != GL_NO_ERROR)
	{
		logger->error("OpenGL error after glBindVertexArray(0): (" + std::to_string(error) + ") - " +
					  reinterpret_cast<const char *>(glewGetErrorString(error)));

		throw std::runtime_error("OpenGL error after glActiveTexture: " + std::to_string(error));
	}

	// always good practice to set everything back to defaults once configured.
	glActiveTexture(GL_TEXTURE0);
	if (const auto error = glGetError() != GL_NO_ERROR)
	{
		logger->error("OpenGL error after glActiveTexture(GL_TEXTURE0): (" + std::to_string(error) + ") - " +
					  reinterpret_cast<const char *>(glewGetErrorString(error)));

		throw std::runtime_error("OpenGL error after glActiveTexture: " + std::to_string(error));
	}
}

void Mesh::setupMesh()
{
	const auto logger = Logger::getInstance();
	// create buffers/arrays
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	if (const auto error = glGetError() != GL_NO_ERROR)
	{
		logger->error("OpenGL error after glBindVertexArray: (" + std::to_string(error) + ") - " +
					  reinterpret_cast<const char *>(glewGetErrorString(error)));

		throw std::runtime_error("OpenGL error after glActiveTexture: " + std::to_string(error));
	}

	// load data into vertex buffers
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	if (const auto error = glGetError() != GL_NO_ERROR)
	{
		logger->error("OpenGL error after glBindBuffer(GL_ARRAY_BUFFER): (" + std::to_string(error) + ") - " +
					  reinterpret_cast<const char *>(glewGetErrorString(error)));

		throw std::runtime_error("OpenGL error after glActiveTexture: " + std::to_string(error));
	}

	glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(vertices.size() * sizeof(Vertex)), &vertices[0],
				 GL_STATIC_DRAW);
	if (const auto error = glGetError() != GL_NO_ERROR)
	{
		logger->error("OpenGL error after glBufferData(GL_ARRAY_BUFFER): (" + std::to_string(error) + ") - " +
					  reinterpret_cast<const char *>(glewGetErrorString(error)));

		throw std::runtime_error("OpenGL error after glActiveTexture: " + std::to_string(error));
	}

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	if (const auto error = glGetError() != GL_NO_ERROR)
	{
		logger->error("OpenGL error after glBindBuffer(GL_ELEMENT_ARRAY_BUFFER): (" + std::to_string(error) + ") - " +
					  reinterpret_cast<const char *>(glewGetErrorString(error)));

		throw std::runtime_error("OpenGL error after glActiveTexture: " + std::to_string(error));
	}

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(indices.size() * sizeof(unsigned int)), &indices[0],
				 GL_STATIC_DRAW);
	if (const auto error = glGetError() != GL_NO_ERROR)
	{
		logger->error("OpenGL error after glBufferData(GL_ELEMENT_ARRAY_BUFFER): (" + std::to_string(error) + ") - " +
					  reinterpret_cast<const char *>(glewGetErrorString(error)));

		throw std::runtime_error("OpenGL error after glActiveTexture: " + std::to_string(error));
	}

	// set the vertex attribute pointers
	// vertex Positions
	glEnableVertexAttribArray(0);
	if (const auto error = glGetError() != GL_NO_ERROR)
	{
		logger->error("OpenGL error after glEnableVertexAttribArray(0): (" + std::to_string(error) + ") - " +
					  reinterpret_cast<const char *>(glewGetErrorString(error)));

		throw std::runtime_error("OpenGL error after glActiveTexture: " + std::to_string(error));
	}

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), static_cast<void *>(nullptr));
	if (const auto error = glGetError() != GL_NO_ERROR)
	{
		logger->error("OpenGL error after glVertexAttribPointer(0): (" + std::to_string(error) + ") - " +
					  reinterpret_cast<const char *>(glewGetErrorString(error)));

		throw std::runtime_error("OpenGL error after glActiveTexture: " + std::to_string(error));
	}

	// vertex normals
	glEnableVertexAttribArray(1);
	if (const auto error = glGetError() != GL_NO_ERROR)
	{
		logger->error("OpenGL error after glEnableVertexAttribArray(1): (" + std::to_string(error) + ") - " +
					  reinterpret_cast<const char *>(glewGetErrorString(error)));

		throw std::runtime_error("OpenGL error after glActiveTexture: " + std::to_string(error));
	}

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void *>(offsetof(Vertex, Normal)));
	if (const auto error = glGetError() != GL_NO_ERROR)
	{
		logger->error("OpenGL error after glVertexAttribPointer(1): (" + std::to_string(error) + ") - " +
					  reinterpret_cast<const char *>(glewGetErrorString(error)));

		throw std::runtime_error("OpenGL error after glActiveTexture: " + std::to_string(error));
	}

	// vertex texture coords
	glEnableVertexAttribArray(2);
	if (const auto error = glGetError() != GL_NO_ERROR)
	{
		logger->error("OpenGL error after glEnableVertexAttribArray(2): (" + std::to_string(error) + ") - " +
					  reinterpret_cast<const char *>(glewGetErrorString(error)));

		throw std::runtime_error("OpenGL error after glActiveTexture: " + std::to_string(error));
	}

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
						  reinterpret_cast<void *>(offsetof(Vertex, TexCoords)));
	if (const auto error = glGetError() != GL_NO_ERROR)
	{
		logger->error("OpenGL error after glVertexAttribPointer(2): (" + std::to_string(error) + ") - " +
					  reinterpret_cast<const char *>(glewGetErrorString(error)));

		throw std::runtime_error("OpenGL error after glActiveTexture: " + std::to_string(error));
	}

	// vertex tangent
	glEnableVertexAttribArray(3);
	if (const auto error = glGetError() != GL_NO_ERROR)
	{
		logger->error("OpenGL error after glEnableVertexAttribArray(3): (" + std::to_string(error) + ") - " +
					  reinterpret_cast<const char *>(glewGetErrorString(error)));

		throw std::runtime_error("OpenGL error after glActiveTexture: " + std::to_string(error));
	}

	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
						  reinterpret_cast<void *>(offsetof(Vertex, Tangent)));
	if (const auto error = glGetError() != GL_NO_ERROR)
	{
		logger->error("OpenGL error after glVertexAttribPointer(3): (" + std::to_string(error) + ") - " +
					  reinterpret_cast<const char *>(glewGetErrorString(error)));

		throw std::runtime_error("OpenGL error after glActiveTexture: " + std::to_string(error));
	}

	// vertex bitangent
	glEnableVertexAttribArray(4);
	if (const auto error = glGetError() != GL_NO_ERROR)
	{
		logger->error("OpenGL error after glEnableVertexAttribArray(4): (" + std::to_string(error) + ") - " +
					  reinterpret_cast<const char *>(glewGetErrorString(error)));

		throw std::runtime_error("OpenGL error after glActiveTexture: " + std::to_string(error));
	}

	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
						  reinterpret_cast<void *>(offsetof(Vertex, BiTangent)));
	if (const auto error = glGetError() != GL_NO_ERROR)
	{
		logger->error("OpenGL error after glVertexAttribPointer(4): (" + std::to_string(error) + ") - " +
					  reinterpret_cast<const char *>(glewGetErrorString(error)));

		throw std::runtime_error("OpenGL error after glActiveTexture: " + std::to_string(error));
	}

	// ids
	glEnableVertexAttribArray(5);
	if (const auto error = glGetError() != GL_NO_ERROR)
	{
		logger->error("OpenGL error after glEnableVertexAttribArray(5): (" + std::to_string(error) + ") - " +
					  reinterpret_cast<const char *>(glewGetErrorString(error)));

		throw std::runtime_error("OpenGL error after glActiveTexture: " + std::to_string(error));
	}

	glVertexAttribIPointer(5, 4, GL_INT, sizeof(Vertex), reinterpret_cast<void *>(offsetof(Vertex, m_BoneIDs)));
	if (const auto error = glGetError() != GL_NO_ERROR)
	{
		logger->error("OpenGL error after glVertexAttribIPointer(5): (" + std::to_string(error) + ") - " +
					  reinterpret_cast<const char *>(glewGetErrorString(error)));

		throw std::runtime_error("OpenGL error after glActiveTexture: " + std::to_string(error));
	}

	// weights
	glEnableVertexAttribArray(6);
	if (const auto error = glGetError() != GL_NO_ERROR)
	{
		logger->error("OpenGL error after glEnableVertexAttribArray(6): (" + std::to_string(error) + ") - " +
					  reinterpret_cast<const char *>(glewGetErrorString(error)));

		throw std::runtime_error("OpenGL error after glActiveTexture: " + std::to_string(error));
	}

	glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex),
						  reinterpret_cast<void *>(offsetof(Vertex, m_Weights)));
	if (const auto error = glGetError() != GL_NO_ERROR)
	{
		logger->error("OpenGL error after glVertexAttribPointer(6): (" + std::to_string(error) + ") - " +
					  reinterpret_cast<const char *>(glewGetErrorString(error)));

		throw std::runtime_error("OpenGL error after glActiveTexture: " + std::to_string(error));
	}

	glBindVertexArray(0);
	if (const auto error = glGetError() != GL_NO_ERROR)
	{
		logger->error("OpenGL error after glBindVertexArray(0): (" + std::to_string(error) + ") - " +
					  reinterpret_cast<const char *>(glewGetErrorString(error)));

		throw std::runtime_error("OpenGL error after glActiveTexture: " + std::to_string(error));
	}
}
