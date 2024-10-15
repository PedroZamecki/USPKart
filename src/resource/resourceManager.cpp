#include "resourceManager.hpp"

#include <GLFW/glfw3.h>
#include <iostream>

#include <SOIL2/SOIL2.h>
#include <assimp/postprocess.h>
#include <utils/logger.hpp>

ResourceManager* ResourceManager::instance = nullptr;
std::mutex ResourceManager::mtx;

ResourceManager::ResourceManager() = default;

ResourceManager::~ResourceManager()
{
	textures.clear();
}

const Texture *ResourceManager::loadTexture(const std::string &filePath, const std::string &type)
{
	const auto logger = Logger::getInstance();
	// Search in the textures map
	for (const auto &[path, texture] : textures)
		if (path == filePath)
			return texture;

	GLuint textureId = 0;
	int width, height;

	glGenTextures(1, &textureId);

	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	const unsigned char *image = SOIL_load_image(filePath.c_str(), &width, &height, nullptr, SOIL_LOAD_RGBA);
	if (image == nullptr)
	{
		logger->error("Error: Texture in \"" + filePath + "\" not found.");
		return nullptr;
	}

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(const_cast<unsigned char *>(image));
	glBindTexture(GL_TEXTURE_2D, 0);

	return textures[filePath] = new Texture{textureId, type, filePath};
}

const char *ResourceManager::loadAudio(std::string &filePath)
{
	const auto logger = Logger::getInstance();
	// Search in the audio map
	for (const auto &[path, audio] : audio)
		if (path == filePath)
			return audio;

	std::ifstream file(filePath, std::ios::binary);
	if (!file.is_open())
	{
		logger->error("Error: Audio in \"" + filePath + "\" not found.");
		return nullptr;
	}

	file.seekg(0, std::ios::end);
	const auto size = file.tellg();
	file.seekg(0, std::ios::beg);

	auto *buffer = new char[size];
	file.read(buffer, size);
	file.close();

	audio[filePath] = buffer;
	return buffer;
}

const void *ResourceManager::loadIcon(const std::string &filePath)
{
	// Search in the icons map
	for (const auto &[path, icon] : icons)
		if (path == filePath)
			return icon;

	const auto icon = new GLFWimage;
	int width, height;
	width = height = 512;
	icon->pixels = SOIL_load_image(filePath.c_str(), &width, &height, nullptr, SOIL_LOAD_RGBA);
	icon->width = width;
	icon->height = height;
	return icon;
}

const aiScene *ResourceManager::loadScene(const std::string &filePath)
{
	const auto logger = Logger::getInstance();
	// Search in the scenes map
	for (const auto &[path, scene] : scenes)
		if (path == filePath)
			return scene;

	const auto scene = importer.ReadFile(
		filePath, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
	// check for errors
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
	{
		logger->error("ASSIMP:: " + std::string(importer.GetErrorString()));
		return nullptr;
	}
	scenes[filePath] = scene;
	return scene;
}
