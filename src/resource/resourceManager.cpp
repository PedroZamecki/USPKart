#include "resourceManager.hpp"
#include <glad/glad.h>

#include <GLFW/glfw3.h>
#include <iostream>
#include <stb_image.h>

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

	const unsigned char *image = stbi_load(filePath, &width, &height, nullptr, 4);
	if (image)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	}
	else
	{
		std::cerr << "Error: Texture \"" << name << "\" not found." << std::endl;
	}
	stbi_image_free(const_cast<unsigned char *>(image));

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
	icon->pixels = stbi_load(filePath, &width, &height, nullptr, 4);
	icon->width = width;
	icon->height = height;
	return icon;
}
