#include "model.hpp"
#include <GL/glew.h>
#include <fstream>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <resource/resourceManager.hpp>
#include <sstream>
#include <stdexcept>

Model::Model(const char *filePath, ResourceManager *rm)
{
	loadModel(filePath, rm);
	setupMesh();
}

Model::~Model()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteTextures(1, &texture);
}

void Model::loadModel(const char *fileDir, ResourceManager *rm)
{
	std::ifstream file(std::string(fileDir) + "model.obj");
	if (!file.is_open())
	{
		throw std::runtime_error("Failed to open OBJ file.");
	}

	std::string line;
	std::string mtlFilePath;
	while (std::getline(file, line))
	{
		std::istringstream iss(line);
		std::string prefix;
		iss >> prefix;

		if (prefix == "v")
		{
			glm::vec3 vertex;
			iss >> vertex.x >> vertex.y >> vertex.z;
			vertices.push_back(vertex);
		}
		else if (prefix == "vt")
		{
			glm::vec2 texCoord;
			iss >> texCoord.x >> texCoord.y;
			texCoords.push_back(texCoord);
		}
		else if (prefix == "vn")
		{
			glm::vec3 normal;
			iss >> normal.x >> normal.y >> normal.z;
			normals.push_back(normal);
		}
		else if (prefix == "f")
		{
			unsigned int vertexIndex[4], texCoordIndex[4], normalIndex[4];
			char slash;
			for (int i = 0; i < 4; ++i)
			{
				iss >> vertexIndex[i] >> slash >> texCoordIndex[i] >> slash >> normalIndex[i];
				indices.push_back(vertexIndex[i] - 1);
			}
		}
		else if (prefix == "mtllib")
		{
			iss >> mtlFilePath;
		}
	}

	if (!mtlFilePath.empty())
	{
		loadMaterial(fileDir, mtlFilePath, rm);
	}
}

void Model::loadMaterial(const std::string &fileDir, const std::string &mtlFilePath, ResourceManager *rm)
{
	std::ifstream file(fileDir + mtlFilePath);
	if (!file.is_open())
	{
		throw std::runtime_error("Failed to open MTL file.");
	}

	std::string line;
	std::string textureFilePath;
	while (std::getline(file, line))
	{
		std::istringstream iss(line);
		std::string prefix;
		iss >> prefix;

		if (prefix == "map_Kd")
		{
			iss >> textureFilePath;
		}
	}

	if (!textureFilePath.empty())
	{
		texture = rm->loadTexture((fileDir + textureFilePath).c_str(), 0, 0, "model_texture");
	}
}

void Model::setupMesh()
{
	std::vector<float> vertexData;
	for (unsigned int index : indices)
	{
		const glm::vec3 &vertex = vertices[index];
		const glm::vec2 &texCoord = texCoords[index];
		const glm::vec3 &normal = normals[index];

		vertexData.insert(vertexData.end(),
						  {vertex.x, vertex.y, vertex.z, normal.x, normal.y, normal.z, texCoord.x, texCoord.y});
	}

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(float), vertexData.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), nullptr);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<void *>(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<void *>(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Model::draw(const Position *pos, float rotationAngle, unsigned int shaderProgram) const
{
	glUseProgram(shaderProgram);

	// Set up model matrix
	glm::mat4 modelMatrix = glm::mat4(1.0f);
	modelMatrix = glm::translate(modelMatrix, pos->toVec3());
	modelMatrix = glm::rotate(modelMatrix, glm::radians(rotationAngle), glm::vec3(0.0f, 1.0f, 0.0f));
	modelMatrix = glm::scale(modelMatrix, glm::vec3(1.0f, 1.0f, 1.0f));

	// Set up normal matrix
	glm::mat4 normalMatrix = glm::transpose(glm::inverse(modelMatrix));

	// Pass matrices to the shader
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "normal"), 1, GL_FALSE, glm::value_ptr(normalMatrix));

	// Bind the texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glUniform1i(glGetUniformLocation(shaderProgram, "ourTexture"), 0);

	// Bind the vertex array object
	glBindVertexArray(VAO);

	// Enable the vertex attribute arrays
	glEnableVertexAttribArray(0); // Position
	glEnableVertexAttribArray(1); // Color
	glEnableVertexAttribArray(2); // Texture coordinates
	glEnableVertexAttribArray(3); // Normals

	// Draw the elements
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, indices.data());

	// Disable the vertex attribute arrays
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(3);

	// Unbind the vertex array object
	glBindVertexArray(0);

	// Check for OpenGL errors
	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		std::cerr << "OpenGL Error: " << error << std::endl;
	}
}
