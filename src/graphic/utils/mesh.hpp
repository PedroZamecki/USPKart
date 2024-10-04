#ifndef MESH_HPP
#define MESH_HPP

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

#include "resource/resourceManager.hpp"
#include "shader.hpp"

using namespace std;

#define MAX_BONE_INFLUENCE 4

struct Vertex
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
	glm::vec3 Tangent;
	glm::vec3 BiTangent;
	int m_BoneIDs[MAX_BONE_INFLUENCE];
	float m_Weights[MAX_BONE_INFLUENCE];
};

class Mesh
{
	// render data
	unsigned int VAO{}, VBO{}, EBO{};

	// initializes all the buffer objects/arrays
	void setupMesh();

	// mesh Data
	vector<Vertex> vertices;
	vector<unsigned int> indices;
	vector<Texture> textures;

public:
	Mesh(const vector<Vertex> &vertices, const vector<unsigned int> &indices, const vector<Texture> &textures);

	void Draw(const Shader &shader) const;
};


#endif // MESH_HPP