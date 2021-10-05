#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include "GameObject.h"
#include "Components.h"
#include <map>
#include <vector>
#include <string>
#include "renderwindow.h"

class RenderWindow;

class ResourceManager : public QOpenGLFunctions_4_1_Core
{
public:

    class GameObject* CreateObject(std::string filename);

    static ResourceManager& getInstance();

private:
    /** Functions */
    ResourceManager();

    void init(Mesh &mesh, int lod);

    int readObj(std::string filename);

    int Cube(Mesh* mesh);

    /** Variables */
    GameObject* object;

    std::map<std::string, unsigned int> mMeshIndexMap;
    std::vector<Mesh> mMeshComponents;

    Mesh* mesh;
    Transform* transform;
    Material* material;
};

#endif // RESOURCEMANAGER_H
