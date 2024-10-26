#ifndef CONTROLS_HANDLER_HPP
#define CONTROLS_HANDLER_HPP

#include <GLFW/glfw3.h>
#include <functional>
#include <map>
#include <mutex>

class ControlsHandler
{
	static ControlsHandler *instance;
	static std::mutex mtx;

	explicit ControlsHandler();
	~ControlsHandler();

	std::map<unsigned int, std::vector<std::function<void()>>> keyCallbacks;
	int keyStates[GLFW_KEY_LAST]{};

public:
	static ControlsHandler *getInstance()
	{
		if (instance == nullptr)
		{
			std::lock_guard lock(mtx);
			if (instance == nullptr)
			{
				instance = new ControlsHandler();
			}
		}
		return instance;
	}

	void executeKeyCallback(int key, int action, int mods);
	void insertKeyCallback(int key, const std::function<void()> &callback);
	[[nodiscard]] int getKeyState(const int key) const { return keyStates[key]; }
};

#endif
