#pragma once

#ifndef UTILS_HPP
#define UTILS_HPP

#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define SQUARE(x) ((x)*(x))
#define ROUND(x) ((x < 0)?(int)(x-0.5):(int)(x+0.5))
#define SIGN(x) ((x >= 0)?1:-1)

#define MAX_Z 6 //4
#define MAX_Y 30
#define MAX_X 9 //7

#define PI 3.142857

#define EPS 0.00001

enum skinType {none, professor, last};
enum camViewType {back, front, left, right};

class Position
{
    public:
        Position();
        Position(float x, float y, float z);

        void setPos(float x, float y, float z);

        Position operator+(Position pos) const;
        Position operator-(Position pos) const;
        Position operator*(float scalar) const;
        Position operator/(float scalar) const;

        auto toVec3() const -> glm::vec3 { return {x_, y_, z_}; };
        auto norm() const { return std::sqrt(SQUARE(x_) + SQUARE(y_) + SQUARE(z_)); }
        const auto dist(const Position pos) -> float {
            return (std::sqrt(SQUARE(x_ - pos.x()) + SQUARE(y_ - pos.y()) + SQUARE(z_ - pos.z())));
        }

        static const auto dist(const Position pos1, const Position pos2) ->  float {
            return (std::sqrt(SQUARE(pos1.x() - pos2.x()) + SQUARE(pos1.y() - pos2.y()) + SQUARE(pos1.z() - pos2.z())));
        }

        float dot(Position pos) const;
        Position cross(Position pos) const;
        float x() const { return x_; }
        float y() const { return y_; }
        float z() const { return z_; }

    private:
        float x_, y_, z_;
};

class Camera
{
    public:
        Camera();
        Camera( Position pos,
                Position target,
                float pitch,
                float yaw,
                float roll,
                float fov,
                float aspectRatio,
                float near,
                float far);
        Position pos, target;
        float pitch, yaw, roll, fov, aspectRatio, near, far;

        void setAspectRatio(const int width, const int height) { aspectRatio = static_cast<float>(width) / static_cast<float>(height); };

        auto targetDist() const -> auto {
            return (target - pos).norm();
        }

        auto forward() const -> glm::vec<3, float> {
            return {
                std::cos(yaw) * std::cos(pitch),
                std::sin(pitch),
                std::sin(yaw) * std::cos(pitch)
            };
        }
        auto right() const -> glm::vec<3, float> {
            return cross( forward(), glm::vec<3, float>({0, 1, 0}));
        }
        auto up() const -> glm::vec<3, float> {
            // Calculate the cross product between the forward vector and {0, 1, 0}
            return cross(right(), forward());
        }
        auto direction() const -> glm::vec<3, float> {
            return {
                (cos(glm::radians(yaw))) * cos(glm::radians(pitch)),
                sin(glm::radians(pitch)),
                sin(glm::radians(yaw)) * cos(glm::radians(pitch))
            };
        }
        auto cameraFront() const -> auto { return normalize(direction()); };

        glm::mat4 getViewMatrix() const;

        glm::mat4 getProjectionMatrix() const;
};


class CollisionBox {
    public:
        CollisionBox();
        CollisionBox(Position pos, float width, float height, float depth);
        ~CollisionBox();
        bool isInside(CollisionBox box);
    private:
        Position pos;
        float width, height, depth;
};

class Object {
    public:
        Object();
        ~Object();
    private:
        Position pos;
        float width, height, depth;
        CollisionBox box;
};

class Character: Object {
    public:
        Character();
        ~Character();
    private:
        Position pos;
        CollisionBox box;
        camViewType camView;
};

class Player: Character {
    public:
        Player();
        ~Player();
    private:
        Position pos;
        float width, height, depth;
        CollisionBox box;
        skinType skin;
        camViewType camView;
};

class Data {
    public:
        Data();
        ~Data();
    private:
        Object *objects;
};

#endif