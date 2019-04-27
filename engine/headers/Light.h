
#ifndef ENGINE_LIGHT_H
#define ENGINE_LIGHT_H
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif

#include "Point.h"

class Light {
    private:
        bool isPoint;
        Point* point;

    public:
        Light();
        Light(bool isPoint, Point* point);
        bool getIsPoint();
        Point* getPoint();
        void setIsPoint(bool);
        void setPoint(Point*);
        void draw();
};

#endif //ENGINE_LIGHT_H
