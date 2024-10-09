#include "game.hpp"

#include <iostream>
#include <string>

#include "model/model.hpp"

#define WINDOW_TITLE std::string("USPKart v") + USP_KART_VERSION
#define getPointer static_cast<Game *>(glfwGetWindowUserPointer(window))

void Game::configureEnvironment()
{
#ifdef _WIN32
	std::cout << "Running on Windows" << std::endl;
#elif __linux__
	if (const std::string sessionType = std::getenv("XDG_SESSION_TYPE"); !sessionType.empty())
	{
		if (std::string(sessionType) == "wayland")
		{
			std::cout << "Running on Linux (Wayland)" << std::endl;
			putenv(const_cast<char *>("GDK_BACKEND=wayland"));
		}
		else if (sessionType == "x11")
		{
			std::cout << "Running on Linux (X11)" << std::endl;
			putenv(const_cast<char *>("GDK_BACKEND=x11"));
		}
		else
		{
			std::cout << "Unknown session type: " << sessionType << std::endl;
		}
	}
	else
	{
		std::cout << "Variable XDG_SESSION_TYPE not defined." << std::endl;
	}
#else
	std::cout << "Not running on known environment, please ask for new implementation if required" << std::endl;
	throw std::runtime_error("Unknown environment");
#endif
}

Game::Game() : data(new Data)
{
	window = new GameWindow(WINDOW_TITLE, (GLFWimage *)ResourceManager::getInstance()->loadIcon("assets/icon.png"));
	data->objects.push_front(new Object("assets/models/model.fbx", Position{0, 0, 0}, .01, .01, .01, 0));
	data->objects.push_front(new Object("assets/models/model.fbx", Position{0, 2, 0}, .01, .01, .01, 0));
}

Game::~Game()
{
	delete window;
	delete data;
}

void Game::run() const
{
	configureEnvironment();
	window->run(data);
}

int main()
{
	const auto game = new Game();
	game->run();
	delete game;
}
