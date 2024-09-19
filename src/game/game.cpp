#include "game.hpp"

#include <graphic/drawingHelper.hpp>
#include <string>

#include "object/model.hpp"

#define WINDOW_TITLE std::string("USPKart v") + USP_KART_VERSION
#define getPointer static_cast<Game *>(glfwGetWindowUserPointer(window))

Game::Game() :
	config(new Configuration), graphicsHelper(new GraphicsHelper), data(new Data), cam(new Camera),
	rm(new ResourceManager), ch(new ControlsHandler())
{
	const auto icon = static_cast<GLFWimage *>(ResourceManager::loadIcon("assets/icon.png"));
	window = static_cast<GLFWwindow *>(GraphicsHelper::createWindow((WINDOW_TITLE).c_str(), config, icon));
	glfwSetWindowUserPointer(window, this);
	glfwSetKeyCallback(window,
					   [](GLFWwindow *window, const int key, int, const int action, const int mods)
					   {
						   const auto game = getPointer;
						   game->ch->executeKeyCallback(key, action, mods);
					   });
	glfwSetWindowSizeCallback(window,
							  [](GLFWwindow *window, const int width, const int height)
							  {
								  if (width == 0 || height == 0)
									  return;
								  if (const auto game = getPointer; !game->config->fullScreen)
								  {
									  game->config->width = width;
									  game->config->height = height;
									  game->cam->setAspectRatio(width, height);
								  }
								  GraphicsHelper::setupOpenGL(width, height);
							  });
	glfwSetWindowPosCallback(window,
							 [](GLFWwindow *window, const int x, const int y)
							 {
								 if (x == 0 || y == 0 || x == -32000 || y == -32000)
									 return;
								 if (const auto game = getPointer; !game->config->fullScreen)
								 {
									 game->config->posX = x;
									 game->config->posY = y;
								 }
							 });

	ch->insertKeyCallback(GLFW_KEY_ESCAPE, GLFW_PRESS, 0,
						  [this]() -> void { glfwSetWindowShouldClose(window, GLFW_TRUE); });

	ch->insertKeyCallback(GLFW_KEY_F, GLFW_PRESS, 0,
						  [this]() -> void
						  {
							  // Set fullscreen
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
						  });
	data->objects.push_front(new Object(Position{0, 2, 0}, 1, 1, 1, new CollisionBox(), drawFluffy));
}

Game::~Game()
{
	delete graphicsHelper;
	delete config;
	delete ch;
	delete rm;
	delete cam;
}

void Game::run()
{
	GraphicsHelper::configureEnvironment();

	rm = new ResourceManager();
	loadTextures();
	cam->setAspectRatio(config->width, config->height);
	auto model = new Model(std::string("assets/models/").c_str(), rm);

	// Set up the shaders
	const auto shaderProgram = GraphicsHelper::loadShaders();
	double delta = 0.0;
	double fps = 0.0;

	int i = 0;
	while (!glfwWindowShouldClose(window))
	{
		const auto start = glfwGetTime();
		glfwPollEvents();

		// drawWindow(cam, shaderProgram, static_cast<float>(delta), rm, data);

		// TODO: fix the drawing of the interface
		// drawInterface(config->height, config->height, cam, shaderProgram, rm->getTexture("track"));
		model->draw(new Position(i % 20, i % 20, i % 20), static_cast<int>(glfwGetTime()) % 90, shaderProgram);
		i = (i + 1) % 40;

		glfwSwapBuffers(window);

		const auto end = glfwGetTime();
		delta = end - start;
		fps = 1.0 / delta;

		glfwSetWindowTitle(window, (WINDOW_TITLE + " (FPS: " + std::to_string(fps) + ")").c_str());
	}
}
void Game::loadTextures() const
{
	rm->loadTexture("assets/textures/null.png", 1, 1, "null");
	rm->loadTexture("assets/textures/kart/SNES_Donut_Plains_1.png", 1024, 1024, "track");
	rm->loadTexture("assets/textures/kart/background.png", 256, 1024, "background");
	rm->loadTexture("assets/textures/IME/ime_usp.png", 209, 668, "ime_usp");
	rm->loadTexture("assets/textures/IME/fluffy_4.png", 378, 378, "fluffy");
	rm->loadTexture("assets/textures/wheel_cap.png", 512, 512, "wheel_cap");
}

int main()
{
	const auto game = new Game();
	game->run();
	delete game;
}
