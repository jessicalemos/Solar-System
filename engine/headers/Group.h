#ifndef GROUP_GROUP_H
#define GROUP_GROUP_H

#include "Transformation.h"
#include "Light.h"
#include "Shape.h"
#include <vector>

using namespace std;

class Group {
    private:
        vector<Group*> groups;
        vector<Transformation*> transformations;
        vector<Shape*> shapes;
        vector<Light*> lights;

    public:
        Group();
        void addTransformation(Transformation* t);
        void addGroup(Group* g);
        void setShapes(vector<Shape*> pt);
        vector<Shape*> getShapes();
        vector<Group*> getGroups();
        void setLights(vector<Light*> l);
        vector<Transformation*> getTransformations();

};

#endif
