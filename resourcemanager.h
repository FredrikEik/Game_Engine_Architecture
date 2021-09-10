#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <QOpenGLFunctions_4_1_Core>
#include "components.h"
#include "gameobject.h"
#include <string>
#include <vector>
#include <sstream>
#include <fstream>

class ResourceManager : public QOpenGLFunctions_4_1_Core
{
public:
    ResourceManager();
    GameObject* addObject(std::string objName, std::vector<Vertex> vertexes);
    GameObject* readObj(std::string filename);
    GameObject* addTriangle();
    GameObject* addCube();
    GameObject* addXYZ();

private:
//    GameObject* tempObj{nullptr};
};

#endif // RESOURCEMANAGER_H
