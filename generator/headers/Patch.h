#ifndef __PATCH_H__
#define __PATCH_H__

#include <vector>

#include "Point.h"

using namespace std;

class Patch{

	vector<Point*> controlPoints;

public:
	Patch();
	Patch(vector<Point*> p);
	void multMatrixVector(float *m, float *v, float *res);
};

#endif
