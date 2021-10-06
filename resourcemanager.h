#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include "xyz.h"
#include "camera.h"
#include "triangle.h"
#include "cube.h"
#include "objmesh.h"
#include "components.h"
#include "visualobject.h"
#include <sstream>

class ResourceManager
{
public:
    ResourceManager();
    ~ResourceManager();

    void manageObjects(std::string objectName, int ObjectID);
    void addObject(std::string name = "DefaultGameObject", int id = 0, int ObjectType = 0,
                   int TransformID = 0, int MeshID = 0, int MaterialID = 0);
    void readObj(std::string name, VisualObject* object);
    std::vector<VisualObject*> mVisualObjects;
private:
int id;
std::string name;
MeshComponent cubeMemory;
MeshComponent triangleMemory;
MeshComponent axisMemory;
};

#endif // RESOURCEMANAGER_H
