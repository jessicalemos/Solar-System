#include "headers/parser.h"
#include "headers/Point.h"


int readPointsFile(string filename, vector<Point*> *points, vector<Point*> *normalList, vector<float> *textureList){
    string l, t; int index,j;
    ifstream file(filename);
    string token;
    vector<float>tokens;

    int i;

    if (!file.is_open()) {
        cout << "Unable to open file: " << filename << "." << endl; return -1;
    }
    else
    {
        index = 0;
        getline(file, l);
        int numVertex = atoi(l.c_str());
        for(int i=0; i < numVertex; i++){
            getline(file,l);
            stringstream ss(l);
            j = 0;

            while(j < 3 && getline(ss,token,',')) {
                tokens.push_back(stof(token));
                j++;
            }
            Point *p = new Point(tokens[index++],tokens[index++],tokens[index++]);
            points->push_back(p);
        }

        index = 0;
        getline(file, l);
        int numNormals = atoi(l.c_str());
        for(int i=0; i < numNormals; i++){
            getline(file,l);
            stringstream ss(l);
            j = 0;

            while(j < 3 && getline(ss,token,',')) {
                tokens.push_back(stof(token));
                j++;
            }
            Point *p = new Point(tokens[index++],tokens[index++],tokens[index++]);
            normalList->push_back(p);
        }

        index = 0;
        getline(file, l);
        int numTexture = atoi(l.c_str());
        for(int i=0; i < numTexture; i++){
            getline(file,l);
            stringstream ss(l);
            j = 0;

            while(j < 2 && getline(ss,token,',')) {
                tokens.push_back(stof(token));
                j++;
            }
            textureList->push_back(token[index++]);
            textureList->push_back(token[index++]);
        }

        file.close();
    }
    return 0;
}

Group* loadXMLfile(string filename) {
    Group* group = nullptr;
    XMLDocument xmlDoc;
    XMLNode *pRoot;
    XMLElement *pElement;
    string fileDir = "../../files/" + filename;
    XMLError eResult = xmlDoc.LoadFile(fileDir.c_str());
    
    if (eResult == XML_SUCCESS)
    {
        pRoot = xmlDoc.FirstChild();
        if (pRoot != nullptr)
        {
            group = new Group();
            pElement = pRoot->FirstChildElement("group");
            parseGroup(group,pElement);
        }
    }
    else
    {
        cout << "Unable to open file: " << filename << "." << endl;
    }
    return group;
}

void parseRotate (Group* group, XMLElement* element) {
    float angle = 0, x = 0, y = 0, z = 0;
    string type = "rotation";
    Transformation *t;

     if(element->Attribute("time"))
    {
        float time = stof(element->Attribute("time"));
        angle = 360 / (time * 1000);
        type = "rotateTime";
    }
    else if(element->Attribute("angle"))
        angle = stof(element->Attribute("angle"));

    if(element->Attribute("X"))
        x = stof(element->Attribute("X"));

    if(element->Attribute("Y"))
        y = stof(element->Attribute("Y"));

    if(element->Attribute("Z"))
        z = stof(element->Attribute("Z"));

    t = new Transformation(type,angle,x,y,z);
    group->addTransformation(t);
}

void parseTranslate (Group *group, XMLElement *element) {
    float x=0, y=0, z=0, time = 0;
    vector<Point*> cPoints;
    Transformation *t;

    if (element->Attribute("time"))
    {
        bool deriv = false;
        if (element->Attribute("derivative"))
            deriv = (stoi(element->Attribute("derivative"))== 1) ? true : false;
        time = stof(element->Attribute("time"));
        time = 1 / (time * 1000);
        element = element->FirstChildElement("point");

        while (element != nullptr)
        {
            x = stof(element->Attribute("X"));
            y = stof(element->Attribute("Y"));
            z = stof(element->Attribute("Z"));

            Point *p = new Point(x,y,z);
            cPoints.push_back(p);

            element = element->NextSiblingElement("point");
        }

        t = new Transformation(time,cPoints,deriv,"translateTime");
        group->addTransformation(t);
    }
    else{   

    if(element->Attribute("X"))
        x = stof(element->Attribute("X"));

    if(element->Attribute("Y"))
        y = stof(element->Attribute("Y"));

    if(element->Attribute("Z"))
        z = stof(element->Attribute("Z"));

    t = new Transformation("translate",0,x,y,z);
    group->addTransformation(t);

  }
}

