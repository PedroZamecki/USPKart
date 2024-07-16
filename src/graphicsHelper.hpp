#pragma once

#include <glad/glad.h>
#include <GL/glu.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <cstdlib>
#include <string>

#include "config.hpp"

struct Position {
    float x;
    float y;
    float z;
};

class Camera {
public:
    Position target = {0.0f, 0.0f, 0.0f};
    float pitch = 0.0f;
    float yaw = 0.0f;
    float camTargetDist = 10.0f;
};

class GraphicsHelper
{
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
    SDL_Window *createWindow(const char *title, Configuration *config);

    void manageWindow();

private:
    Camera *cam = new Camera();
    SDL_GLContext glContext;
    Configuration *graphicsConfig;
    unsigned int shaderProgram;
    SDL_Renderer *renderer;
    SDL_Window *window;
    std::string session;

    TTF_Font *font;
    SDL_Color textColor = {255, 255, 255, 255}; // White

    Uint32 startTime = 0, currentTime = 0;
    float deltaTime = 0;
    int frameCount = 0, fps = 0;

    /*
     * Set up the OpenGL context.
     */
    void setupOpenGL();

    /*
     * Display the frames per second on the screen.
     * @param renderer The renderer to display the FPS.
     */
    void displayText(std::string message, SDL_Color color, int x, int y, int size);

    /*
     * Calculate the frames per second.
     */
    void calculateFPS();

    SDL_bool manageInputs(SDL_Event event);

    /*
     * Generate a shader program with the specified vertex and fragment shaders.
     * @param programID The ID of the shader program.
     * @param vertexShader The vertex shader.
     * @param fragmentShader The fragment shader.
     */
    void generateShaders(GLuint &programID, const char *vertexShader, const char *fragmentShader);

    void stop();
};