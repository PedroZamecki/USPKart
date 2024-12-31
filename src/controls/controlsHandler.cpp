#include "controlsHandler.hpp"

ControlsHandler *ControlsHandler::instance = nullptr;
std::mutex ControlsHandler::mtx;


ControlsHandler::ControlsHandler() { pressCallbacks.clear(); }

ControlsHandler::~ControlsHandler() { pressCallbacks.clear(); }

void ControlsHandler::executeKeyCallback(const int key, const int action, const int mods)
{
	keyStates[key] = action;
	if (action == GLFW_PRESS)
		for (const auto &callback : pressCallbacks[key])
			callback();
	else if (action == GLFW_RELEASE)
		for (const auto &callback : releaseCallbacks[key])
			callback();
	else if (action == GLFW_REPEAT)
		for (const auto &callback : repeatCallbacks[key])
			callback();
}

void ControlsHandler::insertKeyCallback(const int key, const std::function<void()> &callback, const int action)
{
	if (action & PRESS)
		pressCallbacks[key].push_back(callback);
	if (action & RELEASE)
		releaseCallbacks[key].push_back(callback);
	if (action & REPEAT)
		repeatCallbacks[key].push_back(callback);
}
