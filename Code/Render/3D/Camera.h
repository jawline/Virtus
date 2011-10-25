#ifndef _CAMERA_DEF_H_
#define _CAMERA_DEF_H_
#include <Math/Vector.h>

#define DEGREE_TO_RADIAN(number) number * (3.14159265358979323846 / 180.0f )

class Camera
{
private:

    point3f_t Position;
    point3f_t Rotation;

    float angle, ratio, nearD, farD;

    void setCameraInternals(float angle, float ratio, float near, float far);

public:
    Camera();
    ~Camera();

    void setPosition(float x, float y, float z);
    void move(float x, float y, float z);

    point3f_t getPosition()
    {
        return Position;
    }

    void setRotation(float x, float y, float z);
    void rotate(float x, float y, float z);

    point3f_t getRotation()
    {
        return Rotation;
    }

    //Translate to the cameras position and rotate to the correct angles
    void renderMoveTo();
};

#endif //_CAMERA_DEF_H_
