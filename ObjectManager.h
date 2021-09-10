#ifndef OBJECTMANAGER_H
#define OBJECTMANAGER_H

#include "GameObject.h"
#include "Components.h"
#include <vector>
#include <string>


class ObjectManager : public QOpenGLFunctions_4_1_Core
{
public:
    ObjectManager();
    GameObject* CreateObject(std::string filename);
    int readObj(std::string filename);
    void init(Mesh mesh);

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
