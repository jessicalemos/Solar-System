#include "headers/Patch.h"

Patch::Patch(){
}

Patch::Patch(vector<Point*> p){
	controlPoints = p;
}

void Patch::multMatrixVector(float *m, float *v, float *res){
    for (int j = 0; j < 4; ++j){
        res[j] = 0;
        for (int k = 0; k < 4; ++k)
        res[j] += v[k] * m[j * 4 + k];
    }
}

vector<Point> getPatchPoints(int patch){
    vector<Point> points;
    vector<int> indexesControlPoints = patchs.at(patch);

    float coordenadasX[4][4], coordenadasY[4][4], coordenadasZ[4][4];
    float u,v,uu,vv;
    float t = 1.0f /tessellation;
    int pos = 0;

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            Point controlPoint = controlPoints[indexesControlPoints[pos]];
            coordenadasX[i][j] = controlPoint.getX();
            coordenadasY[i][j] = controlPoint.getY();
            coordenadasZ[i][j] = controlPoint.getZ();
            pos++;
        }
    }


    for(int i = 0; i < tessellation; i++)
    {
        for (int j = 0; j < tessellation; j++)
        {
            u = i*t;
            v = j*t;
            uu = (i+1)*t;
            vv = (j+1)*t;
            Point *p0,*p1,*p2,*p3;
            p0 = getPoint(u, v, coordenadasX, coordenadasY, coordenadasZ);
	    p1 = getPoint(u, vv, coordenadasX, coordenadasY, coordenadasZ);
            p2 = getPoint(uu, v, coordenadasX, coordenadasY, coordenadasZ);
            p3 = getPoint(uu, vv, coordenadasX, coordenadasY, coordenadasZ);

            points.push_back(*p0); points.push_back(*p2); points.push_back(*p1);
            points.push_back(*p1); points.push_back(*p2); points.push_back(*p3);
        }
    }
    return points;
}
