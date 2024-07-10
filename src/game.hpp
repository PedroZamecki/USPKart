#pragma once

#include <iostream>

#include "graphicsHelper.hpp"

class Game {
    public:
        Game();
        ~Game();

        void run();

    private:
        GraphicsHelper *graphicsHelper;
        Configuration *config;
        SDL_Window* window;
};