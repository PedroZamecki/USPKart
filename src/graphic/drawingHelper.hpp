#pragma once

#ifndef DRAWING_HELPER_HPP
#define DRAWING_HELPER_HPP

#include <resource/resourceManager.hpp>
#include <utils/camera.hpp>

void drawWindow(const Camera *cam, unsigned int shaderProgram, float deltaTime, const ResourceManager *rm);

void drawInterface(int height, int width, const Camera *cam, unsigned int shaderProgram, unsigned int interfaceTexture);

void drawFluffy(Position pos, float playerAngle, unsigned int shaderProgram, float deltaTime, unsigned int nullTexture);

void drawKart(Position pos, unsigned int shaderProgram, float playerAngle, float steeringAngle,
			  unsigned int wheelTexture, unsigned int nullTexture, unsigned int fluffyTexture, unsigned int imeTexture);

void drawTrack(Position pos, unsigned int shaderProgram, unsigned int texture);

void drawBackground(Position pos, unsigned int shaderProgram, unsigned int texture);

#endif
