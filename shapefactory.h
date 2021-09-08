#ifndef SHAPEFACTORY_H
#define SHAPEFACTORY_H

#include <iostream>
#include <string>
#include "visualobject.h"
#include "vector3d.h"

class Shape : public VisualObject{
public:
    virtual void init() override;
    virtual ~Shape() {}

};

class ShapeFactory : public Shape
{
public:
    void createShapes(int id);
    virtual ~ShapeFactory();
private:
    Shape* myShapes[3];
    bool circleExist = false;
    bool squareExist = false;
    bool triangleExist = false;

};

class Circle : public Shape {

public:
    Circle(int n=0);
    ~Circle() { }
    Shape *create(){return new Circle;};
    friend class ShapeFactory;

private:
    int m_rekursjoner;
    int m_indeks;               // brukes i rekursjon, til å bygge m_vertices
    void makeTriangel(gsl::Vector3D& v1, gsl::Vector3D& v2, gsl::Vector3D& v3);
    void subDivide(gsl::Vector3D& a, gsl::Vector3D& b, gsl::Vector3D& c, int n);
    void oktaederUnitBall();
};

class Square : public Shape {

public:
    Square();
    ~Square() { }
    Shape* create(){return new Square;};
    friend class ShapeFactory;
};

class Triangle : public Shape {

public:
    Triangle();
    ~Triangle() {  }
    Shape* create(){return new Triangle;};
    friend class ShapeFactory;

};

#endif // SHAPEFACTORY_H
