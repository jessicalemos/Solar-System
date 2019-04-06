#ifndef __PATCH_H__
#define __PATCH_H__

#include <vector>
#include <map>
#include "Point.h"

using namespace std;

class Patch{
	int tessellation;
	vector<Point*> controlPoints;
	map<int,vector<int> > patchs;

public:
	Patch();
	Patch(vector<Point*> p);
	void multMatrixVector(float *m, float *v, float *res);
	vector<Point> getPatchPoints(int patch);
};

#endif
