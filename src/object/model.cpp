#include "model.hpp"

#include <glad/glad.h>
#include <glm/ext/matrix_transform.hpp>
#include <glm/fwd.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <ostream>


#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define TINYGLTF_IMPLEMENTATION
#include <tiny_gltf.h>

Model::Model(const char *path, const GLuint texture) : texture(texture)
{
	// Read the model from the path using tinygltf
	tinygltf::Model model;
	tinygltf::TinyGLTF loader;
	std::string err;
	std::string warn;
	const bool ret = loader.LoadASCIIFromFile(&model, &err, &warn, path);
	if (!warn.empty())
	{
		std::cerr << "Warn: " << warn << std::endl;
	}
	if (!err.empty())
	{
		std::cerr << "Err: " << err << std::endl;
	}
	if (!ret)
	{
		std::cerr << "Failed to load glTF model" << std::endl;
	}

	// Extract the vertices and indices from the model
	std::vector<float> vertices;
	std::vector<unsigned int> indices;
	for (const auto &mesh : model.meshes)
	{
		for (const auto &primitive : mesh.primitives)
		{
			const auto accessor = model.accessors[primitive.attributes.at("POSITION")];
			const auto bufferView = model.bufferViews[accessor.bufferView];
			const auto buffer = model.buffers[bufferView.buffer];
			const auto data = buffer.data.data() + bufferView.byteOffset + accessor.byteOffset;
			const auto count = accessor.count;
			const auto bufferData = reinterpret_cast<const float *>(data);
			for (size_t i = 0; i < count; i++)
			{
				vertices.push_back(bufferData[i * 3]);
				vertices.push_back(bufferData[i * 3 + 1]);
				vertices.push_back(bufferData[i * 3 + 2]);
			}

			const auto indicesAccessor = model.accessors[primitive.indices];
			const auto indicesBufferView = model.bufferViews[indicesAccessor.bufferView];
			const auto indicesBuffer = model.buffers[indicesBufferView.buffer];
			const auto indicesData = indicesBuffer.data.data() + indicesBufferView.byteOffset + indicesAccessor.byteOffset;
			const auto indicesCount = indicesAccessor.count;
			const auto indicesBufferData = reinterpret_cast<const unsigned int *>(indicesData);
			for (size_t i = 0; i < indicesCount; i++)
			{
				indices.push_back(indicesBufferData[i]);
			}
		}
	}

	// Create the VAO, VBO, and EBO
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	indicesCount = indices.size();
}

Model::~Model()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}

void Model::draw(Position *pos, const float rotationAngle, const unsigned int shaderProgram) const
{
	glUseProgram(shaderProgram);
	glBindVertexArray(VAO);

	const glm::mat4 model = translate(glm::mat4(1.0f), pos->toVec3()) *
		rotate(glm::mat4(1.0f), glm::radians(rotationAngle), glm::vec3(0.0f, 0.0f, 1.0f));
	const unsigned int modelLoc = glGetUniformLocation(shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, value_ptr(model));

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}
