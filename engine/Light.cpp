#include "headers/Light.h"

Light::Light(){}

Light::Light(bool isPoint, Point* point){
    isPoint = isPoint;
    point = point;
}

bool Light::getIsPoint(){
    return isPoint;
}

Point* Light::getPoint(){
    return point;
}

void Light::setIsPoint(bool type){
    isPoint = type;
}

void Light::setPoint(Point* p){
    point = p;
}

void Light::draw() {

    GLfloat pos[4] = {point->getX(), point->getY(), point->getZ(), (float) isPoint};
    GLfloat amb[4] = {0.1, 0.1, 0.1, 1};
    GLfloat diff[4] = {1, 1, 1, 0};

    glLightfv(GL_LIGHT0, GL_POSITION, pos);
    glLightfv(GL_LIGHT0, GL_AMBIENT, amb);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diff);
}
