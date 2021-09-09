#ifndef RESOURCEMANAGEER_H
#define RESOURCEMANAGEER_H

#include <QOpenGLFunctions_4_1_Core>
#include "components.h"
#include "gameobject.h"
#include <string>
#include <vector>
#include <sstream>
#include <fstream>

class ResourceManageer : public QOpenGLFunctions_4_1_Core
{
public:
    ResourceManageer();
    GameObject* addObject(std::string objName, std::vector<Vertex> vertexes);
    GameObject* readObj(std::string filename);
    GameObject* addTriangle();
    GameObject* addCube();

private:
//    GameObject* tempObj{nullptr};
};

#endif // RESOURCEMANAGEER_H
