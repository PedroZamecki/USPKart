#ifndef ANIMATED_MODEL_HPP
#define ANIMATED_MODEL_HPP

#include "model.hpp"
#include "utils/position.hpp"

class AnimatedModel : public Model
{
	std::map<std::string, BoneInfo> m_BoneInfoMap;
	int m_BoneCounter = 0;

	// processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this
	// process on its children nodes (if any).
	void processNode(aiNode *node, const aiScene *scene)
	{
		// process each mesh located at the current node
		for (unsigned int i = 0; i < node->mNumMeshes; i++)
		{
			// the node object only contains indices to index the actual objects in the scene.
			// the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
			aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
			meshes.push_back(processMesh(mesh, scene));
		}
		// after we've processed all the meshes (if any) we then recursively process each of the children nodes
		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			processNode(node->mChildren[i], scene);
		}
	}

	void SetVertexBoneDataToDefault(Vertex &vertex)
	{
		for (int i = 0; i < MAX_BONE_INFLUENCE; i++)
		{
			vertex.m_BoneIDs[i] = -1;
			vertex.m_Weights[i] = 0.0f;
		}
	}

	Mesh processMesh(aiMesh *mesh, const aiScene *scene);

	void extractBoneWeightForVertices(std::vector<Vertex> &vertices, const aiMesh *mesh, const aiScene *scene);

public:
	explicit AnimatedModel(string const &path, const bool gamma = false) : Model(path, gamma) {}
};

#endif // ANIMATED_MODEL_HPP
