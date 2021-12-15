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

class XYZ : public VisualObject
{
public:
    XYZ();
    ~XYZ() override{};
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
    ~Square(){}
};

class Plain : public VisualObject
{
public:
    Plain();
    ~Plain(){};
};

class Heart : public VisualObject
{
public:
    Heart();
    ~Heart(){};
};

class ObjMesh : public VisualObject
{
public:
    ObjMesh();
    ~ObjMesh(){};
};

/**
   ShapeFactory er vår Ressurshåndterer.
   Her lager og henter vi Mesher. Vi kan få tak i objekter ved å bruke createShape().
   Vi gjenbruker MeshComponent på like objekter.
   @param string shapeName - Skriv hvilken shape du vil ha i parameter som string.
   @return VisualObject* - Returnerer shape som VisualObject peker.
 */

class ShapeFactory{
public:

    virtual ~ShapeFactory() {}
    VisualObject* createShape(string shapeName);
    void makeVertices();

    MeshComponent* getMesh(int i);
    CollisionComponent* getColli(int i);
    MeshComponent* getParticleMesh();
    gsl::Vector3D mColor{1,1,1};
    int getCount(){return mCounter;};
    void addCount(){mCounter++;};

private:

    void readFile(std::string filename, MeshComponent* m);
    void subDivide(const gsl::Vector3D &a, const gsl::Vector3D &b, const gsl::Vector3D &c, int n);

    std::vector<MeshComponent*> myMeshes;
    std::vector<CollisionComponent*> myCollis;
    bool doOnce[6]{false};
    int mCounter = 1;

    int objCounter=6;
    std::map<string, int> myObjs;
    string monkeyString = "../GEA2021/Assets/Monkey.obj";
    string pacmanString = "../GEA2021/Assets/Pacman.obj";
    string enemyString = "../GEA2021/Assets/Enemy.obj";

};




#endif // SHAPEFACTORY_H
