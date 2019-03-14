#ifndef __PARSER_H__
#define __PARSER_H__

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

#include "../tinyxml2.h"
#include "Point.h"
#include "Group.h"
#include "Shape.h"
#include <vector>

using namespace std;
using namespace tinyxml2;

int loadXMLfile(string filename, vector<Point*> *points);


#endif
