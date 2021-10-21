#ifndef RESOURCESYSTEM_H
#define RESOURCESYSTEM_H

#include <QDebug>

#include <iostream>
#include <fstream>
#include <ostream>
#include <string>
#include <istream>
#include <sstream>
#include <vector>

#include "components.h"
#include "constants.h"
#include "vertex.h"


struct copyOBJ{
    std::vector<Vertex> meshVert;
    std::vector<GLuint> meshIndic;
    float collisionRadius;
};
class resourceSystem
{
public:
    resourceSystem();
    void CreateMeshComponent();
    void CreateMeshComponent(std::string input, MeshComponent * mesh);


private:
    std::vector < std::pair<std::string, std::vector<Vertex> > > meshContainer;
    std::vector < std::pair<std::string, std::vector<GLuint> > > meshIndiceContainer;
    std::vector < std::pair<std::string, float>> collisionRad;
    std::vector < std::pair<std::string, copyOBJ>> OBJCOPYContainer;
    float calculateLenght(QVector3D pos );



};

#endif // RESOURCESYSTEM_H