void parseScale (Group *group, XMLElement *element){
    float x=1, y=1, z=1;
    string type = "scale";
    Transformation *t;

    if(element->Attribute("X"))
        x = stof(element->Attribute("X"));

    if(element->Attribute("Y"))
        y = stof(element->Attribute("Y"));

    if(element->Attribute("Z"))
        z = stof(element->Attribute("Z"));

    t = new Transformation(type,0,x,y,z);
    group->addTransformation(t);
}

void parseColour (Group *group, XMLElement *element){
    float x=1, y=1, z=1;
    string type = "colour";
    Transformation *t;

    if(element->Attribute("R"))
        x = stof(element->Attribute("R"));

    if(element->Attribute("G"))
        y = stof(element->Attribute("G"));

    if(element->Attribute("B"))
        z = stof(element->Attribute("B"));


    t = new Transformation(type,0,x,y,z);
    group->addTransformation(t);
}

void parseModels (Group *group, XMLElement *element) {
    string file;
    vector<Shape*> shapes;

    element = element->FirstChildElement("model");

    if (element == nullptr) {
        cout << "XML error: Models are not available!" << endl;
        return;
    }

    while (element != nullptr) {

        file = element->Attribute("file");
        string fileDir = "../../files/" + file;
        if(!file.empty()) {
            vector<Point*> points;
            readPointsFile(fileDir, &points);

            if (points.size()) {
                Shape *shape = new Shape(points);
                shapes.push_back(shape);
            }
        }
        element = element->NextSiblingElement("model");
    }

    if (shapes.size())
        group->setShapes(shapes);
}

void parseGroup (Group *group, XMLElement *gElement)
{
    XMLElement *element = gElement->FirstChildElement();

    while (element)
    {
        if (strcmp(element->Name(),"translate") == 0)
            parseTranslate(group,element);

        else if (strcmp(element->Name(),"scale") == 0)
            parseScale(group,element);

        else if (strcmp(element->Name(),"rotate") == 0)
            parseRotate(group,element);

        else if (strcmp(element->Name(),"models") == 0)
            parseModels(group, element);

	else if (strcmp(element->Name(),"colour") == 0)
            parseColour(group, element);

        else if (strcmp(element->Name(),"group") == 0)
        {
            Group *child = new Group();
            group->addGroup(child);
            parseGroup(child,element);
        }

        element = element->NextSiblingElement();
    }
}

void parseMaterials (XMLElement *element, float *info, vector<int> *material)
{
    // DIFFUSE MATERIAL
    if (element->Attribute("diffR") || element->Attribute("diffG") || element->Attribute("diffB"))
    {
        material->push_back(DIFFUSE);
        if (element->Attribute("diffR"))
            info[0] = stof(element->Attribute("diffR"));
        if (element->Attribute("diffG"))
            info[1] = stof(element->Attribute("diffG"));
        if (element->Attribute("diffB"))
            info[2] = stof(element->Attribute("diffB"));
        info[3] = 1.0f;
    }

    // AMBIENT MATERIAL
    if (element->Attribute("ambR") || element->Attribute("ambG") || element->Attribute("ambB"))
    {
        material->push_back(AMBIENT);
        if (element->Attribute("ambR"))
            info[4] = stof(element->Attribute("ambR"));
        if (element->Attribute("ambG"))
            info[5] = stof(element->Attribute("ambG"));
        if (element->Attribute("ambB"))
            info[6] = stof(element->Attribute("ambB"));
        info[7] = 1.0f;
    }

    // SPECULAR MATERIAL
    if (element->Attribute("specR") || element->Attribute("specG") || element->Attribute("specB"))
    {
        material->push_back(SPECULAR);
        if (element->Attribute("specR"))
            info[8] = stof(element->Attribute("specR"));
        if (element->Attribute("specG"))
            info[9] = stof(element->Attribute("specG"));
        if (element->Attribute("specB"))
            info[10] = stof(element->Attribute("specB"));
        info[11] = 1.0f;
    }

    // EMISSION MATERIAL
    if (element->Attribute("emiR") || element->Attribute("emiG") || element->Attribute("emiB"))
    {
        material->push_back(EMISSION);
        if (element->Attribute("emiR"))
            info[12] = stof(element->Attribute("emiR"));
        if (element->Attribute("emiG"))
            info[13] = stof(element->Attribute("emiG"));
        if (element->Attribute("emiB"))
            info[14] = stof(element->Attribute("emiB"));
       info[15] = 1.0f;
    }
}                                                                                                                                                                    314,8         98%

