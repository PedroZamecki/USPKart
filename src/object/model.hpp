#ifndef MODEL_HPP
#define MODEL_HPP

#include <glm/glm.hpp>
#include <string>
#include <utils/position.hpp>
#include <vector>

#include "resource/resourceManager.hpp"

class Model
{
public:
	Model(const char *filePath, ResourceManager *rm);
	~Model();
	void draw(const Position *pos, float rotationAngle, unsigned int shaderProgram) const;

private:
	void loadModel(const char *fileDir, ResourceManager *rm);
	void loadMaterial(const std::string &fileDir, const std::string &mtlFilePath, ResourceManager *rm);
	void setupMesh();

	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> texCoords;
	std::vector<glm::vec3> normals;
	std::vector<unsigned int> indices;

	unsigned int VAO{}, VBO{}, EBO{};
	unsigned int texture{};
};

#endif // MODEL_HPP