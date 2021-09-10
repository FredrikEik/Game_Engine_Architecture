#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <QOpenGLFunctions_4_1_Core>
#include "components.h"
#include "gameobject.h"
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <map>

class ResourceManager : public QOpenGLFunctions_4_1_Core
{
public:
    ResourceManager();
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

#endif // RESOURCEMANAGER_H
