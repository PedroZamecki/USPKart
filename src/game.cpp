// Game.cpp : Defines the entry point for the application.
//

#include "game.hpp"

int main()
{
	Configuration *config = new Configuration();
	configureEnvironment();

	SDL_Window* window = createWindow("USPKart v0.0.1", config);
	manageWindow(window);

	return 0;
}
