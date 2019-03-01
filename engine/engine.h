#ifndef ENGINE_ENGINE_H
#define ENGINE_ENGINE_H

#define _USE_MATH_DEFINES
#include <math.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>

using namespace std;

typedef struct point {
	float x;
	float y;
	float z;
}Point;

float alpha = 0.7f, beta = 0.3f, radius = 10.0f, step = 0.08f;

void drawPrimitives();
int readPointsFile(string filename);
void specialKey (int key, int a, int b);
void renderScene(void);
void changeSize(int w, int h);


#endif //ENGINE_ENGINE_H
