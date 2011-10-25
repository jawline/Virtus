#include "Camera.h"
#include "../GRender.h"

Camera::Camera()
{
    float BlankMatrix[3] =
    {
        0, 0, 0
    };

    Position.setMatrix(BlankMatrix);
    Rotation.setMatrix(BlankMatrix);
}

Camera::~Camera()
{

}

void Camera::setCameraInternals(float angle, float ratio, float near, float far)
{

    Camera::angle = angle;
    Camera::ratio = ratio;

}

void Camera::setPosition(float x, float y, float z)
{
    //Assemble a new position matrix
    float positionMatrix[3] =
    {
        x, y, z
    };

    Position.setMatrix(positionMatrix);
}

void Camera::move(float x, float y, float z)
{
    float* currentMatrix = Position.getMatrix();

    //Assemble a new position matrix by incrementing the values on the old matrix
    float newPositionMatrix[3] =
    {
        currentMatrix[0] + x, currentMatrix[1] + y, currentMatrix[2] + z
    };

    Position.setMatrix(newPositionMatrix);
}

void Camera::setRotation(float x, float y, float z)
{
    //Assemble a new rotation matrix
    float rotationMatrix[3] =
    {
        x, y, z
    };

    Rotation.setMatrix(rotationMatrix);
}

void Camera::rotate(float x, float y, float z)
{

    float* currentMatrix = Rotation.getMatrix();

    //Assemble a new position matrix by incrementing the values on the old matrix
    float newRotationMatrix[3] =
    {
        currentMatrix[0] + x, currentMatrix[1] + y, currentMatrix[2] + z
    };

    Rotation.setMatrix(newRotationMatrix);

}

void Camera::renderMoveTo()
{
    float* positionMatrix = Position.getMatrix();
    float* rotationMatrix = Rotation.getMatrix();

    GRenderer::getRenderer()->getGraphicsAPI()->rotateMatrix( rotationMatrix[0], rotationMatrix[1], rotationMatrix[2] );
    GRenderer::getRenderer()->getGraphicsAPI()->translateMatrix( positionMatrix[0] , positionMatrix[1] , positionMatrix[2] );
}
