// Game.cpp : Defines the entry point for the application.
//

#include "game.hpp"

Game::Game()
{
	config = new Configuration();
	graphicsHelper = new GraphicsHelper();
}

Game::~Game()
{
	delete config;
	delete graphicsHelper;
}

void Game::run()
{
	graphicsHelper->configureEnvironment();

	graphicsHelper->createWindow("USPKart v0.0.1", config);

	graphicsHelper->manageWindow();
}

int main()
{
	Game *game = new Game();
	game->run();
}
