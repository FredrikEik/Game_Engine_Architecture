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
    void makeVertices();
    MeshComponent* getMesh(int i);
    CollisionComponent* getColli(int i);
    int getCount(){return mCounter;};
    void addCount(){mCounter++;};
private:
    void readFile(std::string filename, MeshComponent* m);
    void subDivide(const gsl::Vector3D &a, const gsl::Vector3D &b, const gsl::Vector3D &c, int n);
    std::map<string, int> myObjs;
    std::vector<MeshComponent*> myMeshes;
    std::vector<CollisionComponent*> myCollis;
    int mCounter = 1;
    bool doOnce[6]{false};
    string monkeyString = "../GEA2021/Assets/Monkey.obj";
    string pacmanString = "../GEA2021/Assets/Pacman.obj";
    string enemyString = "../GEA2021/Assets/Monkey.obj";

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
    ~Square() {}
    void CheckPlayerCol(VisualObject* p);
    bool onRwallX{false};
    bool onLwallX{false};
    bool onFwallY{false};
    bool onBwallY{false};
    void noCol(){onRwallX = false;onLwallX = false;onFwallY = false;onBwallY = false;};
};

class Plain : public VisualObject
{
public:
    Plain();
    ~Plain(){};
};

class ParticleMesh : public VisualObject
{
public:
    ParticleMesh();
    ~ParticleMesh(){};
};



class ObjMesh : public VisualObject
{
public:
    ObjMesh();
    ~ObjMesh(){};
};
#endif // SHAPEFACTORY_H
