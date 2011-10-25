#include "Vector.h"
#include <math.h>

Vector2f::Vector2f()
{
    m_Dirty = true;
    x = 0;
    y = 0;
}

Vector2f Vector2f::reflect(Vector2f normal)
{
    Vector2f result;
    result = subtract(normal.multiply(2).multiply(normal.dotProduct(*this)));
    return result;
}

float Vector2f::length()
{
    if (m_Dirty) {

        float ret;
        ret = sqrtf((x * x) + (y * y));
        m_Length = ret;
        m_Dirty = false;

        return ret;
    }

    else
    {

        return m_Length;

    }
}

Vector2f Vector2f::normalize()
{
    Vector2f r;

    r.x = x / length();
    r.y = y / length();

    return r;
}

Vector2f Vector2f::scale(float Milis)
{
    Vector2f r;

    r.x = x * Milis;
    r.y = y * Milis;

    return r;
}

Vector2f Vector2f::add(Vector2f v)
{
    Vector2f result(v);

    result.x += x;
    result.y += y;

    return result;
}

Vector2f Vector2f::negative()
{
    Vector2f result;

    result.x = -x;
    result.y = -y;

    return result;
}

Vector2f Vector2f::subtract(Vector2f v)
{
    Vector2f result(*this);
    result.x -= v.x;
    result.y -= v.y;

    return result;
}

float Vector2f::dotProduct(Vector2f other)
{
    float xt = x * other.x;
    float yt = y * other.y;
    return xt + yt;
}

float Vector2f::angle(Vector2f vector)
{
    Vector2f v = normalize();
    Vector2f r = vector.normalize();
    float dProduct = v.dotProduct(r);
    return acos(dProduct);
}

Vector2f Vector2f::multiply(float x)
{
    Vector2f result(*this);

    result.x = result.x * x;
    result.y = result.y * x;

    return result;
}

Vector2f Vector2f::divide(float x)
{
    Vector2f result(*this);

    result.x = result.x / x;
    result.y = result.y / x;

    return result;
}

Vector3f::Vector3f(float sx, float sy, float sz)
{

    setX(sx);
    setY(sy);
    setZ(sz);

    m_lengthDirty = true;
}

Vector3f::Vector3f(float* matrix)
{
    setMatrix(matrix);
}

Vector3f::Vector3f()
{
    m_vectorMatrix[0] = 0;
    m_vectorMatrix[1] = 0;
    m_vectorMatrix[2] = 0;

    m_lengthDirty = false;
    m_storeLength = 0;
}

float Vector3f::length()
{
    if (m_lengthDirty == false)
    {
        return m_storeLength;
    }
    else
    {
        float computedLength;
        computedLength = sqrtf((m_vectorMatrix[0] * m_vectorMatrix[0]) + (m_vectorMatrix[1] * m_vectorMatrix[1]) + (m_vectorMatrix[2] * m_vectorMatrix[2]));
        m_lengthDirty = false;
        m_storeLength = computedLength;
        return computedLength;
    }
}

Vector3f Vector3f::normalize()
{
    Vector3f normalVector(m_vectorMatrix[0] / length(), m_vectorMatrix[1] / length(), m_vectorMatrix[2] / length());
    return normalVector;
}

Vector3f Vector3f::cross(Vector3f other)
{
    float crossMatrix[3] =
    {
        (getY() * other.getZ()) - (getZ() * other.getY()),
        (getZ() * other.getX()) - (getX() * other.getZ()),
        (getX() * other.getY()) - (getY() * other.getX())
    };

    return Vector3f(crossMatrix);
}

void Vector3f::setX(float x)
{
    m_vectorMatrix[0] = x;
    m_lengthDirty = true;
}

void Vector3f::setY(float y)
{
    m_vectorMatrix[1] = y;
    m_lengthDirty = true;
}

void Vector3f::setZ(float z)
{
    m_vectorMatrix[2] = z;
    m_lengthDirty = true;
}

void Vector3f::setMatrix(float* newMatrix)
{
    m_vectorMatrix[0] = newMatrix[0];
    m_vectorMatrix[1] = newMatrix[1];
    m_vectorMatrix[2] = newMatrix[2];

    m_lengthDirty = true;
}

float Vector3f::getX()
{
    return m_vectorMatrix[0];
}

float Vector3f::getY()
{
    return m_vectorMatrix[1];
}

float Vector3f::getZ()
{
    return m_vectorMatrix[2];
}

float* Vector3f::getMatrix()
{
    return m_vectorMatrix;
}
