#pragma once

#ifndef UTILS_HPP
#define UTILS_HPP

#define SQUARE(x) ((x)*(x))
#define ROUND(x) ((x < 0)?(int)(x-0.5):(int)(x+0.5))
#define SIGN(x) ((x >= 0)?1:-1)

#define MAX_Z 6 //4
#define MAX_Y 30
#define MAX_X 9 //7

#define PI 3.142857

#define EPS 0.00001

enum animationType {normal, pushing, walking, pulling, hanging, hangingLeft, hangingRight, seated};
enum skinType {none=0, professor, last};
enum camViewType {back=0, front, left, right};


class Data {
    public:
        Data();
        ~Data();
};

class Position
{
    public:
        Position();
        Position(float x, float y, float z);

        void setPos(float x, float y, float z);

        Position operator+(Position pos);
        Position operator-(Position pos);
        Position operator*(float scalar);
        Position operator/(float scalar);

        float dot(Position pos);
        Position cross(Position pos);

        float x, y, z;
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
};

#endif