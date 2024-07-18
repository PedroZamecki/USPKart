#ifndef GRAPHICS_HELPER_HPP
#define GRAPHICS_HELPER_HPP

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

#include <config.hpp>
#include <utils.hpp>

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
     * @param config The configuration of the window.
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
    unsigned int frameCount = 0, fps = 0;

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

#endif