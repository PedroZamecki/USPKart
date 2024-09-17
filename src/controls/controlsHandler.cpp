#include "controlsHandler.hpp"
#include <iostream>

unsigned int hash(const int key, const int action, const int mods) {
    return key & 0xFF | (glfwGetKeyScancode(key) & 0xFF) << 8 | (action & 0xFF) << 16 | (mods & 0xFF) << 24;
}

ControlsHandler::ControlsHandler() { keyCallbacks.clear(); }

ControlsHandler::~ControlsHandler() { keyCallbacks.clear(); }

void ControlsHandler::executeKeyCallback(const int key, const int action, const int mods) {
    keyStates[key % GLFW_KEY_LAST] = action;
    if (const unsigned int hashKey = hash(key, action, mods); keyCallbacks.contains(hashKey))
        keyCallbacks.at(hashKey)();
}

void ControlsHandler::insertKeyCallback(const int key, const int action, const int mods,
                                        const std::function<void()> &callback) {
    if (keyCallbacks.contains(hash(key, action, mods))) {
        std::cerr << "Key callback already exists" << std::endl;
        return;
    }
    keyCallbacks.insert_or_assign(hash(key, action, mods), callback);
}
