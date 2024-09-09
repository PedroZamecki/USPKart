#pragma once

#ifndef GAME_HPP
#define GAME_HPP

#include <game/utils.hpp>
#include <graphic/graphicsHelper.hpp>

class Game {
    public:
        Game();
        ~Game();

        void run() const;

    private:
        GraphicsHelper *graphicsHelper;
        Configuration *config;
        GLFWwindow *window;
        Data *data;
};

#endif