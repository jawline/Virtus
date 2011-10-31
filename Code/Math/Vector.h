#ifndef _VECTOR_DEF_H_
#define _VECTOR_DEF_H_

/**
 * @brief A vector with 2 points
 *
 */
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

/**
 * @brief A vector with 3 points
 *
 */

class Vector3f {
private:

    float m_storeLength;
    bool m_lengthDirty;

    float m_vectorMatrix[3];
public:

    /**
     * @brief Initialize a blank vector(0, 0, 0)
     */
    Vector3f();

    /**
     * @brief Initialize a new vector with the specified values
     */
    Vector3f(float x, float y, float z);

    /**
     * @brief Initialize a vector from the specified float array
     */
    Vector3f(float* matrix);

    /**
     * @brief Set the X value
     */
    void setX(float x);

    /**
     * @brief Set the Y value
     */
    void setY(float y);

    /**
     * @brief Set the Z value
     */
    void setZ(float z);

    /**
     * @brief Set the values to values on the specified float array
     */
    void setMatrix(float* newMatrix);

    /**
     * @brief Return the X value of the vector
     */
    float getX();

    /**
     * @brief Return the Y value of the vector
     */
    float getY();

    /**
     * @brief Get the Z value of the vector
     */
    float getZ();

    /**
     * @brief Get a reference to the raw array
     */
    float* getMatrix();

    /**
     * @brief Calculate the length of the vector
     */
    float length();

    /**
     * @brief Return a new vector containing the normalization of this one
     */

    Vector3f normalize();

    /**
     * @brief Return the cross product of this vector with another
     */

    Vector3f cross(Vector3f other);
};

/**
 * @brief Define point2f_t as a Vector2f
 */
typedef Vector2f point2f_t;

/**
 * @brief Define point3f_t as a Vector3f
 */
typedef Vector3f point3f_t;

#endif //_VECTOR_DEF_H_
