#pragma once

#ifndef GAME_HPP
#define GAME_HPP

#include <graphic/graphicsHelper.hpp>
#include <game/utils.hpp>
#include <resource/resourceManager.hpp>
#include <controls/controlsHandler.h>

class Game {
    public:
        Game();
        ~Game();

        void run();

    private:
        Configuration *config;
        GraphicsHelper *graphicsHelper;
        GLFWwindow *window;
        Data *data;
        Camera *cam;
        ResourceManager *rm;
        ControlsHandler *ch{};

        void loadTextures() const;
};

#endif