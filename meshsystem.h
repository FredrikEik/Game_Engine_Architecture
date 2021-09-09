#ifndef MESHSYSTEM_H
#define MESHSYSTEM_H

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



class meshsystem
{
public:
    meshsystem();
    void CreateMeshComponent();
    MeshComponent * CreateMeshComponent(std::string input, int entity, bool bInProjDir = false);

    //void CreateMeshComponent(std::string defaultMesh);
    //MeshComponent OBJReader();


    MeshComponent * mesh;
};

#endif // MESHSYSTEM_H
