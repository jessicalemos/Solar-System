#ifndef GENERATOR_GENERATOR_H
#define GENERATOR_GENERATOR_H

#include <vector>

using namespace std;

typedef struct point{
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
int parseInput();

#endif