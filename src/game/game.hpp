#pragma once

#ifndef GAME_HPP
#define GAME_HPP

#include "utils/data.hpp"

#include "graphic/gameWindow.hpp"

class Game
{
	static void configureEnvironment();

	GameWindow *window{};
	Data *data{};

public:
	Game();
	~Game();

	void run() const;
};

#endif
