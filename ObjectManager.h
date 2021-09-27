#ifndef OBJECTMANAGER_H
#define OBJECTMANAGER_H

#include "GameObject.h"
#include "Components.h"
#include <map>
#include <vector>
#include <string>
#include "renderwindow.h"

// I will soon rename this class to "ResourceManager" because
// this name fits better when including audio and texture resources.
class ObjectManager : public QOpenGLFunctions_4_1_Core
{
public:

    class GameObject* CreateObject(std::string filename);

    static ObjectManager& getInstance();

private:
    /** Functions */
    ObjectManager();

    void init(Mesh &mesh);

    int readObj(std::string filename);

    int Cube(Mesh* mesh);

    /** Variables */
    GameObject* object;
    std::vector<GameObject*> gameObjects;

    std::map<std::string, unsigned int> mMeshIndexMap;
    std::vector<Mesh> mMeshComponents;

    Mesh* mesh;
    Transform* transform;
    Material* material;
};

#endif // OBJECTMANAGER_H
