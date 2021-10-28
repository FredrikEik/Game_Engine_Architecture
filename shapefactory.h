#ifndef SHAPEFACTORY_H
#define SHAPEFACTORY_H
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include "vertex.h"
#include "shader.h"
#include "gltypes.h"

#include "visualobject.h"

using namespace std;

class ShapeFactory{
public:
    virtual ~ShapeFactory() {}
    VisualObject* createShape(string shapeName);
//    VisualObject* createMonkey();
private:
    std::map<string, int> myObjs;
    int ObjStartID;
    std::vector<VisualObject*> myShapes;
//    VisualObject* myMonkey;
//    string monkeyString = "../GEA2021/Assets/Monkey.obj";
//    string pacmanString = "../GEA2021/Assets/Pacman.obj";
//    bool doOnce = false;
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

class Line : public VisualObject
{
public:
        Line(gsl::Vector3D &startIn, gsl::Vector3D endIn, float lenght, gsl::Vector3D colorIn);
        ~Line() {};
private:
};
#endif // SHAPEFACTORY_H
