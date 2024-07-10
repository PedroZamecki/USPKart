class Position
{
    public:
        Position();
        Position(float x, float y, float z);

        void setPos(float x, float y, float z);
        void setX(float x);
        void setY(float y);
        void setZ(float z);

        float getX();
        float getY();
        float getZ();

        Position operator+(Position pos);

    private:
        float x, y, z;
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

        void setPos(Position pos);
        void setTarget(Position target);
        void setPitch(float pitch);
        void setYaw(float yaw);
        void setRoll(float roll);
        void setFOV(float fov);
        void setAspectRatio(float aspectRatio);
        void setNear(float near);
        void setFar(float far);

        Position getPos();
        Position getTarget();
        float getPitch();
        float getYaw();
        float getRoll();
        float getFOV();
        float getAspectRatio();
        float getNear();
        float getFar();

    private:
        Position pos;
        Position target;
        float pitch, yaw, roll;
        float fov;
        float aspectRatio;
        float near, far;
};