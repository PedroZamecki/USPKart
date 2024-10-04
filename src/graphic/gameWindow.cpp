#include <GL/glew.h>

#include <iostream>
#include "gameWindow.hpp"

#include "controls/controlsHandler.hpp"
#include "game/config.hpp"
#include "utils/camera.hpp"

void GameWindow::setupOpenGL(const int width, const int height)
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDepthFunc(GL_LESS);
	glEnable(GL_DEPTH_TEST);

	/* Set the clear color. */
	glClearColor(1, 0.6, 0.8, 0);

	/* Setup our viewport. */
	glViewport(0, 0, width, height);
}

GameWindow::GameWindow(const std::string &title, const GLFWimage *icon)
{
	const auto c = Configuration::getInstance();

	if (!glfwInit())
	{
		std::cerr << "Failed to initialize GLFW" << std::endl;
		throw std::runtime_error("Failed to initialize GLFW");
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glfwWindowHint(GLFW_RESIZABLE, c->resizable);
	glfwWindowHint(GLFW_DECORATED, !c->borderless);

	if (c->fullScreen)
	{
		GLFWmonitor *primaryMonitor = glfwGetPrimaryMonitor();
		const GLFWvidmode *mode = glfwGetVideoMode(primaryMonitor);
		window = glfwCreateWindow(mode->width, mode->height, title.c_str(), primaryMonitor, nullptr);
	}

	else
	{
		window = glfwCreateWindow(c->width, c->height, title.c_str(), nullptr, nullptr);
	}

	if (!window)
	{
		std::cerr << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		throw std::runtime_error("Failed to create GLFW window");
	}

	glfwSetWindowIcon(window, 1, icon);

	glfwSetWindowPos(window, c->posX, c->posY);
	glfwMakeContextCurrent(window);
	glewInit();

	std::cout << "Video information:" << std::endl
			  << "OpenGL version: " << glGetString(GL_VERSION) << std::endl
			  << "OpenGL renderer: " << glGetString(GL_RENDERER) << std::endl
			  << "OpenGL vendor: " << glGetString(GL_VENDOR) << std::endl;

	// Set up OpenGL
	setupOpenGL(c->width, c->height);
	// Turn on VSync if requested
	glfwSwapInterval(c->vsync);

	glfwSetWindowUserPointer(window, this);
	glfwSetKeyCallback(window, [](GLFWwindow *window, const int key, int, const int action, const int mods)
					   { ControlsHandler::getInstance()->executeKeyCallback(key, action, mods); });
	glfwSetWindowSizeCallback(window,
							  [](GLFWwindow *window, const int width, const int height)
							  {
								  if (width == 0 || height == 0)
									  return;
								  if (const auto config = Configuration::getInstance(); !config->fullScreen)
								  {
									  config->width = width;
									  config->height = height;
									  Camera::getInstance()->setAspectRatio(width, height);
									  config->writeConfigurationFile();
								  }
								  setupOpenGL(width, height);
							  });
	glfwSetWindowPosCallback(window,
							 [](GLFWwindow *window, const int x, const int y)
							 {
								 if (x == 0 || y == 0 || x == -32000 || y == -32000)
									 return;
								 if (const auto config = Configuration::getInstance(); !config->fullScreen)
								 {
									 config->posX = x;
									 config->posY = y;
									 config->writeConfigurationFile();
								 }
							 });

	const auto ch = ControlsHandler::getInstance();

	ch->insertKeyCallback(GLFW_KEY_ESCAPE, GLFW_PRESS, 0,
						  [this]() -> void { glfwSetWindowShouldClose(window, GLFW_TRUE); });

	ch->insertKeyCallback(GLFW_KEY_F, GLFW_PRESS, 0,
						  [this]() -> void
						  {
							  // Set fullscreen
							  const auto config = Configuration::getInstance();
							  if (config->fullScreen)
							  {
								  glfwSetWindowMonitor(window, nullptr, config->posX, config->posY, config->width,
													   config->height, GLFW_DONT_CARE);
								  glfwSetWindowPos(window, config->posX, config->posY);
								  glfwSetWindowSize(window, config->width, config->height);
								  glfwSetWindowAttrib(window, GLFW_DECORATED, !config->borderless);
								  glfwSetWindowAttrib(window, GLFW_RESIZABLE, config->resizable);
								  Camera::getInstance()->setAspectRatio(config->width, config->height);
								  config->fullScreen = false;
							  }
							  else
							  {
								  const auto monitor = glfwGetPrimaryMonitor();
								  const auto mode = glfwGetVideoMode(monitor);
								  Camera::getInstance()->setAspectRatio(mode->width, mode->height);
								  glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height,
													   mode->refreshRate);
								  config->fullScreen = true;
							  }
							  config->writeConfigurationFile();
						  });
}
GameWindow::~GameWindow()
{
	glfwTerminate();
	Configuration::getInstance()->writeConfigurationFile();
}

void GameWindow::run() const
{
	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}