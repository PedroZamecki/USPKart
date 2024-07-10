#pragma once

#include <glad/glad.h>
#include <GL/glu.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

#include <iostream>
#include <cstdlib>
#include <string>

#include "config.hpp"


class GraphicsHelper {
    public:
        /*
        * Configure the environment for the game.
        */
        void configureEnvironment();

        /*
        * Create a window with the specified title, width, and height.
        * The window can be resizable, full screen, and borderless.
        * Returns a pointer to the window created.
        * @param title The title of the window.
        * @param width The width of the window.
        * @param height The height of the window.
        * @param resizable If the window can be resized.
        * @param fullScreen If the window is full screen.
        * @param borderless If the window is borderless.
        * @return A pointer to the window created.
        */
        SDL_Window* createWindow(const char* title, Configuration *config);

        void manageWindow(SDL_Window* window);

    private:
        /*
        * Set up the OpenGL context.
        */
        void setupOpenGL();

        /*
        * Display the frames per second on the screen.
        * @param renderer The renderer to display the FPS.
        */
        void displayFPS(SDL_Renderer* renderer);

        /*
        * Calculate the frames per second.
        */
        void calculateFPS();

        /*
        * Load a texture from the specified file.
        * @param texture The texture to load.
        * @param file The file to load the texture from.
        */
        static SDL_bool manageInputs(SDL_Window* window, SDL_Event event);

        /*
        * Generate a shader program with the specified vertex and fragment shaders.
        * @param programID The ID of the shader program.
        * @param vertexShader The vertex shader.
        * @param fragmentShader The fragment shader.
        */
        void generateShaders(GLuint &programID, const char* vertexShader, const char* fragmentShader);
};