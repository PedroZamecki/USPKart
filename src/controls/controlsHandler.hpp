#ifndef CONTROLS_HANDLER_HPP
#define CONTROLS_HANDLER_HPP

#include <GLFW/glfw3.h>
#include <functional>
#include <map>

class ControlsHandler {
public:
	explicit ControlsHandler(GLFWwindow *window);
	~ControlsHandler();

	void executeKeyCallback(int key, int action, int mods) const;
	void insertKeyCallback(int key, int action, int mods, std::function<void()> callback);
private:
	std::map<unsigned int, std::function<void()>> keyCallbacks;
};

#endif
