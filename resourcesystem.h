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



class resourceSystem
{
public:
    resourceSystem();
    void CreateMeshComponent();
    void CreateMeshComponent(std::string input, MeshComponent * mesh);

    //void CreateMeshComponent(std::string defaultMesh);
    //MeshComponent OBJReader();
    bool alreadyExists(std::vector<Vertex> inVertices);
    void manageResources();
private:
    std::vector<std::vector<Vertex>> meshContainer;
    //std::vector<std::vector<GLuint>> meshIndiceContainer;
    float calculateLenght(QVector3D pos );
};

#endif // RESOURCESYSTEM_H
