#ifndef SHAPEFACTORY_H
#define SHAPEFACTORY_H
#include <iostream>
#include "vertex.h"
#include "shader.h"
#include "component.h"
#include <vector>
#include "gltypes.h"

#include "visualobject.h"

using namespace std;

class Shape : public VisualObject {
public:
    Shape();
    ~Shape(){};
    virtual void init() override;
};

class ShapeFactory : public Shape{
public:
    virtual ~ShapeFactory() {}
    void createShape(int id);
private:
    Shape* myShapes[3];
    bool circleExist = false;
    bool squareExist = false;
    bool triangleExist = false;

};

class Circle : public Shape {
    Circle();
public:
    ~Circle(){}
    friend class ShapeFactory;
private:
    void subDivide(const gsl::Vector3D &a, const gsl::Vector3D &b, const gsl::Vector3D &c, int n);
    void circleUnitBall();
    void makeTriangle(const gsl::Vector3D& v1, const gsl::Vector3D& v2, const gsl::Vector3D& v3);
    int m_rekursjoner;
    int m_index;
};

class Square : public Shape {
    Square(int n=0);
public:
    ~Square() {}
    friend class ShapeFactory;
};

class Triangle : public Shape {
    Triangle();
public:
    ~Triangle() {};
    friend class ShapeFactory;
};


#endif // SHAPEFACTORY_H
