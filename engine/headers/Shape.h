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
        int numVertex[3];
        GLuint buffer[3];
        GLuint text;

    public:
        Shape();
        Shape(vector<Point*> vertex, vector<Point*> normal, vector<Point*> texture);
        Shape(string textureFile, vector<Point*> vertex, vector<Point*> normal, vector<Point*> texture);
        void loadTexture(string textureFile);
        GLuint* getBuffer();
        GLuint getTexture();
        void setTexture(GLuint text);
        void prepareBuffer(vector<Point*> vertex, vector<Point*> normal, vector<Point*> texture);
        void draw();
};


#endif //ENGINE_SHAPE_H
