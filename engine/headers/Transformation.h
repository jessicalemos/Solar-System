#ifndef TRANFORMATION_TRANSFORMATION_H
#define TRANFORMATION_TRANSFORMATION_H
#include <string>
#include "Point.h"
#include <vector>

using namespace std;

class Transformation
{
    private:
        vector<Point> controlPoints;
        string type;
        float angle;
        float **PontosCurva;
        float x;
        float y;
        float z;

    public:
        Transformation();
        Transformation(string typeT, float a, float xx, float yy, float zz);
        string getType();
        float getAngle();
        float getX();
        float getY();
        float getZ();
        void getCatmullRomPoint(float t, int *indexes, float *p, float *deriv);
        void getGlobalCatmullRomPoint(float gt, float *p, float *deriv);
        void Transformation::setCatmullPoints();
};

#endif
