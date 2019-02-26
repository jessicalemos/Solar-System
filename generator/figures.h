#ifndef FIGURES_FIGURES_H
#define FIGURES_FIGURES_H

#include <vector>

typedef struct point {
	float x;
	float y;
	float z;
} Point;

void box(float length, float width, float height, int nDivisions);
void changeSize(int w, int h);
void drawPrimitives();
Point pointsCone(float radius, float angle, float height);
void cone(float radius, float height, int slices, int layers);

#endif