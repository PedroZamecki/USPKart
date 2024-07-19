#pragma once

#ifndef RESOURCE_MANAGER_HPP
#define RESOURCE_MANAGER_HPP

#include <glad/glad.h>
#include <SDL2/SDL_image.h>
#include <utils.hpp>
#include <iostream>

SDL_Surface *loadImage(const char *filePath);

GLuint loadTexture(const char *filePath);    

#endif