#ifndef __MATERIAL_H__
#define __MATERIAL_H__

#include <GLUT/glut.h>
#include <iostream>

#include "Colour.h"


using namespace std;

class Material{
    
    float diffuse[4];     // default: {0.8, 0.8, 0.8, 1};
    float ambient[4];     // default: {0.2, 0.2, 0.2, 1};
    float specular[4];     // default: {0, 0, 0, 1};
    float emission[4];     // default: {0, 0, 0, 1};
    
public:
    Material();
    Material::Material(Point* d, Point* a, Point* s, Point* e);
    void draw();
    float* Material::getDiffuse();
    float* Material::getAmbient();
    float* Material::getSpecular();
    float* Material::getEmission();
};

#endif
