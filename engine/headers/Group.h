#ifndef GROUP_GROUP_H
#define GROUP_GROUP_H

#include "Transformation.h"
#include <vector>

using namespace std;

class Group {
    private:
        vector<Group*> groups;
        vector<Transformation*> transformations;

    public:
        Group();
        void addTransformation(Transformation* t);
        void addGroup(Group* g);

};

#endif
