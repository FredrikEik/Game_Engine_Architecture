#ifndef OBJECTMANAGER_H
#define OBJECTMANAGER_H

#include "GameObject.h"
#include "Components.h"
#include <vector>
#include <string>

// I will soon rename this class to "ResourceManager" because
// this name fits better when including audio and texture resources.
class ObjectManager : public QOpenGLFunctions_4_1_Core
{
public:
    ObjectManager();
    GameObject* CreateObject(std::string filename);
    int readObj(std::string filename);
    void init(Mesh mesh);
    ObjectManager& getInstance();

    int XYZ();

private:
    GameObject* object;
    std::vector<GameObject*> gameObjects;

    std::map<std::string, unsigned int> mMeshIndexMap;
    std::vector<Mesh> mMeshComponents;

    Mesh* mesh;
    Transform* transform;
    Material* material;
};

#endif // OBJECTMANAGER_H
