﻿#pragma once

#ifndef GAME_HPP
#define GAME_HPP

#include <iostream>

#include <utils.hpp>
#include <graphicsHelper.hpp>

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

#endif