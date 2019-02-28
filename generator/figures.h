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
Point pointsSphere(float radius, float beta, float alfa);
void drawSphere(float radius, int slices, int layers);
void draw_plane(float size);

#endif