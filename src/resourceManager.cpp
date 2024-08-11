#include <resourceManager.hpp>

ResourceManager::~ResourceManager()
{
    for (auto & [fst, snd] : textures)
    {
        glDeleteTextures(1, &snd);
    }
    textures.clear();
}

SDL_Surface *loadImage(const char *filePath)
{
    SDL_Surface *loadedImage = nullptr;
    loadedImage = IMG_Load(filePath);

    if (loadedImage == nullptr)
    {
        std::cerr << "Error: " << IMG_GetError() << std::endl;
        return nullptr;
    }

    return loadedImage;
}

GLuint ResourceManager::loadTexture(const char *filePath, const int height, const int width, const std::string& name)
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

    SDL_Surface *image = loadImage(filePath);

    glTexImage2D(GL_TEXTURE_2D, 0,
                 GL_RGB, width, height, 0,
                 GL_RGB, GL_UNSIGNED_BYTE,
                 image->pixels);
    SDL_FreeSurface(image);

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
