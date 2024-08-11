#pragma once

#ifndef RESOURCE_MANAGER_HPP
#define RESOURCE_MANAGER_HPP

#include <glad/glad.h>
#include <SDL2/SDL_image.h>
#include <utils.hpp>
#include <iostream>
#include <map>

class ResourceManager
{
private:
    ~ResourceManager();
    std::map<std::string, GLuint> textures;
public:
    GLuint loadTexture(const char *filePath, const int height, const int width, const std::string& name);
    GLuint getTexture(const std::string& name) const;
};

#endif