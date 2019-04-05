#ifndef ENGINE_SHAPE_H
#define ENGINE_SHAPE_H

#include "Point.h"
#include <vector>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif

using namespace std;

class Shape {
    private:
        int numVertex;
        GLuint bufferVertex;

    public:
        Shape();
        Shape(vector<float> vertex);
        void prepareBuffer(vector<float> vertex);
        void draw();
};


#endif //ENGINE_SHAPE_H
