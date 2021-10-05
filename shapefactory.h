#ifndef SHAPEFACTORY_H
#define SHAPEFACTORY_H
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include "vertex.h"
#include "shader.h"
#include "gltypes.h"

#include "visualobject.h"

using namespace std;

class ShapeFactory{
public:
    virtual ~ShapeFactory() {}
    VisualObject* createShape(string shapeName);
    VisualObject* createMonkeys(int i);
private:
    void createCircle();
    void createSquare();
    void createTriangle();
    void createPlain();
    void createObj();
    VisualObject* myShapes[5];
    VisualObject* myMonkeys[200];
    string monkeyString = "../GEA2021/Assets/Monkey.obj";
};

class Circle : public VisualObject
{
public:
    Circle();
    void makeVerticies();
    ~Circle(){}
private:
    void subDivide(const gsl::Vector3D &a, const gsl::Vector3D &b, const gsl::Vector3D &c, int n);
    void circleUnit(CollisionComponent* dCollision);
    void makeTriangle(const gsl::Vector3D& v1, const gsl::Vector3D& v2, const gsl::Vector3D& v3);
    int m_rekursjoner;
    int m_index;

};

class Square : public VisualObject
{
public:
    Square();
    void makeVerticies(MeshComponent* dMesh, CollisionComponent* dCollision);
    ~Square() {}
};

class Triangle : public VisualObject
{
public:
    Triangle();
    void makeVerticies();
    ~Triangle() {};
};

class Plain : public VisualObject
{
public:
    Plain();
    void makeVerticies(MeshComponent* dMesh);
    ~Plain(){};
};

class ObjMesh : public VisualObject
{
public:
    ObjMesh(std::string filename);
    ~ObjMesh(){};
};

#endif // SHAPEFACTORY_H
