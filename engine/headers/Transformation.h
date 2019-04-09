#ifndef TRANFORMATION_TRANSFORMATION_H
#define TRANFORMATION_TRANSFORMATION_H
#include <string>
#include "Point.h"
#include <vector>

using namespace std;

class Transformation
{
    private:
        vector<Point*> controlPoints;
        string type;
	    bool Deriv;
        float angle;
	    float time;
        vector<Point*> PointsCurve;
        float x;
        float y;
        float z;

    public:
        Transformation();
        Transformation(string typeT, float a, float xx, float yy, float zz);
        Transformation(float ti, vector<Point> cP, bool de, string t);
        string getType();
        float getAngle();
        float getX();
        float getY();
        float getZ();
        float getTime();
        void getCatmullRomPoint(float t, int *indexes, float *p, float *deriv);
        void getGlobalCatmullRomPoint(float gt, float *p, float *deriv);
        vector<Point*> PointsCurve();
        void renderCatmullRomCurve();
};

#endif
