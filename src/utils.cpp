#include <utils.hpp>

Position::Position()
{
    x = 0.0f;
    y = 0.0f;
    z = 0.0f;
}

Position::Position(float x, float y, float z)
{
    this->x = x;
    this->y = y;
    this->z = z;
}

void Position::setPos(float x, float y, float z)
{
    this->x = x;
    this->y = y;
    this->z = z;
}

Position Position::operator+(Position pos)
{
    return Position(x + pos.x, y + pos.y, z + pos.z);
}

Position Position::operator-(Position pos)
{
    return Position(x - pos.x, y - pos.y, z - pos.z);
}

Position Position::operator*(float scalar)
{
    return Position(x * scalar, y * scalar, z * scalar);
}

Position Position::operator/(float scalar)
{
    return Position(x / scalar, y / scalar, z / scalar);
}

float Position::dot(Position pos)
{
    return x * pos.x + y * pos.y + z * pos.z;
}

Position Position::cross(Position pos)
{
    return Position(y * pos.z - z * pos.y, z * pos.x - x * pos.z, x * pos.y - y * pos.x);
}

Camera::Camera()
{
    this->pos = Position();
    this->target = Position();
    this->targetDist = 250;
    this->pitch = -20;
    this->yaw = 0;
    this->roll = 0;
    this->fov = 20;
    this->aspectRatio = 1;
    this->near = 0;
    this->far = 0;
}

Camera::Camera( Position pos,
                Position target,
                float targetDist,
                float pitch,
                float yaw,
                float roll,
                float fov,
                float aspectRatio,
                float near,
                float far)
{
    this->pos = pos;
    this->target = target;
    this->targetDist = targetDist;
    this->pitch = pitch;
    this->yaw = yaw;
    this->roll = roll;
    this->fov = fov;
    this->aspectRatio = aspectRatio;
    this->near = near;
    this->far = far;
}