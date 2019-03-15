#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#define _USE_MATH_DEFINES
#include <math.h>
#include "headers/Camera.h"

Camera::Camera()
{
    posInitialCamera();
}

float Camera::getXPosition() { return positionX; }
float Camera::getYPosition() { return positionY; }
float Camera::getZPosition() { return positionZ; }
float Camera::getXLook() { return lookX; }
float Camera::getYLook() { return lookY; }
float Camera::getZLook() { return lookZ; }

void Camera::posInitialCamera()
{
    radius = 100.0f, speed = 0.4f;
    alpha = 4.5f, teta = 0.5f;
    mouseSense = 0.001f;

    lookX = 0; lookY = 0; lookZ = 0;
    positionX = lookX + radius*sin(alpha)*cos(teta);
    positionY = lookY + radius*sin(teta);
    positionZ = lookZ + radius*cos(alpha)*cos(teta);

    buttonIsPressed = false;
}

void Camera::changePositionLook(float x, float y, float z)
{
    posInitialCamera();
    lookX = x;
    lookY = y;
    lookZ = z;

    positionX += x;
    positionY += y;
    positionZ += z;
}

void Camera::specialKeyCamera(int key)
{
    switch (key)
    {
        case GLUT_KEY_UP:
            if (teta < (M_PI / 2 - speed))
                teta += speed;
            break;

        case GLUT_KEY_DOWN:
            if (teta > -(M_PI / 2 - speed))
                teta -= speed;
            break;

        case GLUT_KEY_LEFT:
            alpha -= speed;
            break;

        case GLUT_KEY_RIGHT:
            alpha += speed;
            break;

        case GLUT_KEY_F1:
            radius -= speed * 2;
            break;

        case GLUT_KEY_F2:
            radius += speed * 2;
            break;
        case GLUT_KEY_F6:
            posInitialCamera();
            break;
        default:
            break;
    }
    positionX = lookX + radius*sin(alpha)*cos(teta);
    positionY = lookY + radius*sin(teta);
    positionZ = lookZ + radius*cos(alpha)*cos(teta);
}

void Camera::mousePress(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON)
    {
        if (state == GLUT_DOWN)
        {
            mouseX = x;
            mouseY = y;
            buttonIsPressed = true;
        }
        else if (state == GLUT_UP)
        {
            alpha += (x - mouseX) * mouseSense;
            teta += (y - mouseY) * mouseSense;
            buttonIsPressed = false;
        }
    }
}

void Camera::mouseMotion(int xx, int yy)
{
    if (buttonIsPressed)
    {
        float alphaAux, betaAux;

        alphaAux = alpha + (xx - mouseX) * mouseSense;
        betaAux = teta + (yy - mouseY) * mouseSense;

        if (betaAux + 0.05f > M_PI/2) betaAux = M_PI/2 - 0.05f;
        else if (betaAux - 0.05f < -M_PI/2) betaAux = -M_PI/2 + 0.05f;

        positionX = lookX + radius * sin(alphaAux) * cos(betaAux);
        positionY = lookY + radius * sin(betaAux);
        positionZ = lookZ + radius * cos(alphaAux) * cos(betaAux);
    }
}
