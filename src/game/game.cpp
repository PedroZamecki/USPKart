#include "game.hpp"

#include <iostream>
#include <string>

#include "model/model.hpp"
#include "object/character/player.hpp"
#include "object/track.hpp"
#include "utils/logger.hpp"

void Game::configureEnvironment()
{
	const auto logger = Logger::getInstance();
#ifdef _WIN32
	logger->debug("Running on Windows");
#elif __linux__
	if (const std::string sessionType = std::getenv("XDG_SESSION_TYPE"); !sessionType.empty())
	{
		if (std::string(sessionType) == "wayland")
		{
			logger->debug("Running on Linux (Wayland)");
			putenv(const_cast<char *>("GDK_BACKEND=wayland"));
		}
		else if (sessionType == "x11")
		{
			logger->debug("Running on Linux (X11)");
			putenv(const_cast<char *>("GDK_BACKEND=x11"));
		}
		else
		{
			logger->warning("Unknown session type: " + sessionType);
		}
	}
	else
	{
		logger->warning("Variable XDG_SESSION_TYPE not defined. Might have issues with window creation.");
	}
#else
	logger->error("Not running on known environment, please ask for new implementation if required");
	throw std::runtime_error("Unknown environment");
#endif
}

Game::Game()
{
	window =
		new GameWindow("Loading USP Kart...", (GLFWimage *)ResourceManager::getInstance()->loadIcon("assets/icon.png"));
}

Game::~Game() { delete window; }

void Game::run() const
{
	const auto data = new Data;
	data->objects.push_back(new Object("assets/models/block.obj", {2, .5, 2}));
	configureEnvironment();
	window->run(data);
	delete data;
}

int main()
{
	const auto game = new Game();
	game->run();
	delete game;
}
