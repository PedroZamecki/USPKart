#include "utils.hpp"

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

void Position::setX(float x)
{
    this->x = x;
}

void Position::setY(float y) { this->y = y; }

void Position::setZ(float z)
{
    this->z = z;
}

float Position::getX()
{
    return x;
}

float Position::getY()
{
    return y;
}

float Position::getZ()
{
    return z;
}

Position Position::operator+(Position pos)
{
    return Position(x + pos.getX(), y + pos.getY(), z + pos.getZ());
}

Camera::Camera()
{
    pos = Position();
    pitch = 0.0f;
    yaw = 0.0f;
    roll = 0.0f;
    fov = 0.0f;
    aspectRatio = 0.0f;
    near = 0.0f;
    far = 0.0f;
}

Camera::Camera( Position pos,
                Position target,
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
    this->pitch = pitch;
    this->yaw = yaw;
    this->roll = roll;
    this->fov = fov;
    this->aspectRatio = aspectRatio;
    this->near = near;
    this->far = far;
}

void Camera::setPos(Position pos)
{
    this->pos = pos;
}

void Camera::setTarget(Position target)
{
    this->target = target;
}

void Camera::setPitch(float pitch)
{
    this->pitch = pitch;
}

void Camera::setYaw(float yaw)
{
    this->yaw = yaw;
}

void Camera::setRoll(float roll)
{
    this->roll = roll;
}

void Camera::setFOV(float fov)
{
    this->fov = fov;
}

void Camera::setAspectRatio(float aspectRatio)
{
    this->aspectRatio = aspectRatio;
}

void Camera::setNear(float near)
{
    this->near = near;
}

void Camera::setFar(float far)
{
    this->far = far;
}

Position Camera::getPos()
{
    return pos;
}

Position Camera::getTarget()
{
    return target;
}

float Camera::getPitch()
{
    return pitch;
}

float Camera::getYaw()
{
    return yaw;
}

float Camera::getRoll()
{
    return roll;
}

float Camera::getFOV()
{
    return fov;
}

float Camera::getAspectRatio()
{
    return aspectRatio;
}
