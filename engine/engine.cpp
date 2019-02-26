#include <stdlib.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#define _USE_MATH_DEFINES

#include "engine.h"

using namespace std;

void drawPrimitives(vector<Point> points) {
    glBegin(GL_TRIANGLES);
    for (const Point pt : points)  {
        glVertex3f(pt.x, pt.y, pt.z);
    }
    glEnd();
}

void specialKey (int key, int a, int b)
{
    (void)a;(void)b;
    switch (key)
    {
        case GLUT_KEY_UP:
            if (beta < (M_PI / 2 - step))
                beta += step;
            break;

        case GLUT_KEY_DOWN:
            if (beta > -(M_PI / 2 - step))
                beta -= step;
            break;

        case GLUT_KEY_LEFT:
            alpha -= step;
            break;

        case GLUT_KEY_RIGHT:
            alpha += step;
            break;

        case GLUT_KEY_F1:
            radius -= step;
            break;

        case GLUT_KEY_F2:
            radius += step;
            break;

        default:
            return;
    }
    glutPostRedisplay();
}

void renderScene(void)
{
    // bg color
    glClearColor(1.0, 1.0, 1.0, 1.0);

    // clear buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // set the camera
    glLoadIdentity();
    gluLookAt(radius*cos(beta)*sin(alpha), radius*sin(beta), radius*cos(beta)*cos(alpha),
              0.0, 0.0, 0.0,
              0.0f, 1.0f, 0.0f);
    drawPrimitives(points);

    // End of frame
    glutSwapBuffers();
}

void changeSize(int w, int h)
{
    // Prevent a divide by zero, when window is too short
    // (you cant make a window with zero width).
    if(h == 0)
        h = 1;

    // compute window's aspect ratio
    float ratio = w * 1.0 / h;

    // Set the projection matrix as current
    glMatrixMode(GL_PROJECTION);
    // Load Identity Matrix
    glLoadIdentity();

    // Set the viewport to be the entire window
    glViewport(0, 0, w, h);

    // Set perspective
    gluPerspective(45.0f ,ratio, 1.0f ,1000.0f);

    // return to the model view matrix mode
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char **argv)
{

    // put GLUT init here
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100,100);
    glutInitWindowSize(800,800);

    // put callback registration here
    glutDisplayFunc(renderScene);
    glutReshapeFunc(changeSize);
    glutIdleFunc(renderScene);
    glutSpecialFunc(specialKey);

    // OpenGL settings
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    // enter GLUT's main loop
    glutMainLoop();

    return 1;
}