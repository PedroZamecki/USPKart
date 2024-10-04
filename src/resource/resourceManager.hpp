#pragma once

#ifndef RESOURCE_MANAGER_HPP
#define RESOURCE_MANAGER_HPP

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <fstream>
#include <map>
#include <mutex>

class Texture
{
	unsigned int id{};
	std::string type;
	std::string path;

public:
	Texture(const unsigned int id, std::string type, std::string path) :
		id(id), type(std::move(type)), path(std::move(path))
	{
	}
	[[nodiscard]] unsigned int getId() const { return id; }
	[[nodiscard]] const std::string &getType() const { return type; }
	[[nodiscard]] const std::string &getPath() const { return path; }
};

class ResourceManager
{
	std::map<std::string, const Texture *> textures;
	std::map<std::string, const char *> audio;
	std::map<std::string, const aiScene *> scenes;
	std::map<std::string, const void *> icons;

	Assimp::Importer importer;
	ResourceManager();
	~ResourceManager();

	static ResourceManager *instance;
	static std::mutex mtx;

public:
	ResourceManager(const ResourceManager &obj) = delete;
	static ResourceManager *getInstance()
	{
		if (instance == nullptr)
		{
			std::lock_guard lock(mtx);
			if (instance == nullptr)
			{
				instance = new ResourceManager();
			}
		}
		return instance;
	}

	const Texture *loadTexture(const std::string &filePath, const std::string &type = "texture_diffuse");
	const char *loadAudio(std::string &filePath);
	const void *loadIcon(const std::string &filePath);
	const aiScene *loadScene(const std::string &filePath);
};

#endif
