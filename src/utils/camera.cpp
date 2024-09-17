#include "camera.hpp"

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

Camera::Camera()
{
	this->pos = Position(-250, 250, -250);
	this->target = Position(0, 0, 0);
	this->pitch = 0;
	this->yaw = 0;
	this->roll = 0;
	this->fov = 80;
	this->aspectRatio = 0;
	this->near = 16;
	this->far = 4096;
}

Camera::Camera(Position const pos, Position const target, float const pitch, float const yaw, float const roll,
			   float const fov, float const aspectRatio, float const near, float const far) :
	pos(pos), target(target), pitch(pitch), yaw(yaw), roll(roll), fov(fov), aspectRatio(aspectRatio), near(near),
	far(far)
{
}

glm::mat4 Camera::getViewMatrix() const { return lookAt(pos.toVec3(), target.toVec3(), up()); }

glm::mat4 Camera::getProjectionMatrix() const { return glm::perspective(glm::radians(fov), aspectRatio, near, far); }
