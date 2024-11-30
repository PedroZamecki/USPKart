#ifndef MODEL_HPP
#define MODEL_HPP

#include <assimp/scene.h>
#include <string>
#include <vector>

#include <assimp/Importer.hpp>
#include "mesh.hpp"

class Model
{
protected:
	// loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes
	// std::vector.
	virtual void loadModel(string const &path);

	// processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this
	// process on its children nodes (if any).
	virtual void processNode(const aiNode *node, const aiScene *scene);

	virtual Mesh processMesh(aiMesh *mesh, const aiScene *scene);

	// checks all material textures of a given type and loads the textures if they're not loaded yet.
	// the required info is returned as a Texture struct.
	std::vector<Texture> loadMaterialTextures(const aiMaterial *mat, aiTextureType type, const string &typeName);

	// model data
	std::vector<Texture> texturesLoaded; // stores all the textures loaded so far, optimization to make sure
	// textures aren't loaded more than once.
	std::vector<Mesh> meshes;
	string directory;
	Assimp::Importer importer;

public:
	// constructor, expects a filepath to a 3D model.
	explicit Model(string const &path);
	virtual ~Model()
	{
		texturesLoaded.clear();
		meshes.clear();
	}

	// draws the model, and thus all its meshes
	virtual void draw(const Shader &shader, const glm::vec3 &maskedColor = {0, 0, 0}, const glm::vec3 &maskColor = {0, 0, 0}) const;
};

#endif // MODEL_HPP
