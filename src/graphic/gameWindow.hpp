#ifndef GAME_WINDOW_HPP
#define GAME_WINDOW_HPP

#include <GLFW/glfw3.h>
#include <string>

#include "game/data.hpp"

class GameWindow {
	GLFWwindow *window;
	static void setupOpenGL(int width, int height);
public:
	explicit GameWindow(const std::string &title, const GLFWimage *icon);
	~GameWindow();
	void run(const Data *data) const;
};

#endif //GAME_WINDOW_HPP
