#ifndef _PHYSICS_VECTOR_DEF_H_
#define _PHYSICS_VECTOR_DEF_H_


class Vector2f {
private:
    float m_Length;
    bool m_Dirty;

public:
    Vector2f();

    float x, y;

    float length();

    Vector2f scale(float time_milis);
    Vector2f normalize();

    Vector2f add(Vector2f vector);
    Vector2f subtract(Vector2f vector);
    Vector2f negative();

    Vector2f multiply(float f);
    Vector2f divide(float f);

    Vector2f reflect(Vector2f other);

    float dotProduct(Vector2f vector);
    float angle(Vector2f vector);
};

class Vector3f {
private:
    float m_storeLength;
    bool m_lengthDirty;

public:
    Vector3f();
    Vector3f(float x, float y, float z);
    Vector3f(float* matrix);

    void setX(float x);
    void setY(float y);
    void setZ(float z);
    void setMatrix(float* newMatrix);

    float getX();
    float getY();
    float getZ();
    float* getMatrix();

    float m_vectorMatrix[3];

    float length();

    Vector3f normalize();
    Vector3f cross(Vector3f other);
};

typedef Vector2f point2f_t;
typedef Vector3f point3f_t;

#endif //_PHYSICS_VECTOR_DEF_H_
