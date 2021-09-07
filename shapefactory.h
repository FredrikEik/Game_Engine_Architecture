#ifndef SHAPEFACTORY_H
#define SHAPEFACTORY_H

#include <iostream>
#include <string>
#include "visualobject.h"

class ShapeFactory
{
public:
    ShapeFactory();
};

class Shape : public VisualObject{
public:
    virtual void init() override;
    virtual ~Shape() {}
};

class Circle : public Shape {

public:
    Circle();
    ~Circle() { }
    Shape* create(){return new Circle;};
};

class Square : public Shape {

public:
     Square();
    ~Square() { }
     Shape* create(){return new Square;};
};

class Triangle : public Shape {

public:
     Triangle();
    ~Triangle() {  }
     Shape* create(){return new Triangle;};

};

#endif // SHAPEFACTORY_H
