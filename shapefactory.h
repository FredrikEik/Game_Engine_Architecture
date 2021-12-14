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

/**
*ShapeFactory -  Har/Lager alle mesher til objekter.
*shapefactory har ressurshåndteringen vår.
*createShape - Lager new objekter, setter objectname, id, mesh og kollisjon box/sphere.
*getMesh/Colli - returnerer mesh og collision ID.
 */

using namespace std;

class ShapeFactory{
public:
    virtual ~ShapeFactory() {}
    VisualObject* createShape(string shapeName);
    void makeVertices();
    MeshComponent* getMesh(int i);
    CollisionComponent* getColli(int i);
    int getCount(){return mCounter;};
    void addCount(){mCounter++;};
    gsl::Vector3D mColor;
    MeshComponent *getParticleMesh();
private:

    void readFile(std::string filename, MeshComponent* m);
    void subDivide(const gsl::Vector3D &a, const gsl::Vector3D &b, const gsl::Vector3D &c, int n);
    std::map<string, int> myObjs;
    std::vector<MeshComponent*> myMeshes;
    std::vector<CollisionComponent*> myCollis;
    int mCounter = 1;
    int objCounter=6;
    bool doOnce[6]{false};
    string monkeyString = "../GEA2021/Assets/Monkey.obj";
    string pacmanString = "../GEA2021/Assets/Pacman.obj";
    string enemyString = "../GEA2021/Assets/Enemy.obj";

};

class XYZ : public VisualObject
{
public:
    XYZ();
    ~XYZ() override{};
};

class Heart : public VisualObject
{
public:
    Heart();
    ~Heart(){};
};

class Circle : public VisualObject
{
public:
    Circle();
    ~Circle(){}
};

class Square : public VisualObject
{
public:
    Square();
    ~Square() {};
};

class Plain : public VisualObject
{
public:
    Plain();
    ~Plain(){};
};


class ObjMesh : public VisualObject
{
public:
    ObjMesh();
    ~ObjMesh(){};
};
#endif // SHAPEFACTORY_H
