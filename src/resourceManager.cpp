#include <resourceManager.hpp>

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

GLuint loadTexture(const char *filePath)
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
                 GL_RGB, 1184, 1184, 0,
                 GL_RGB, GL_UNSIGNED_BYTE,
                 image->pixels);
    SDL_FreeSurface(image);

    return texture;
}