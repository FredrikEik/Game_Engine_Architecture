#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include "xyz.h"
#include "camera.h"
#include "triangle.h"
#include "components.h"
#include "visualobject.h"

class ResourceManager
{
public:
    ResourceManager();

    void manageObjects();
    void addObject(std::string name = "DefaultGameObject", int id = 0,
                   int TransformID = 0, int MeshID = 0, int MaterialID = 0, int GravityID = 0);
    std::vector<VisualObject*> mVisualObjects;
private:
};

#endif // RESOURCEMANAGER_H
