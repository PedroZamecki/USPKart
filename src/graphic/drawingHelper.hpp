#pragma once

#ifndef DRAWING_HELPER_HPP
#define DRAWING_HELPER_HPP

#include <resource/resourceManager.hpp>
#include <game/utils.hpp>

void drawWindow(int height, int width, const Camera *cam, GLuint shaderProgram, float deltaTime, const ResourceManager *rm);

void drawInterface( int height,
                    int width,
                    const Camera* cam,
                    GLuint shaderProgram,
                    GLuint interfaceTexture);

void drawFluffy(Position pos,
                float playerAngle,
                GLuint shaderProgram,
                float deltaTime,
                bool restart,
                GLuint nullTexture,
                GLuint skin);

void drawKart(Position pos,
              unsigned int shaderProgram,
              float deltaTime,
              bool restart,
              float playerAngle,
              float steeringAngle,
              unsigned int wheelTexture,
              unsigned int nullTexture,
              unsigned int fluffyTexture,
              unsigned int imeTexture,
              unsigned int skin);

void drawTrack(Position pos,
               unsigned int shaderProgram,
               float deltaTime,
               float playerAngle,
               unsigned int texture);

void drawBackground(Position pos,
                    unsigned int shaderProgram,
                    float deltaTime,
                    unsigned int texture);

#endif