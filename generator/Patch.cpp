#include "headers/Patch.h"

Patch::Patch(){
}

Patch::Patch(vector<Point*> p){
	controlPoints = p;
}

vector<Point*> Patch::getControlPoints(){
  return controlPoints;
}

void Patch::multMatrixVector(float *m, float *v, float *res){
    for (int j = 0; j < 4; ++j){
        res[j] = 0;
        for (int k = 0; k < 4; ++k)
        res[j] += v[k] * m[j * 4 + k];
    }
}
