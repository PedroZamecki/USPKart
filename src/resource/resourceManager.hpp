#pragma once

#ifndef RESOURCE_MANAGER_HPP
#define RESOURCE_MANAGER_HPP

#include <glad/glad.h>
#include <fstream>
#include <map>

class ResourceManager
{
private:
    std::map<std::string, GLuint> textures;
    std::map<std::string, char*> audio;

public:
    ResourceManager();
    ~ResourceManager();

    GLuint loadTexture(const char *filePath, int height, int width, const std::string& name);
    GLuint getTexture(const std::string& name) const;
    static char* loadAudio(const char *filePath);
    static char* getAudio(char* name);
};

#endif