#ifndef CONTROLS_HANDLER_H
#define CONTROLS_HANDLER_H

#include <GLFW/glfw3.h>
#include <functional>
#include <map>

class ControlsHandler {
public:
	explicit ControlsHandler(GLFWwindow *window);
	~ControlsHandler();

	static void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
	void executeKeyCallback(int key, int scancode, int action, int mods) const;

private:
	std::map<unsigned int, std::function<void()>> keyCallbacks;
};

#endif
