#include "resourceManager.hpp"
#include <GL/glew.h>

#include <GLFW/glfw3.h>
#include <iostream>

#include <SOIL2/SOIL2.h>

ResourceManager::ResourceManager() = default;

ResourceManager::~ResourceManager()
{
	for (const auto &[key, texture] : textures)
		glDeleteTextures(1, &texture);
	textures.clear();
}

GLuint ResourceManager::loadTexture(const char *filePath, int height, int width, const std::string &name)
{
	GLuint texture = 0;

	glGenTextures(1, &texture);

	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	const unsigned char *image = SOIL_load_image(filePath, &width, &height, nullptr, SOIL_LOAD_RGBA);
	if (image == nullptr)
	{
		std::cerr << "Error: Texture \"" << name << "\" not found." << std::endl;
		return 0;
	}

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(const_cast<unsigned char *>(image));
	glBindTexture(GL_TEXTURE_2D, 0);

	textures[name] = texture;
	return texture;
}

GLuint ResourceManager::getTexture(const std::string &name) const
{
	if (!textures.count(name))
	{
		std::cerr << "Error: Texture \"" << name << "\" not found." << std::endl;
		return 0;
	}
	return textures.at(name);
}

char *ResourceManager::loadAudio(const char *filePath) { return const_cast<char *>(filePath); }

char *ResourceManager::getAudio(char *name) { return name; }

void *ResourceManager::loadIcon(const char *filePath)
{
	const auto icon = new GLFWimage;
	int width, height;
	width = height = 512;
	icon->pixels = SOIL_load_image(filePath, &width, &height, nullptr, SOIL_LOAD_RGBA);
	icon->width = width;
	icon->height = height;
	return icon;
}
