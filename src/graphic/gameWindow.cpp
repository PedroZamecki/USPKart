#include <GL/glew.h>

#include <chrono>
#include <iostream>
#include <thread>
#include "gameWindow.hpp"

#include <utils/logger.hpp>
#include "camera/camera.hpp"
#include "controls/controlsHandler.hpp"
#include "game/config.hpp"
#include "game/data.hpp"
#include "object/character/player.hpp"
#include "skybox.hpp"

#define WINDOW_TITLE std::string("USPKart v") + USP_KART_VERSION

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
	if (const auto err = glGetError())
	{
		Logger::getInstance()->error("OpenGL error: (" + std::to_string(err) + ") - " +
									 reinterpret_cast<const char *>(glewGetErrorString(err)));
		throw std::runtime_error("OpenGL error");
	}
}

GameWindow::GameWindow(const std::string &title, const GLFWimage *icon)
{
	const auto c = Configuration::getInstance();
	const auto logger = Logger::getInstance();

	if (!glfwInit())
	{
		logger->error("Failed to initialize GLFW");
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
		logger->error("Failed to create GLFW window");
		glfwTerminate();
		throw std::runtime_error("Failed to create GLFW window");
	}

	glfwSetWindowIcon(window, 1, icon);

	glfwSetWindowPos(window, c->posX, c->posY);
	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		Logger::getInstance()->error("Failed to initialize GLEW");
		throw std::runtime_error("Failed to initialize GLEW");
	}

	logger->debug(std::string("Video information:\n") +
				  "\tOpenGL version: " + reinterpret_cast<const char *>(glGetString(GL_VERSION)) + "\n" +
				  "\tOpenGL renderer: " + reinterpret_cast<const char *>(glGetString(GL_RENDERER)) + "\n" +
				  "\tOpenGL vendor: " + reinterpret_cast<const char *>(glGetString(GL_VENDOR)));

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

	ch->insertKeyCallback(GLFW_KEY_ESCAPE, [this]() -> void { glfwSetWindowShouldClose(window, GLFW_TRUE); });

	ch->insertKeyCallback(GLFW_KEY_F,
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
								  config->fullScreen = false;
							  }
							  else
							  {
								  const auto monitor = glfwGetPrimaryMonitor();
								  const auto mode = glfwGetVideoMode(monitor);
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

void GameWindow::run(const Data *data) const
{
	const auto modelShader = Shader("assets/shaders/model.vs", "assets/shaders/model.fs");
	const auto boxShader = Shader("assets/shaders/box.vs", "assets/shaders/box.fs");
	const auto skybox = Skybox({"assets/textures/skybox/right.jpg", "assets/textures/skybox/left.jpg",
								"assets/textures/skybox/top.jpg", "assets/textures/skybox/bottom.jpg",
								"assets/textures/skybox/front.jpg", "assets/textures/skybox/back.jpg"});
	const auto camera = Camera::getInstance();
	camera->setState(new CameraBehindState);

	bool drawBoxes = false;
	ControlsHandler::getInstance()->insertKeyCallback(GLFW_KEY_B, [&drawBoxes]() -> void
	{
		Logger::getInstance()->info("Toggling boxes: " + std::string(((drawBoxes = !drawBoxes)) ? "on" : "off"));
	});

	auto lastTime = std::chrono::high_resolution_clock::now();
	unsigned long nbFrames = 0;
	while (!glfwWindowShouldClose(window))
	{
		const auto frameStartTime = std::chrono::high_resolution_clock::now();
		const auto delta = std::chrono::duration<float>(frameStartTime - lastTime).count();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		const auto projection = camera->getProjectionMatrix();
		const auto view = camera->getViewMatrix();

		modelShader.setMat4("projection", projection);
		modelShader.setMat4("view", view);
		boxShader.setMat4("projection", projection);
		boxShader.setMat4("view", view);

		for (const auto &object : data->objects)
		{
			object->draw(modelShader, delta, {1}, drawBoxes, boxShader);
			// If the object is the player, update the camera
			if (object->isPlayer())
			{
				camera->update(dynamic_cast<const Player *>(object));
			}
		}

		skybox.draw(view, projection);

		glfwSwapBuffers(window);
		glfwPollEvents();

		lastTime = frameStartTime;
		nbFrames++;
		// Only use the first 2 decimal places of the FPS
		// Detecting where the dot is in the string
		const auto fps = std::to_string(1 / delta);
		const auto fpsString = fps.substr(0, fps.find('.') + 3);
		glfwSetWindowTitle(window, (WINDOW_TITLE + " FPS: " + fpsString).c_str());
		std::this_thread::sleep_for(std::chrono::milliseconds(delta < 0.001 ? 10 : 0));
	}
}
