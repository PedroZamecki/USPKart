#include "resourceManager.hpp"
#include <SOIL2/SOIL2.h>
#include <iostream>

ResourceManager::~ResourceManager()
{
    for (auto & [fst, snd] : textures)
    {
        glDeleteTextures(1, &snd);
    }
    textures.clear();
}

GLuint ResourceManager::loadTexture(const char *filePath, int height, int width, const std::string& name)
{
    GLuint texture;

    glGenTextures(1, &texture);

    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MAG_FILTER,
                    GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MIN_FILTER,
                    GL_LINEAR);

    const unsigned char *image = SOIL_load_image(filePath, &width, &height, nullptr, SOIL_LOAD_RGB);
    if (image == nullptr)
    {
        std::cerr << "Error: Texture \"" << name << "\" not found." << std::endl;
        return 0;
    }

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    SOIL_free_image_data(const_cast<unsigned char *>(image));
    glBindTexture(GL_TEXTURE_2D, 0);

    textures[name] = texture;
    return texture;
}

GLuint ResourceManager::getTexture(const std::string& name) const
{
    if (textures.find(name) == textures.end())
    {
        std::cerr << "Error: Texture \"" << name << "\" not found." << std::endl;
        return 0;
    }
    return textures.at(name);
}

char* ResourceManager::loadAudio(const char* filePath)
{
    return const_cast<char*>(filePath);
}

char* ResourceManager::getAudio(char* name)
{
    return name;
}

