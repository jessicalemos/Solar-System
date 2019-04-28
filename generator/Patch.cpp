#include "headers/Patch.h"

Patch::Patch(){
}

Patch::Patch(vector<Point> p){
	controlPoints = p;
}

void Patch::multMatrixVector(float *m, float *v, float *res){
    for (int j = 0; j < 4; ++j){
        res[j] = 0;
        for (int k = 0; k < 4; ++k)
        res[j] += v[k] * m[j * 4 + k];
    }
}
Patch::Patch(int tess, string filename){
            tessellation = tess;
            parserPatchFile(filename);
}

vector<Point> Patch::geradorModeloBezier(){
            vector<Point> result;

            for(int i=0; i < nPatchs; i++){
                vector<Point> aux = getPatchPoints(i);
                result.insert(result.end(),aux.begin(),aux.end());
            }
            return result;
       }

void Patch::parserPatchFile(string filename){
            string line, x,y,z;
            string fileDir = "../../files/" + filename;
            ifstream file(fileDir);

            if (file.is_open())
            {
                getline(file,line);
                nPatchs = stoi(line);
                //parsing dos indexes
                for(int i = 0; i < nPatchs; i++)
                {
                    vector<int> patchIndex;

                    if(getline(file,line))
                    {
                        char* str = strdup(line.c_str());
                        char* token = strtok(str, " ,");

                        while (token != NULL)
                        {
                            patchIndex.push_back(atoi(token));
                            token = strtok(NULL, " ,");
                        }

                        patchs[i] = patchIndex;
                        free(str);
                    }
                    else
                        cout << "Cannot get all patchIndex!" << endl;
                }

                getline(file,line);
                nPoints = stoi(line);
                //parsing das coordenadas dos pontos
                for(int i = 0; i < nPoints; i++)
                {
                    if(getline(file,line))
                    {
                        char* str = strdup(line.c_str());
                        char* token = strtok(str, " ,");

                        float x = atof(token);
                        token = strtok(NULL, " ,");
                        float y = atof(token);
                        token = strtok(NULL, " ,");
                        float z = atof(token);
                        Point *p = new Point(x,y,z);
                        controlPoints.push_back(*p);

                        free(str);
                    }
                    else
                        cout << "Cannot get all patchIndex!" << endl;
                }
                file.close();
            }
            else
                cout << "Unable to open file: " << filename << "." << endl;
}

