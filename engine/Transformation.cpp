#include "headers/Transformation.h"
//Numero de pontos para se definir a curva
#define SIZE 500

Transformation::Transformation(){}

Transformation::Transformation(string typeT, float a, float xx, float yy, float zz){
    type = typeT;
    angle = a;
    x = xx;
    y = yy;
    z = zz;
}
Transformation(float ti, vector<Point> cP, bool de, string t){
	time = ti;
	controlPoints = cP;
	setCatmullPoints();
	Deriv = de;
	t=type;
}
string Transformation::getType(){
    return type;
}

float Transformation::getAngle(){
    return angle;
}

float Transformation::getX(){
    return x;
}

float Transformation::getY(){
    return y;
}

float Transformation::getZ(){
    return z;
}

float** Transformation::getPontosCurva(){
       	return PontosCurva;
}

//normalizar vetor
void normalize(float *a) {
    float n = sqrt(a[0]*a[0] + a[1] * a[1] + a[2] * a[2]);
    a[0] = a[0]/n;
    a[1] = a[1]/n;
    a[2] = a[2]/n;
}

void cross(float *a, float *b, float *res)
{
    res[0] = a[1]*b[2] - a[2]*b[1];
    res[1] = a[2]*b[0] - a[0]*b[2];
    res[2] = a[0]*b[1] - a[1]*b[0];
}

void multMatrixVector(float *m, float *v, float *res) {
    for (int j = 0; j < 4; ++j) {
        res[j] = 0;
        for (int k = 0; k < 4; ++k) {
            res[j] += v[k] * m[j * 4 + k];
        }
    }
}

void Transformation::getCatmullRomPoint(float t, int *indexes, float *p, float *deriv) {
    float m[4][4] = { 
    { -0.5f, 1.5f, -1.5f, 0.5f },
    { 1.0f, -2.5f, 2.0f, -0.5f },
    { -0.5f, 0.0f, 0.5f, 0.0f },
    { 0.0f, 1.0f, 0.0f, 0.0f } 
    };

    float px[4],py[4],pz[4];
    for(int i = 0; i < 4 ; i++){
        px[i] = controlPoints[indexes[i]].getX();
        py[i] = controlPoints[indexes[i]].getY();
        pz[i] = controlPoints[indexes[i]].getZ();
    }

    // Compute A = M * P
    float a[4];
    multMatrixVector(*m, px, a[0]);
    multMatrixVector(*m, py, a[1]);
    multMatrixVector(*m, pz, a[2]);

    // Compute pos = T * A
    float T[4] = { t*t*t, t*t, t, 1};
    multMatrixVector(*a, T, p);

    // Compute deriv = T' * A
    float Tdev[4] = { 3*T[1] , 2*T[2], 1 , 0};
    multMatrixVector(*a, Tdev, deriv);
}

void Transformation::getGlobalCatmullRomPoint(float gt, float *p, float *deriv) {
    int num = controlPoints.size();
    float t = gt * num;
    int index = floor(t);

    t = t - index;

    int indexes[4];
    indexes[0] = (index + num - 1)  % num;
    indexes[1] = (indexes[0]+1)     % num;
    indexes[2] = (indexes[1]+1)     % num;
    indexes[3] = (indexes[2]+1)     % num;

    getCatmullRomPoint(t, indexes, p, deriv);
}

void Transformation::setCatmullPoints(){
            float ponto[4];
            float deriv[4];
	    float aux = i/(SIZE * 1.0f);

            PontosCurva = (float **) malloc(SIZE * sizeof(float *));
            for(int i = 0; i < SIZE; i++)
                PontosCurva[i] = (float *) malloc (3 * sizeof(float));

            for(int i = 0; i<SIZE; i++)
            {
                getGlobalCatmullRomPoint(aux, ponto, deriv);

		//pontos para definir a órbita
                PontosCurva[i][0] = ponto[0];
                PontosCurva[i][1] = ponto[1];
                PontosCurva[i][2] = ponto[2];
            }
        }
};

