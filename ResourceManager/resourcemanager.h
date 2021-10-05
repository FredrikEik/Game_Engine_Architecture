#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <QOpenGLFunctions_4_1_Core>
#include "components.h"
#include "gameobject.h"
#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <fstream>
#include "camera.h"

class ResourceManager : public QOpenGLFunctions_4_1_Core
{
public:
    ResourceManager();
    GameObject* addObject(std::string objName, std::vector<Vertex> vertexes);
    GameObject* readObj(std::string filename);
    GameObject* addTriangle();
    GameObject* addCube();
    GameObject* addXYZ();
    GameObject* makeFrustum(const Frustum &frustumIn);

    GameObject* objectCreator(std::string objName);
    int standardTriangle();
    int standardCube();

private:
    std::map<std::string, unsigned int> meshComponentMap;
    std::vector<MeshComponent*> meshComponents;

//    GameObject* tempObj{nullptr};
};

#endif // RESOURCEMANAGER_H
