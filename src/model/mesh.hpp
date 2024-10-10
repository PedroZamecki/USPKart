#ifndef MESH_HPP
#define MESH_HPP

#include <vector>

#include "helper.hpp"
#include "resource/resourceManager.hpp"
#include "shader/shader.hpp"

using namespace std;

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

	void draw(const Shader &shader) const;
};


#endif // MESH_HPP
