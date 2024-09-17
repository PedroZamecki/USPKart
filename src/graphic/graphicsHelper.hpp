#pragma once

#ifndef GRAPHICS_HELPER_HPP
#define GRAPHICS_HELPER_HPP

#include <game/config.hpp>

class GraphicsHelper
{
public:
	~GraphicsHelper();

	static unsigned int loadShaders();
	/*
	 * Configure the environment for the game.
	 */
	static void configureEnvironment();

	/*
	 * Create a window with the specified title, width, and height.
	 * The window can be resizable, full screen, and borderless.
	 * Returns a pointer to the window created.
	 * @param title The title of the window.
	 * @param config The configuration of the window.
	 * @return A pointer to the window created.
	 */
	static void *createWindow(const char *title, const Configuration *c, const void *icon);

	/*
	 * Generate a shader program with the specified vertex and fragment shaders.
	 * @param programID The ID of the shader program.
	 * @param vertexShader The vertex shader.
	 * @param fragmentShader The fragment shader.
	 */
	static void generateShaders(unsigned int &programID, const char *vertexShader, const char *fragmentShader);

	/*
	 * Set up the OpenGL context.
	 */
	static void setupOpenGL(int width, int height);

private:
	unsigned int shaderProgram{};
};

#endif
