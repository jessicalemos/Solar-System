#include "../../Material.h"

float diffuse[4], ambient[4], specular[4], emission[4];

Material::Material(){}

Material::Material(Point* d, Point* a, Point* s, Point* e){
    diffuse[0] = d->getX();
    diffuse[1] = d->getY();
    diffuse[2] = d->getZ();
    diffuse[3] = 1;
    
    ambient[0] = a->getX();
    ambient[1] = a->getY();
    ambient[2] = a->getZ();
    ambient[3] = 1;
    
    specular[0] = s->getX();
    specular[1] = s->getY();
    specular[2] = s->getZ();
    specular[3] = 1;
    
    emission[0] = e->getX();
    emission[1] = e->getY();
    emission[2] = e->getZ();
    emission[3] = 1;
}

float* Material::getDiffuse(){
    return diffuse;
}

float* Material::getAmbient(){
    return ambient;
}

float* Material::getSpecular(){
    return specular;
}

float* Material::getEmission(){
    return emission;
}

void Material::draw() {
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, emission);
}
