#pragma once

#ifndef GAME_HPP
#define GAME_HPP

#include <controls/controlsHandler.hpp>
#include <game/utils.hpp>
#include <graphic/graphicsHelper.hpp>
#include <resource/resourceManager.hpp>

class Game
{
public:
	Game();
	~Game();

	void run();

private:
	Configuration *config;
	GraphicsHelper *graphicsHelper;
	GLFWwindow *window;
	Data *data;
	Camera *cam;
	ResourceManager *rm;
	ControlsHandler *ch{};

	void loadTextures() const;
};

#endif
