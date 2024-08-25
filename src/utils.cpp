#include <utils.hpp>

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

Position::Position(const float x, const  float y, const float z)
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

Position Position::operator+(Position const pos) const
{
    return {x_ + pos.x(), y_ + pos.y(), z_ + pos.z()};
}

Position Position::operator-(Position const pos) const
{
    return {x_ - pos.x(), y_ - pos.y(), z_ - pos.z()};
}

Position Position::operator*(float const scalar) const
{
    return {x_ * scalar, y_ * scalar, z_ * scalar};
}

Position Position::operator/(float const scalar) const
{
    return {x_ / scalar, y_ / scalar, z_ / scalar};
}

float Position::dot(Position const pos) const
{
    return {x_ * pos.x() + y_ * pos.y() + z_ * pos.z()};
}

Position Position::cross(Position const pos) const
{
    return {y_ * pos.z() - z_ * pos.y(), z_ * pos.x() - x_ * pos.z(), x_ * pos.y() - y_ * pos.x()};
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
    this->far = 4096;
}

Camera::Camera( Position const pos,
                Position const target,
                float const targetDist,
                float const pitch,
                float const yaw,
                float const roll,
                float const fov,
                float const aspectRatio,
                float const near,
                float const far)
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

glm::mat4 Camera::getViewMatrix() const
{
    return lookAt(glm::vec3(pos.x(), pos.y(), pos.z()),
                                 glm::vec3(target.x(), target.y(), target.z()),
                                 glm::vec3(0, 1, 0));
}

glm::mat4 Camera::getProjectionMatrix() const
{
    return glm::perspective(glm::radians(fov), aspectRatio, near, far);
}
