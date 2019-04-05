#include "headers/Transformation.h"

Transformation::Transformation(){}

Transformation::Transformation(string typeT, float a, float xx, float yy, float zz){
    type = typeT;
    angle = a;
    x = xx;
    y = yy;
    z = zz;
}

string Transformation::getType(){
    return type;
}

float Transformation::getAngle(){
    return angle;
}

float Transformation::getX(){
    return x;
}

float Transformation::getY(){
    return y;
}

float Transformation::getZ(){
    return z;
}

void Transformation::getCatmullRomPoint(float t, int *indexes, float *p, float *deriv) {
    float m[4][4] = { 
    { -0.5f, 1.5f, -1.5f, 0.5f },
    { 1.0f, -2.5f, 2.0f, -0.5f },
    { -0.5f, 0.0f, 0.5f, 0.0f },
    { 0.0f, 1.0f, 0.0f, 0.0f } 
    };

    float px[4],py[4],pz[4];
    for(int i = 0; i < 4 ; i++){
        px[i] = controlPoints[indexes[i]].getX();
        py[i] = controlPoints[indexes[i]].getY();
        pz[i] = controlPoints[indexes[i]].getZ();
    }

    // Compute A = M * P
    float a[4];
    multMatrixVector(*m, px, a[0]);
    multMatrixVector(*m, py, a[1]);
    multMatrixVector(*m, pz, a[2]);

    // Compute pos = T * A
    float T[4] = { t*t*t, t*t, t, 1};
    multMatrixVector(*a, T, p);

    // Compute deriv = T' * A
    float Tdev[4] = { 3*T[1] , 2*T[2], 1 , 0};
    multMatrixVector(*a, Tdev, deriv);
}

void Transformation::getGlobalCatmullRomPoint(float gt, float *p, float *deriv) {
    int num = controlPoints.size();
    float t = gt * num;
    int index = floor(t);

    t = t - index;

    int indexes[4];
    indexes[0] = (index + num - 1)  % num;
    indexes[1] = (indexes[0]+1)     % num;
    indexes[2] = (indexes[1]+1)     % num;
    indexes[3] = (indexes[2]+1)     % num;

    getCatmullRomPoint(t, indexes, p, deriv);
}
