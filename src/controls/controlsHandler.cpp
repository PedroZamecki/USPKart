#include "controlsHandler.hpp"
#include <utils/logger.hpp>
#include <iostream>

ControlsHandler *ControlsHandler::instance = nullptr;
std::mutex ControlsHandler::mtx;

unsigned int hash(const int key, const int action, const int mods)
{
	return key & 0xFF | (glfwGetKeyScancode(key) & 0xFF) << 8 | (action & 0xFF) << 16 | (mods & 0xFF) << 24;
}

ControlsHandler::ControlsHandler() { keyCallbacks.clear(); }

ControlsHandler::~ControlsHandler() { keyCallbacks.clear(); }

void ControlsHandler::executeKeyCallback(const int key, const int action, const int mods)
{
	keyStates[key % GLFW_KEY_LAST] = action;
	if (const unsigned int hashKey = hash(key, action, mods); keyCallbacks.count(hashKey))
		keyCallbacks.at(hashKey)();
}

void ControlsHandler::insertKeyCallback(const int key, const int action, const int mods,
										const std::function<void()> &callback)
{
	const auto logger = Logger::getInstance();
	if (keyCallbacks.count(hash(key, action, mods)))
	{
		logger->error("Key callback for key" + std::string(glfwGetKeyName(key, glfwGetKeyScancode(key))) + "already exists");
		return;
	}
	keyCallbacks.insert_or_assign(hash(key, action, mods), callback);
}
