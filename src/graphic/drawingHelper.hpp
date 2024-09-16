#pragma once

#ifndef DRAWING_HELPER_HPP
#define DRAWING_HELPER_HPP

#include <game/utils.hpp>
#include <resource/resourceManager.hpp>

void drawWindow(const Camera *cam, unsigned int shaderProgram, float deltaTime, const ResourceManager *rm);

void drawInterface(int height, int width, const Camera *cam, unsigned int shaderProgram, unsigned int interfaceTexture);

void drawFluffy(Position pos, float playerAngle, unsigned int shaderProgram, float deltaTime, bool restart,
				unsigned int nullTexture, unsigned int skin);

void drawKart(Position pos, unsigned int shaderProgram, float deltaTime, bool restart, float playerAngle,
			  float steeringAngle, unsigned int wheelTexture, unsigned int nullTexture, unsigned int fluffyTexture,
			  unsigned int imeTexture, unsigned int skin);

void drawTrack(Position pos, unsigned int shaderProgram, float deltaTime, float playerAngle, unsigned int texture);

void drawBackground(Position pos, unsigned int shaderProgram, float deltaTime, unsigned int texture);

#endif
