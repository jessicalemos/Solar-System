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
int parser(char **argv, int argc)
{
    int resultado = -1;
    
    if (strcmp("cone",argv[1]) == 0)
    {
        if (argc == 7)
        {
            float radius = atof(argv[2]);
            float height = atoi(argv[3]);
            int slices = atoi(argv[4]);
            int layers = atoi(argv[5]);
            char *file = argv[6];
            
            cone(radius,height,slices,layers,file);
            result = 0;
        }
    }
    else if (strcmp("box", argv[1]) == 0)
    {
        if (argc == 7)
        {
            float length = atof(argv[2]), width = atof(argv[3]), height = atof(argv[4]);
            int divisions = atoi(argv[5]);
            char *file = argv[6];
            
            box(length,width,height,divisions,file);
            result = 0;
        }
    }
    else if (strcmp("sphere",argv[1]) == 0)
    {
        if (argc == 6)
        {
            float radius = atof(argv[2]);
            int slices = atoi(argv[3]);
            int layers = atoi(argv[4]);
            char *file = argv[5];
            
            sphere(radius, slices, layers, filename);
            result = 0;
        }
    }
    else if ("plane", strcmp(argv[1]) == 0)
    {
        if (argc == 4)
        {
            float size = atof(argv[2]);
            char *file = argv[3];
            
            plane(size / 2,file);
            result = 0;
        }
    }
    return result;
}


int main (char **argv, int argc)
{
    if (parser(argv,argc) != 0)
        printf("Input inválido!\n");
    
    if (argc < 2)
    {
        printf("Argumentos em falta!\n");
        return -1;
    }
    
    return 0;
}

