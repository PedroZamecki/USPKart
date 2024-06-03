#pragma once

#include <gl/glew.h>
#include <GLFW/glfw3.h>
#include <SOIL2/SOIL2.h>

#include <iostream>
#include <string>

#include "config.hpp"

/*
* Set up the OpenGL context.
*/
void setupOpenGL();

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
GLFWwindow* createWindow(const char* title, Configuration *config);
void manageWindow(GLFWwindow* window);

/*
* Generate a shader program with the specified vertex and fragment shaders.
* @param programID The ID of the shader program.
* @param vertexShader The vertex shader.
* @param fragmentShader The fragment shader.
*/
void generateShaders(GLuint &programID, const char* vertexShader, const char* fragmentShader);

static void manageInputs(GLFWwindow* window);