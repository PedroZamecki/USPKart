#include <GL/glew.h>

#include <chrono>
#include <iostream>
#include <thread>
#include "gameWindow.hpp"

#include <utils/logger.hpp>
#include "controls/controlsHandler.hpp"
#include "game/config.hpp"
#include "game/data.hpp"
#include "utils/camera.hpp"

#define WINDOW_TITLE std::string("USPKart v") + USP_KART_VERSION

void rotateCamera(const float angle)
{
	const auto cam = Camera::getInstance();
	const auto relativePos = cam->pos - cam->target;
	const auto newAngle = atan2(relativePos.z(), relativePos.x()) + angle;
	const auto dist = sqrt(pow(relativePos.x(), 2) + pow(relativePos.z(), 2));
	cam->pos = cam->target + Position(cos(newAngle) * dist, cam->pos.y(), sin(newAngle) * dist);
}

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
	glewInit();

	logger->trace(std::string("Video information:\n") +
				  "OpenGL version: " + reinterpret_cast<const char *>(glGetString(GL_VERSION)) + "\n" +
				  "OpenGL renderer: " + reinterpret_cast<const char *>(glGetString(GL_RENDERER)) + "\n" +
				  "OpenGL vendor: " + reinterpret_cast<const char *>(glGetString(GL_VENDOR)));

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
	ch->insertKeyCallback(GLFW_KEY_D, GLFW_PRESS, 0, [this]() -> void { rotateCamera(0.0872665f); });
	ch->insertKeyCallback(GLFW_KEY_A, GLFW_PRESS, 0, [this]() -> void { rotateCamera(-0.0872665f); });
	ch->insertKeyCallback(GLFW_KEY_W, GLFW_PRESS, 0,
						  [this]() -> void
						  {
							  const auto cam = Camera::getInstance();
							  const auto direction = (cam->target - cam->pos).normalize();
							  cam->pos += direction * 2.0f; // Move closer to the target
						  });
	ch->insertKeyCallback(GLFW_KEY_S, GLFW_PRESS, 0,
						  [this]() -> void
						  {
							  const auto cam = Camera::getInstance();
							  const auto direction = (cam->pos - cam->target).normalize();
							  cam->pos += direction * 2.0f; // Move away from the target
						  });
}
GameWindow::~GameWindow()
{
	glfwTerminate();
	Configuration::getInstance()->writeConfigurationFile();
}

void GameWindow::run(const Data *data) const
{
	const auto logger = Logger::getInstance();
	const auto modelShader = Shader("assets/shaders/model.vs", "assets/shaders/model.fs");
	const auto animatedModelShader = Shader("assets/shaders/animatedModel.vs", "assets/shaders/animatedModel.fs");
	const auto camera = Camera::getInstance();
	auto lastTime = std::chrono::high_resolution_clock::now();
	unsigned long nbFrames = 0;
	while (!glfwWindowShouldClose(window))
	{
		const auto frameStartTime = std::chrono::high_resolution_clock::now();
		const auto delta = std::chrono::duration<float>(frameStartTime - lastTime).count();

		modelShader.use();
		modelShader.setMat4("projection", camera->getProjectionMatrix());
		modelShader.setMat4("view", camera->getViewMatrix());

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		for (const auto &object : data->objects)
		{
			object->draw(modelShader, delta);
		}

		if (const auto err = glGetError())
			logger->error("OpenGL error: " + std::to_string(err) + " at " + __FILE__ + ":" + std::to_string(__LINE__));

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
