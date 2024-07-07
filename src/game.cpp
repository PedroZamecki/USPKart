// Game.cpp : Defines the entry point for the application.
//

#include "game.hpp"

int main()
{
	putenv(const_cast<char*>("GDK_BACKEND=wayland"));
	putenv(const_cast<char*>("SDL_VIDEODRIVER=wayland"));

	Configuration *config = new Configuration();
	SDL_Window* window = createWindow("USPKart v0.0.1", config);
	manageWindow(window);

	return 0;
}
