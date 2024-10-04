#pragma once

#ifndef GAME_HPP
#define GAME_HPP

#include <graphic/graphicsHelper.hpp>
#include <utils/camera.hpp>
#include <utils/data.hpp>

class Game
{
public:
	Game();
	~Game();

	void run() const;

private:
	GraphicsHelper *graphicsHelper;
	GLFWwindow *window;
	Data *data;
	Camera *cam;
};

#endif
