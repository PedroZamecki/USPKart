#pragma once

#ifndef GAME_HPP
#define GAME_HPP

#include <graphic/graphicsHelper.hpp>
#include <utils/camera.hpp>
#include <utils/data.hpp>

class Game
{
	GraphicsHelper *graphicsHelper;
	GLFWwindow *window;
	Data *data;
	Camera *cam;

public:
	Game();
	~Game();

	void run() const;
};

#endif
