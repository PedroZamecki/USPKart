#include "game.hpp"

#include <graphic/drawingHelper.hpp>

Game::Game(): config(new Configuration), window(), data(), cam(new Camera), rm(new ResourceManager)
{
	graphicsHelper = new GraphicsHelper();
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
	graphicsHelper->configureEnvironment();

	const auto window = graphicsHelper->createWindow("USPKart v0.0.2", config);
	ch = new ControlsHandler(window);

	rm = new ResourceManager();
	loadTextures();

	// Set up the shaders
	const auto shaderProgram = GraphicsHelper::loadShaders();
	auto now = time(nullptr);
	float deltaTime = 0.0f;
	while (!glfwWindowShouldClose(window)) {
	    glfwPollEvents();

	    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	    glUseProgram(shaderProgram);

	    drawWindow(config->getHeight(), config->getWidth(), cam, shaderProgram,
	               deltaTime, rm);

	    drawInterface(config->getHeight(), config->getWidth(), cam, shaderProgram,
	                  rm->getTexture("track"));

	    glfwSwapBuffers(window);
	    deltaTime = static_cast<float>(time(nullptr) - now);
	    now = time(nullptr);
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
