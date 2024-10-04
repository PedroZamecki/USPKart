#ifndef GAME_WINDOW_HPP
#define GAME_WINDOW_HPP

#include <GLFW/glfw3.h>
#include <string>

class GameWindow {
	GLFWwindow *window;
	static void setupOpenGL(int width, int height);
public:
	explicit GameWindow(const std::string &title, const GLFWimage *icon);
	~GameWindow();
	void run() const;
};

#endif //GAME_WINDOW_HPP
