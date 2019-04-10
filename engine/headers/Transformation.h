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
	    bool deriv;
        float angle;
	    float time;
        vector<Point*> pointsCurve;
        float x;
        float y;
        float z;

    public:
        Transformation();
        Transformation(string typeT, float a, float xx, float yy, float zz);
        Transformation(float ti, vector<Point*> cP, bool de, string t);
        string getType();
        float getAngle();
        float getX();
        float getY();
        float getZ();
        float getTime();
        vector<Point*> getPointsCurve();
        void getCatmullRomPoint(float t, int *indexes, float *p, float *deriv);
        void getGlobalCatmullRomPoint(float gt, float *p, float *deriv);
        void renderCatmullRomCurve(vector<Point*> points);
	    void orbitaCatmullRom(float gt);
};

#endif
