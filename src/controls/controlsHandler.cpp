#include "controlsHandler.h"
#include <iostream>

unsigned int hash(const int key, const int scancode, const int action, const int mods)
{
	return (key & 0xFF) | ((scancode & 0xFF) << 8) | ((action & 0xFF) << 16) | ((mods & 0xFF) << 24);
}

ControlsHandler::ControlsHandler(GLFWwindow *window) {
	glfwSetKeyCallback(window, keyCallback);
	glfwSetWindowUserPointer(window, this);

	keyCallbacks.clear();
	keyCallbacks.insert_or_assign(67840, [window]() -> void {
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	});
}

ControlsHandler::~ControlsHandler() {
	keyCallbacks.clear();
}

void ControlsHandler::keyCallback(GLFWwindow *window, const int key, const int scancode, const int action, const int mods)
{
	if (const auto instance = static_cast<ControlsHandler*>(glfwGetWindowUserPointer(window)))
	{
		instance->executeKeyCallback(key, scancode, action, mods);
	}
}

void ControlsHandler::executeKeyCallback(const int key, const int scancode, const int action, const int mods) const {
	const unsigned int hashKey = hash(key, scancode, action, mods);

	std::cout << "Key: " << key << std::endl;
	std::cout << "Scancode: " << scancode << std::endl;
	std::cout << "Action: " << action << std::endl;
	std::cout << "Mods: " << mods << std::endl;
	std::cout << "Hash: " << hashKey << std::endl;
	if (keyCallbacks.count(hashKey) == 0)
	{
		std::cout << "Key callback not found." << std::endl;
		return;
	}
	keyCallbacks.at(hashKey)();
}