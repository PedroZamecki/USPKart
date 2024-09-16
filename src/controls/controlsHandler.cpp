#include "controlsHandler.hpp"
#include <iostream>

unsigned int hash(const int key, const int action, const int mods)
{
	return (key & 0xFF) | ((glfwGetKeyScancode(key) & 0xFF) << 8) | ((action & 0xFF) << 16) | ((mods & 0xFF) << 24);
}

ControlsHandler::ControlsHandler(GLFWwindow *window) { keyCallbacks.clear(); }

ControlsHandler::~ControlsHandler() { keyCallbacks.clear(); }

void ControlsHandler::executeKeyCallback(const int key, const int action, const int mods) const
{
	const unsigned int hashKey = hash(key, action, mods);
	if (keyCallbacks.count(hashKey) == 0)
	{
		return;
	}
	keyCallbacks.at(hashKey)();
}
void ControlsHandler::insertKeyCallback(const int key, const int action, const int mods, std::function<void()> callback)
{
	if (keyCallbacks.count(hash(key, action, mods)) > 0)
	{
		std::cerr << "Key callback already exists" << std::endl;
		return;
	}
	keyCallbacks.insert_or_assign(hash(key, action, mods), callback);
}
