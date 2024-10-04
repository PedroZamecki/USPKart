#pragma once

#ifndef GRAPHICS_HELPER_HPP
#define GRAPHICS_HELPER_HPP

#include <GLFW/glfw3.h>

class GraphicsHelper
{
	unsigned int shaderProgram{};

public:
	~GraphicsHelper();

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
	static void *createWindow(const char *title, const GLFWimage *icon);

	/*
	 * Set up the OpenGL context.
	 */
	static void setupOpenGL(int width, int height);
};

#endif
