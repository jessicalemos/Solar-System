#include "headers/parser.h"
#include "headers/Point.h"
#include "headers/Material.h"


int readPointsFile(string filename, vector<Point*> *points, vector<Point*> *normalList, vector<float> *textureList){
    string l, t; int index,j;
    ifstream file(filename);
    string token;
    vector<float>tokens;

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
                j++;
                textureList->push_back(stof(token));
            }
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
            pElement = pRoot->FirstChildElement("lights");
            if (pElement)
            {
                pElement->FirstChildElement();
                parseLights(group, pElement);
            }
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

/*void parseColour (Group *group, XMLElement *element){
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
}*/

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
            vector<Point*> normal;
            vector<float> texture;
            readPointsFile(fileDir, &points,&normal, &texture);
            
            if (points.size()) {
                Shape *shape;
                if(element->Attribute("texture"))
                    shape  = new Shape(element->Attribute("texture"),points, normal, texture);
                else
                    shape = new Shape(points, normal, texture);
                parseMaterial(shape, element);
                shapes.push_back(shape);
            }
        }
        element = element->NextSiblingElement("model");
    }

    if (shapes.size())
        group->setShapes(shapes);
}

void parseLights (Group *group, XMLElement *element)
{
    vector<Light*> lights;
    bool ponto;
    Light* light;
    float x=0;
    float y=0;
    float z=0;
    
    element = element->FirstChildElement();
    for(;element; element=element->NextSiblingElement())
        if(!strcmp(element->Name(),"light")){
            if(element->Attribute("type") && !strcmp(element->Attribute("type"),"POINT"))
                ponto = true;
            else ponto = false;
            
            if(element->Attribute("x"))
                x = stof(element->Attribute("x"));
            if(element->Attribute("y"))
                y = stof(element->Attribute("y"));
            if(element->Attribute("z"))
                z = stof(element->Attribute("z"));
            
            Point* p = new Point(x,y,z);
            light = new Light(ponto, p);
            lights.push_back(light);
        }
    
    group->setLights(lights);
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

	/*else if (strcmp(element->Name(),"colour") == 0)
            parseColour(group, element);*/

        else if (strcmp(element->Name(),"group") == 0)
        {
            Group *child = new Group();
            group->addGroup(child);
            parseGroup(child,element);
        }

        element = element->NextSiblingElement();
    }
}
void parseMaterial(Shape* shape, XMLElement* element) {
    Transformation* diffuse = NULL;
    Transformation* ambient = NULL;
    Transformation* specular = new Transformation(0, 0, 0);
    Transformation* emission = new Transformation(0, 0, 0);
    

    // Diffuse
    if(element->Attribute("diffR") || element->Attribute("diffG") || element->Attribute("diffB")) {
        diffuse = new Transformation(0.8f, 0.8f, 0.8f);
        if (element->Attribute("diffR"))
            diffuse->setX(stof(element->Attribute("diffR")));
        if (element->Attribute("diffG"))
            diffuse->setY(stof(element->Attribute("diffG")));
        if (element->Attribute("diffB"))
            diffuse->setZ(stof(element->Attribute("diffB")));
    }

    // Ambient
    if(element->Attribute("ambR") || element->Attribute("ambG") || element->Attribute("ambB")) {
        ambient = new Transformation(0.2f, 0.2f, 0.2f);
        if (element->Attribute("ambR"))
            ambient->setX(stof(element->Attribute("ambR")));
        if (element->Attribute("ambG"))
            ambient->setY(stof(element->Attribute("ambG")));
        if (element->Attribute("ambB"))
            ambient->setZ(stof(element->Attribute("ambB")));
    }


    // Specular
    if(element->Attribute("specR"))
        specular->setX(stof(element->Attribute("specR")));
    if(element->Attribute("specG"))
        specular->setY(stof(element->Attribute("specG")));
    if(element->Attribute("specB"))
        specular->setZ(stof(element->Attribute("specB")));

    // Emission
    if(element->Attribute("emiR"))
        emission->setX(stof(element->Attribute("emiR")));
    if(element->Attribute("emiG"))
        emission->setY(stof(element->Attribute("emiG")));
    if(element->Attribute("emiB"))
        emission->setZ(stof(element->Attribute("emiB")));

   
    Material* m = new Material(diffuse, ambient, specular, emission);
    shape->setParseMat(m);

    
}
