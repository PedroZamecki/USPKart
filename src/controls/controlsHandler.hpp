#ifndef CONTROLS_HANDLER_HPP
#define CONTROLS_HANDLER_HPP

#include <GLFW/glfw3.h>
#include <functional>
#include <map>
#include <mutex>

#define RELEASE 1
#define PRESS 2
#define REPEAT 4
#define ALL 7

class ControlsHandler
{
	static ControlsHandler *instance;
	static std::mutex mtx;

	explicit ControlsHandler();
	~ControlsHandler();

	std::map<unsigned int, std::vector<std::function<void()>>> pressCallbacks;
	std::map<unsigned int, std::vector<std::function<void()>>> releaseCallbacks;
	std::map<unsigned int, std::vector<std::function<void()>>> repeatCallbacks;

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
	void insertKeyCallback(int key, const std::function<void()> &callback, const int action = PRESS);
	[[nodiscard]] int getKeyState(const int key) const { return keyStates[key]; }
};

#endif
