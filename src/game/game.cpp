#include "game.hpp"

#include <chrono>
#include <graphic/drawingHelper.hpp>
#include <iostream>

#define getPointer static_cast<Game *>(glfwGetWindowUserPointer(window))
#define Clock std::chrono::system_clock

Game::Game():	config(new Configuration),
		graphicsHelper(new GraphicsHelper),
		window(graphicsHelper->createWindow("USPKart v0.0.2", config)),
		data(new Data),
		cam(new Camera),
		rm(new ResourceManager),
		ch(new ControlsHandler(window)) {
	glfwSetWindowUserPointer(window, this);
	glfwSetKeyCallback(window, []
		(GLFWwindow *window, const int key, int, const int action, const int mods) {
		const auto game = getPointer;
		game->ch->executeKeyCallback(key, action, mods);
	});
	glfwSetWindowSizeCallback(window, [](GLFWwindow *window, const int width, const int height) {
		if (const auto game = getPointer; !game->config->fullScreen) {
			game->config->width = width;
			game->config->height = height;
			game->cam->setAspectRatio(width, height);
		}
		GraphicsHelper::setupOpenGL(width, height);
	});
	glfwSetWindowPosCallback(window, [](GLFWwindow *window, const int x, const int y) {
		 if (const auto game = getPointer; !game->config->fullScreen) {
			game->config->posX = x;
			game->config->posY = y;
		}
	});

	ch->insertKeyCallback(GLFW_KEY_ESCAPE, GLFW_PRESS, 0, [this]() -> void {
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	});

	ch->insertKeyCallback(GLFW_KEY_F, GLFW_PRESS, 0, [this]() -> void {
		// Set fullscreen
		if (config->fullScreen) {
			glfwSetWindowMonitor(window, nullptr, config->posX, config->posY, config->width, config->height, GLFW_DONT_CARE);
			glfwSetWindowPos(window, config->posX, config->posY);
			glfwSetWindowSize(window, config->width, config->height);
			glfwSetWindowAttrib(window, GLFW_DECORATED, !config->borderless);
			glfwSetWindowAttrib(window, GLFW_RESIZABLE, config->resizable);
			cam->setAspectRatio(config->width, config->height);
			config->fullScreen = false;
		} else {
			const auto monitor = glfwGetPrimaryMonitor();
			const auto mode = glfwGetVideoMode(monitor);
			cam->setAspectRatio(mode->width, mode->height);
			glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
			config->fullScreen = true;
		}
	});

}

Game::~Game()
{
	delete graphicsHelper;
	delete config;
	delete ch;
	delete rm;
	delete cam;
}

void Game::run() {
	GraphicsHelper::configureEnvironment();

	rm = new ResourceManager();
	loadTextures();
	cam->setAspectRatio(config->width, config->height);

	// Set up the shaders
	const auto shaderProgram = GraphicsHelper::loadShaders();
        std::chrono::duration<double> delta{};
	double fps = 0.0;

	while (!glfwWindowShouldClose(window)) {
	    std::chrono::time_point<Clock> start = Clock::now();
	    glfwPollEvents();

	    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	    glUseProgram(shaderProgram);

	    drawWindow(config->height, config->height, cam, shaderProgram, static_cast<float>(delta.count()), rm);

	    drawInterface(config->height, config->height, cam, shaderProgram,
	    rm->getTexture("track"));

	    glfwSwapBuffers(window);

	    std::chrono::time_point<Clock> end = Clock::now();
	    delta = end - start;
	    if (delta.count() > 0) {
	        fps = 1.0 / delta.count();
	    }

	    glfwSetWindowTitle(window, ("USPKart v0.0.2 (FPS: " + std::to_string(fps) + ")").c_str());
	}
}
void Game::loadTextures() const {
	rm->loadTexture("assets/textures/null.png", 1, 1, "null");
	rm->loadTexture("assets/textures/kart/SNES_Donut_Plains_1.png", 1024, 1024, "track");
	rm->loadTexture("assets/textures/kart/background.png", 256, 1024, "background");
	rm->loadTexture("assets/textures/IME/ime_usp.png", 209, 668, "ime_usp");
	rm->loadTexture("assets/textures/IME/fluffy_4.png", 378, 378,"fluffy");
	rm->loadTexture("assets/textures/wheel_cap.png", 512, 512, "wheel_cap");
}

int main()
{
	const auto game = new Game();
	game->run();
	delete game;
}
