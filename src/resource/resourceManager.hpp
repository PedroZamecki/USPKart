#pragma once

#ifndef RESOURCE_MANAGER_HPP
#define RESOURCE_MANAGER_HPP

#include <fstream>
#include <map>

class ResourceManager
{
private:
	std::map<std::string, unsigned int> textures;
	std::map<std::string, char *> audio;

public:
	ResourceManager();
	~ResourceManager();

	unsigned int loadTexture(const char *filePath, int height, int width, const std::string &name);
	unsigned int getTexture(const std::string &name) const;
	static char *loadAudio(const char *filePath);
	static char *getAudio(char *name);
	static void *loadIcon(const char *filePath);
};

#endif
