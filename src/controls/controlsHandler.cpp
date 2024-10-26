#include "controlsHandler.hpp"
#include <iostream>
#include <utils/logger.hpp>

ControlsHandler *ControlsHandler::instance = nullptr;
std::mutex ControlsHandler::mtx;


ControlsHandler::ControlsHandler() { keyCallbacks.clear(); }

ControlsHandler::~ControlsHandler() { keyCallbacks.clear(); }

void ControlsHandler::executeKeyCallback(const int key, const int action, const int mods)
{
	keyStates[key] = action;
	if (action == GLFW_RELEASE)
		return;
	for (const auto &function : keyCallbacks[key])
	{
		function();
	}
}

void ControlsHandler::insertKeyCallback(const int key, const std::function<void()> &callback)
{
	keyCallbacks[key].push_back(callback);
}
