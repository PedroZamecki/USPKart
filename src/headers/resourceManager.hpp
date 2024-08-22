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
    std::map<std::string, GLuint> textures;
public:
    ~ResourceManager();

    GLuint loadTexture(const char *filePath, int height, int width, const std::string& name);
    GLuint getTexture(const std::string& name) const;
};

#endif