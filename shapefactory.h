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

//class ShapeFactory {
//    virtual Shape* create() = 0;
//public:
//    virtual ~ShapeFactory() {}
//};

class Circle : public Shape {
public:
    Circle();
    //void erase();
    ~Circle(){}
    Shape* create(){ return new Circle; };
};

class Square : public Shape {
public:
    Square();
    //void erase()
    ~Square() {}
    Shape* create() { return new Square; }
};

class Triangle : public Shape {
public:
    Triangle();
    ~Triangle() {};
    Shape* create() { return new Triangle; }
};


#endif // SHAPEFACTORY_H
