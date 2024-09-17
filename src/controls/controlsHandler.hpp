#ifndef CONTROLS_HANDLER_HPP
#define CONTROLS_HANDLER_HPP

#include <GLFW/glfw3.h>
#include <functional>
#include <map>

class ControlsHandler
{
public:
	explicit ControlsHandler();
	~ControlsHandler();

	void executeKeyCallback(int key, int action, int mods);
	void insertKeyCallback(int key, int action, int mods, const std::function<void()> &callback);
	[[nodiscard]] int getKeyState(const int key) const { return keyStates[key]; }

private:
	std::map<unsigned int, std::function<void()>> keyCallbacks;
	int keyStates[GLFW_KEY_LAST]{};
};

#endif
