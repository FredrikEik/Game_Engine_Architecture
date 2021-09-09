#ifndef SHAPEFACTORY_H
#define SHAPEFACTORY_H

#include <iostream>
#include "vertex.h"
#include "shader.h"
#include <vector>
#include "gltypes.h"

#include "visualobject.h"

using namespace std;

class ShapeFactory{
public:
    virtual ~ShapeFactory() {}
    VisualObject* createShape(string shapeName);
private:
    void createCircle();
    void createSquare();
    void createTriangle();
    void createPlain();
    VisualObject* myShapes[4];
};

class Circle : public VisualObject {
public:
    Circle();
    ~Circle(){}
private:
    void subDivide(const gsl::Vector3D &a, const gsl::Vector3D &b, const gsl::Vector3D &c, int n);
    void circleUnit();
    void makeTriangle(const gsl::Vector3D& v1, const gsl::Vector3D& v2, const gsl::Vector3D& v3);
    int m_rekursjoner;
    int m_index;
};

class Square : public VisualObject {
public:
    Square();
    ~Square() {}
};

class Triangle : public VisualObject {
public:
    Triangle();
    ~Triangle() {};
};

class Plain : public VisualObject {
public:
    Plain();
    ~Plain() {};
};


#endif //  SHAPEFACTORY_H
