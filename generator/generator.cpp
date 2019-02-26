#include "generator.h"
#include "figures.h"

#include <iostream>
#include <fstream>
#include <math.h>
#include <string> 
using namespace std;

void writePointsFile(string filename) {
	ofstream file;

	file.open(filename, ios_base::trunc);

	if (!file.is_open()) {
		cout << "Error while opening the file " << filename << endl;
	}
	else {
		for (const Point point : points) {
			string x, y, z;

			x = to_string(point.x).append(",");
			y = to_string(point.y).append(",");
			z = to_string(point.z).append("\n");

			file << x << y << z << endl;
		}
		file.close();
		cout << filename << " file was created!" << endl;
	}
}

int main(int argc, char **argv) {

}