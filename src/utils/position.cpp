#include "position.hpp"

Position::Position()
{
	x_ = 0.0f;
	y_ = 0.0f;
	z_ = 0.0f;
}
Position::Position(const glm::vec3 vector)
{
	x_ = vector.x;
	y_ = vector.y;
	z_ = vector.z;
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

float Position::dot(Position const pos) const { return {x_ * pos.x() + y_ * pos.y() + z_ * pos.z()}; }

Position Position::cross(Position const pos) const
{
	return {y_ * pos.z() - z_ * pos.y(), z_ * pos.x() - x_ * pos.z(), x_ * pos.y() - y_ * pos.x()};
}

float Position::z() const { return z_; }
