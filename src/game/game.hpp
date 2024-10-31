#pragma once

#ifndef GAME_HPP
#define GAME_HPP

#include "graphic/gameWindow.hpp"

class Game
{
	static void configureEnvironment();

	GameWindow *window{};

public:
	Game();
	~Game();

	void run() const;
};

#endif
