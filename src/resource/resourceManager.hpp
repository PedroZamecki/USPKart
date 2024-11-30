#pragma once

#ifndef RESOURCE_MANAGER_HPP
#define RESOURCE_MANAGER_HPP

#include <SOIL2/SOIL2.h>
#include <fstream>
#include <map>
#include <mutex>
#include <utility>

class Texture
{
	unsigned int id{};
	std::string type;
	std::string path;
	unsigned int width{};
	unsigned int height{};
	unsigned int channels{};
	unsigned char *data{};

public:
	Texture(const unsigned int id, std::string type, std::string path, const unsigned int width,
			const unsigned int height, const unsigned int channels, unsigned char *data) :
		id(id), type(std::move(type)), path(std::move(path)), width(width), height(height), data(data)
	{
	}
	[[nodiscard]] unsigned int getId() const { return id; }
	[[nodiscard]] const std::string &getType() const { return type; }
	[[nodiscard]] const std::string &getPath() const { return path; }
	[[nodiscard]] unsigned int getWidth() const { return width; }
	[[nodiscard]] unsigned int getHeight() const { return height; }
	[[nodiscard]] unsigned char *getData() const { return data; }
	[[nodiscard]] bool hasAlphaChannel() const { return channels == 4; };
};

class Model;

class ResourceManager
{
	std::map<std::string, const Texture> textures;
	std::map<std::string, const Model *> models;
	std::map<std::string, const char *> audio;
	std::map<std::string, const void *> icons;

	std::mutex texturesMutex;
	std::mutex modelsMutex;
	std::mutex audioMutex;
	std::mutex iconsMutex;

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

	Texture loadTexture(const std::string &filePath, const std::string &type = "texture_diffuse");
	const char *loadAudio(std::string &filePath);
	const void *loadIcon(const std::string &filePath);
	const Model *loadModel(const std::string &filePath);
};

#endif
