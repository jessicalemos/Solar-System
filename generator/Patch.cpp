#include "headers/Patch.h"

Patch::Patch(){
}

Patch::Patch(vector<Point*> p){
	controlPoints = p;
}

void Patch::multMatrixVector(float *m, float *v, float *res){
    for (int j = 0; j < 4; ++j){
        res[j] = 0;
        for (int k = 0; k < 4; ++k)
        res[j] += v[k] * m[j * 4 + k];
    }
}


Point* getPoint(float ta, float tb, float coordenadasX[4][4], float coordenadasY[4][4], float coordenadasZ[4][4]){
	    float x = 0.0f, y = 0.0f, z = 0.0f;
            float m[4][4] = {{-1.0f,  3.0f, -3.0f,  1.0f},
                             { 3.0f, -6.0f,  3.0f,  0.0f},
                             {-3.0f,  3.0f,  0.0f,  0.0f},
                             { 1.0f,  0.0f,  0.0f,  0.0f}};

            float a[4] = { pow(ta,3), pow(ta,2), ta, 1.0f};
            float b[4] = { pow(tb,3), pow(tb,2), tb, 1.0f};

	    //multiplicar matriz m por vetor a e b
            float am[4];
            multMatrixVector(*m,a,am);

            float bm[4];
            multMatrixVector(*m,b,bm);

	    //multiplicar resultado obtido anteriormente por cada componente X,Y,Z de cada ponto controlo
            float amCoordenadaX[4], amCoordenadaY[4], amCoordenadaZ[4];
            multMatrixVector(*coordenadasX,am,amCoordenadaX);
            multMatrixVector(*coordenadasY,am,amCoordenadaY);
            multMatrixVector(*coordenadasZ,am,amCoordenadaZ);

            //
            for (int i = 0; i < 4; i++)
            {
                x += amCoordenadaX[i] * bm[i];
                y += amCoordenadaY[i] * bm[i];
                z += amCoordenadaZ[i] * bm[i];
            }

            Point *p = new Point(x,y,z);
            return p;
        }


vector<Point> getPatchPoints(int patch){
    vector<Point> points;
    vector<int> indexesControlPoints = patchs.at(patch);

    float coordenadasX[4][4], coordenadasY[4][4], coordenadasZ[4][4];
    float u,v,uu,vv;
    float t = 1.0f /tessellation;
    int pos = 0;

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            Point controlPoint = controlPoints[indexesControlPoints[pos]];
            coordenadasX[i][j] = controlPoint.getX();
            coordenadasY[i][j] = controlPoint.getY();
            coordenadasZ[i][j] = controlPoint.getZ();
            pos++;
        }
    }


    for(int i = 0; i < tessellation; i++)
    {
        for (int j = 0; j < tessellation; j++)
        {
            u = i*t;
            v = j*t;
            uu = (i+1)*t;
            vv = (j+1)*t;
            Point *p0,*p1,*p2,*p3;
            p0 = getPoint(u, v, coordenadasX, coordenadasY, coordenadasZ);
	    p1 = getPoint(u, vv, coordenadasX, coordenadasY, coordenadasZ);
            p2 = getPoint(uu, v, coordenadasX, coordenadasY, coordenadasZ);
            p3 = getPoint(uu, vv, coordenadasX, coordenadasY, coordenadasZ);

            points.push_back(*p0); points.push_back(*p2); points.push_back(*p1);
            points.push_back(*p1); points.push_back(*p2); points.push_back(*p3);
        }
    }
    return points;
}
