#ifndef MODEL_HPP
#define MODEL_HPP

#include <assimp/scene.h>
#include <string>
#include <vector>

#include "mesh.hpp"
#include "resource/resourceManager.hpp"

class Model
{
protected:
	// loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes
	// std::vector.
	void loadModel(string const &path);

	// processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this
	// process on its children nodes (if any).
	void processNode(const aiNode *node, const aiScene *scene);

	Mesh processMesh(aiMesh *mesh, const aiScene *scene);

	// checks all material textures of a given type and loads the textures if they're not loaded yet.
	// the required info is returned as a Texture struct.
	std::vector<Texture> loadMaterialTextures(const aiMaterial *mat, aiTextureType type, const string &typeName);

	// model data
	std::vector<const Texture *> texturesLoaded; // stores all the textures loaded so far, optimization to make sure
	// textures aren't loaded more than once.
	std::vector<Mesh> meshes;
	string directory;
	bool gammaCorrection;

public:
	// constructor, expects a filepath to a 3D model.
	explicit Model(string const &path, bool gamma = false);

	// draws the model, and thus all its meshes
	void draw(const Shader &shader) const;
};

#endif // MODEL_HPP
