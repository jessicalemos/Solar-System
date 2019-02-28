#ifndef FIGURES_FIGURES_H
#define FIGURES_FIGURES_H

#include <vector>

using namespace std;

typedef struct point {
	float x;
	float y;
	float z;
} Point;

float quadrants[8][3] = {
    { 1,  1,  1},
    { 1,  1, -1},
    {-1,  1, -1},
    {-1,  1,  1},
    { 1, -1,  1},
    { 1, -1, -1},
    {-1, -1, -1},
    {-1, -1,  1},
};

void box(float length, float width, float height, int nDivisions);
Point pointsCone(float radius, float angle, float height);
void cone(float radius, float height, int slices, int layers);
Point pointsSphere(float radius, float beta, float alfa);
void sphere(float radius, int slices, int layers);
void plane(float size);

#endif
