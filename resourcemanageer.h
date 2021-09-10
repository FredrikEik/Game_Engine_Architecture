#ifndef RESOURCEMANAGEER_H
#define RESOURCEMANAGEER_H

#include <QOpenGLFunctions_4_1_Core>
#include "components.h"
#include "gameobject.h"
#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <fstream>

class ResourceManageer : public QOpenGLFunctions_4_1_Core
{
public:
    ResourceManageer();
    GameObject* addObject(std::string objName, std::vector<Vertex> vertexes);
    GameObject* addCreatedObject(std::string objName);
    GameObject* readObj(std::string filename);
    GameObject* addTriangle();
    GameObject* addCube();
    GameObject* addXYZ();

    int makeTriangle();

private:
    std::map<std::string, int> meshComponentMap;
    std::vector<MeshComponent> meshComponents;

//    GameObject* tempObj{nullptr};
};

#endif // RESOURCEMANAGEER_H
