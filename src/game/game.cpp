#include "game.hpp"

#include <string>

#include "config.hpp"
#include "controls/controlsHandler.hpp"
#include "graphic/utils/model.hpp"

#define WINDOW_TITLE std::string("USPKart v") + USP_KART_VERSION
#define getPointer static_cast<Game *>(glfwGetWindowUserPointer(window))

Game::Game() : graphicsHelper(new GraphicsHelper), data(new Data), cam(new Camera)
{
	const auto ch = ControlsHandler::getInstance();
	const auto icon = ResourceManager::getInstance()->loadIcon("assets/icon.png");
	window = static_cast<GLFWwindow *>(
		GraphicsHelper::createWindow((WINDOW_TITLE).c_str(), static_cast<const GLFWimage *>(icon)));
	glfwSetWindowUserPointer(window, this);
	glfwSetKeyCallback(window, [](GLFWwindow *window, const int key, int, const int action, const int mods)
					   { ControlsHandler::getInstance()->executeKeyCallback(key, action, mods); });
	glfwSetWindowSizeCallback(window,
							  [](GLFWwindow *window, const int width, const int height)
							  {
								  if (width == 0 || height == 0)
									  return;
								  const auto config = Configuration::getInstance();
								  if (const auto game = getPointer; !config->fullScreen)
								  {
									  config->width = width;
									  config->height = height;
									  config->writeConfigurationFile();
									  game->cam->setAspectRatio(width, height);
								  }
								  GraphicsHelper::setupOpenGL(width, height);
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
								  cam->setAspectRatio(config->width, config->height);
								  config->fullScreen = false;
							  }
							  else
							  {
								  const auto monitor = glfwGetPrimaryMonitor();
								  const auto mode = glfwGetVideoMode(monitor);
								  cam->setAspectRatio(mode->width, mode->height);
								  glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height,
													   mode->refreshRate);
								  config->fullScreen = true;
							  }
							  config->writeConfigurationFile();
						  });
	data->objects.push_front(
		new Object(Position{0, 0, 0}, "assets/models/model.fbx", .01, .01, .01, new CollisionBox()));
	data->objects.push_front(
		new Object(Position{0, 2, 0}, "assets/models/model.fbx", .01, .01, .01, new CollisionBox()));
}

Game::~Game()
{
	delete graphicsHelper;
	delete cam;
}

void Game::run() const
{
	GraphicsHelper::configureEnvironment();
	const auto config = Configuration::getInstance();
	cam->setAspectRatio(config->width, config->height);

	// Set up the shaders
	const auto shader = new Shader("shaders/shader.vs", "shaders/shader.fs");
	double delta = 0.0;
	double fps = 0.0;

	int i = 0;
	while (!glfwWindowShouldClose(window))
	{
		const auto start = glfwGetTime();
		glfwPollEvents();

		// Clear the screen
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader->use();

		// Draw the objects
		for (const auto &object : data->objects)
		{
			object->draw(*shader, delta);
		}

		// Swap buffers
		glfwSwapBuffers(window);

		const auto end = glfwGetTime();
		delta = end - start;
		fps = 1.0 / delta;

		glfwSetWindowTitle(window, (WINDOW_TITLE + " (FPS: " + std::to_string(fps) + ")").c_str());
	}
	config->writeConfigurationFile();
}

int main()
{
	const auto game = new Game();
	game->run();
	delete game;
}
