// Game.cpp : Defines the entry point for the application.
//

#include "game.hpp"

int main()
{
	Configuration *config = new Configuration();
	GLFWwindow* window = createWindow("USPKart v0.0.1", config);
	setupOpenGL();
	manageWindow(window);

	return 0;
}