Point* Patch::getPoint(float ta, float tb, float coordenadasX[4][4], float coordenadasY[4][4], float coordenadasZ[4][4]){
	    float x = 0.0f, y = 0.0f, z = 0.0f;
            float m[4][4] = {{-1.0f,  3.0f, -3.0f,  1.0f},
                             { 3.0f, -6.0f,  3.0f,  0.0f},
                             {-3.0f,  3.0f,  0.0f,  0.0f},
                             { 1.0f,  0.0f,  0.0f,  0.0f}};

            float a[4] = { ta*ta*ta, ta*ta, ta, 1.0f};
            float b[4] = { tb*tb*tb, tb*tb, tb, 1.0f};

            float am[4];
            multMatrixVector(*m,a,am);

            float bm[4];
            multMatrixVector(*m,b,bm);

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

float Patch::getTangent(float tu, float tv, float m[4][4] , float points[4][4], int deriv) {
    float point = 0;
    float aux[4], segAux[4];

    if(deriv){
        for(int i = 0; i<4; i++){
            aux[i] = (powf(tu,3.0)*m[0][i]) + (powf(tu,2.0)*m[1][i]) + (tu*m[2][i]) + m[3][i];
        }
    } else {
        for(int i = 0; i<4; i++){
            aux[i] = (3 * powf(tu,2.0)*m[0][i]) + (2*tu*m[1][i]) + (1*m[2][i]);
        }
    }

    //(bu*M)*P
    for(int i = 0; i<4; i++){
        segAux[i] = (aux[0]*points[0][i]) + (aux[1]*points[1][i]) + (aux[2]*points[2][i]) + (aux[3]*points[3][i]);
    }

    //((bu*M)*P)*MT
    for(int i = 0; i<4; i++){
        aux[i] = (segAux[0]*m[0][i]) + (segAux[1]*m[1][i]) + (segAux[2]*m[2][i]) + (segAux[3]*m[3][i]);
    }

    if(deriv) {
        point = aux[0] * (3 * powf(tv,2.0));
        point += aux[1] * (2 * tv);
        point += aux[2];
    } else {
        point = aux[0] * powf(tv,3.0);
        point += aux[1] * powf(tv,2.0);
        point += aux[2] * tv;
        point += aux[3];
    }

    return point;
}


void Patch::getPatchPoints(int patch, vector<Point>* points, vector<float>* textureList, vector<Point>* normalList){
    vector<int> indexesControlPoints = patchs.at(patch);

    float coordenadasX[4][4], coordenadasY[4][4], coordenadasZ[4][4];
    float u,v,uu,vv,resU[3],resV[3];
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
            float *tangenteU, *tangenteV, res[3];
            u = i*t;
            v = j*t;
            uu = (i+1)*t;
            vv = (j+1)*t;
            Point *p0,*p1,*p2,*p3,*n0,*n1,*n2,*n3,;

            p0 = getPoint(u, v, coordenadasX, coordenadasY, coordenadasZ);
            resU[0] = getTangent(u,v,coordenadasX,0);
            resU[1] = getTangent(u,v,coordenadasY,0);
            resU[2] = getTangent(u,v,coordenadasZ,0);

            resU[0] = getTangent(u,v,coordenadasX,1);
            resU[1] = getTangent(u,v,coordenadasY,1);
            resU[2] = getTangent(u,v,coordenadasZ,1);
            normalize(resU);
            normalize(resV);
            cross(resV,resU,res);
            n0 = new Point(res[0],res[1],res[2]);

            p1 = getPoint(u, vv, coordenadasX, coordenadasY, coordenadasZ);
            resU[0] = getTangent(u,vv,coordenadasX,0);
            resU[1] = getTangent(u,vv,coordenadasY,0);
            resU[2] = getTangent(u,vv,coordenadasZ,0);

            resU[0] = getTangent(u,vv,coordenadasX,1);
            resU[1] = getTangent(u,vv,coordenadasY,1);
            resU[2] = getTangent(u,vv,coordenadasZ,1);
            normalize(resU);
            normalize(resV);
            cross(resV,resU,res);
            n1 = new Point(res[0],res[1],res[2]);

            p2 = getPoint(uu, v, coordenadasX, coordenadasY, coordenadasZ);
            resU[0] = getTangent(uu,v,coordenadasX,0);
            resU[1] = getTangent(uu,v,coordenadasY,0);
            resU[2] = getTangent(uu,v,coordenadasZ,0);

            resU[0] = getTangent(uu,v,coordenadasX,1);
            resU[1] = getTangent(uu,v,coordenadasY,1);
            resU[2] = getTangent(uu,v,coordenadasZ,1);
            normalize(resU);
            normalize(resV);
            cross(resV,resU,res);
            n2 = new Point(res[0],res[1],res[2]);

            p3 = getPoint(uu, vv, coordenadasX, coordenadasY, coordenadasZ);
            resU[0] = getTangent(uu,vv,coordenadasX,0);
            resU[1] = getTangent(uu,vv,coordenadasY,0);
            resU[2] = getTangent(uu,vv,coordenadasZ,0);

            resU[0] = getTangent(uu,vv,coordenadasX,1);
            resU[1] = getTangent(uu,vv,coordenadasY,1);
            resU[2] = getTangent(uu,vv,coordenadasZ,1);
            normalize(resU);
            normalize(resV);
            cross(resV,resU,res);
            n3 = new Point(res[0],res[1],res[2]);

            (*points).push_back(*p0); points.push_back(*p2); points.push_back(*p1);
            (*points).push_back(*p1); points.push_back(*p2); points.push_back(*p3);
            (*normalList).push_back(*p0); points.push_back(*p2); points.push_back(*p1);
            (*normalList).push_back(*p1); points.push_back(*p2); points.push_back(*p3);

            (*textureList).push_back(1-u); textureList->push_back(1-v);
            (*textureList).push_back(1-uu); textureList->push_back(1-v);
            (*textureList).push_back(1-u); textureList->push_back(1-vv);
            (*textureList).push_back(1-u); textureList->push_back(1-vv);
            (*textureList).push_back(1-uu); textureList->push_back(1-v);
            (*textureList).push_back(1-uu); textureList->push_back(1-vv);
        }
    }
}
