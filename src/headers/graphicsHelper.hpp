#pragma once

#ifndef GRAPHICS_HELPER_HPP
#define GRAPHICS_HELPER_HPP

#include <glad/glad.h>
#include <GL/gl.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>

#include <config.hpp>
#include <utils.hpp>
#include <drawingHelper.hpp>
#include <resourceManager.hpp>

class GraphicsHelper
{
public:
    explicit GraphicsHelper(Data *data);
    ~GraphicsHelper();

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
    SDL_Window *createWindow(const char* title, Configuration* configuration);

    void manageWindow();

private:
    Data *data;

    Camera *cam = new Camera();
    SDL_GLContext glContext{};
    Configuration *config{};
    GLuint shaderProgram{};
    SDL_Window *window{};
    std::string session;
    ResourceManager *rm{};

    TTF_Font *font{};
    SDL_Color textColor = {255, 255, 255, 255}; // White

    Uint32 startTime = 0, currentTime = 0;
    float deltaTime = 0;
    unsigned int frameCount = 0, fps = 0;

    /*
     * Set up the OpenGL context.
     */
    void setupOpenGL() const;

    /*
     * Calculate the frames per second.
     */
    void calculateFPS();

    SDL_bool manageInputs(const SDL_Event& event) const;

    /*
     * Generate a shader program with the specified vertex and fragment shaders.
     * @param programID The ID of the shader program.
     * @param vertexShader The vertex shader.
     * @param fragmentShader The fragment shader.
     */
    static void generateShaders(GLuint &programID, const char *vertexShader, const char *fragmentShader);

    void stop() const;
};

#endif