#pragma once

#ifndef GRAPHICS_HELPER_HPP
#define GRAPHICS_HELPER_HPP

#include <glad/glad.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <string>

#include <game/config.hpp>

class GraphicsHelper
{
public:
    ~GraphicsHelper();

    static GLuint loadShaders();
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
    GLFWwindow *createWindow(const char* title,
                             const Configuration * configuration);

    /*
     * Generate a shader program with the specified vertex and fragment shaders.
     * @param programID The ID of the shader program.
     * @param vertexShader The vertex shader.
     * @param fragmentShader The fragment shader.
     */
    void generateShaders(GLuint &programID, const char *vertexShader, const char *fragmentShader);
    void stop() const;

private:
    GLuint shaderProgram{};
    GLFWwindow *window{};

    /*
     * Set up the OpenGL context.
     */
    void setupOpenGL() const;
};

#endif