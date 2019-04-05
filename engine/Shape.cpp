#include "headers/Shape.h"

Shape::Shape(){}

Shape::Shape(vector<float> vertex){
    numVertex = vertex.size() / 3;
    prepareBuffer(vertex);
}

void Shape::prepareBuffer(vector<float> vertex){
    glGenBuffers(1,&bufferVertex);
    glBindBuffer(GL_ARRAY_BUFFER, bufferVertex);
    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(float) * numVertex * 3,
                 &(vertex[0]),
                 GL_STATIC_DRAW);
}

void Shape::draw(){
    glBindBuffer(GL_ARRAY_BUFFER, bufferVertex);
    glVertexPointer(3, GL_FLOAT, 0, 0);
    glDrawArrays(GL_TRIANGLES, 0, numVertex);
}