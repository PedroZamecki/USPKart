#include "game.hpp"

Game::Game(): config(new Configuration), window(), data()
{
	graphicsHelper = new GraphicsHelper(data);
}

Game::~Game()
{
	delete graphicsHelper;
	delete config;
}

void Game::run() const
{
	graphicsHelper->configureEnvironment();

	graphicsHelper->createWindow("USPKart v0.0.2", config);

	graphicsHelper->manageWindow();
}

int main()
{
	const auto game = new Game();
	game->run();
	delete game;
}
