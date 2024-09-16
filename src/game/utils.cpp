#include "utils.hpp"

Data::Data()
{
	// Constructor
}

Data::~Data()
{
	// Destructor
}

Position::Position()
{
	x_ = 0.0f;
	y_ = 0.0f;
	z_ = 0.0f;
}

Position::Position(const float x, const float y, const float z)
{
	this->x_ = x;
	this->y_ = y;
	this->z_ = z;
}

void Position::setPos(const float x, const float y, const float z)
{
	this->x_ = x;
	this->y_ = y;
	this->z_ = z;
}

Position Position::operator+(Position const pos) const { return {x_ + pos.x(), y_ + pos.y(), z_ + pos.z()}; }

Position Position::operator-(Position const pos) const { return {x_ - pos.x(), y_ - pos.y(), z_ - pos.z()}; }

Position Position::operator*(float const scalar) const { return {x_ * scalar, y_ * scalar, z_ * scalar}; }

Position Position::operator/(float const scalar) const { return {x_ / scalar, y_ / scalar, z_ / scalar}; }

float Position::dot(Position const pos) const { return {x_ * pos.x() + y_ * pos.y() + z_ * pos.z()}; }

Position Position::cross(Position const pos) const
{
	return {y_ * pos.z() - z_ * pos.y(), z_ * pos.x() - x_ * pos.z(), x_ * pos.y() - y_ * pos.x()};
}

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
