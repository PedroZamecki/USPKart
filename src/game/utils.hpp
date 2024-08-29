#pragma once

#ifndef UTILS_HPP
#define UTILS_HPP

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

enum animationType {normal, pushing, walking, pulling, hanging, hangingLeft, hangingRight, seated};
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
                float targetDist,
                float pitch,
                float yaw,
                float roll,
                float fov,
                float aspectRatio,
                float near,
                float far);
        Position pos = {0.0f, 0.0f, 0.0f};
        Position target = {1.0f, 1.0f, 1.0f};
        float targetDist = 250;
        float pitch = -20, yaw = 0, roll = 0;
        float fov = 20;
        float aspectRatio = 1;
        float near = 0, far = 0;

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
        animationType animation;
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