#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include "GameObject.h"
#include "Components.h"
#include <map>
#include <vector>
#include <string>
#include "renderwindow.h"
#include "coreengine.h"
#include "texture.h"

class CoreEngine;

class ResourceManager : public QOpenGLFunctions_4_1_Core
{
public:

    class GameObject* CreateMainCharacter(std::string filename);
    class GameObject* CreateObject(std::string filename);

    static ResourceManager& getInstance();
    void init(Mesh &mesh, int lod);

private:
    /** Functions */
    ResourceManager();


    int makeHeightMap(Mesh* meshComp);

    void makeSphereRadius(Mesh* meshIn, gsl::Vector3D &vertexIn);

    int readObj(std::string filename);

    /** Variables */
    GameObject* object;

    std::map<std::string, unsigned int> mMeshIndexMap;
    std::vector<Mesh> mMeshComponents;

    Mesh* mesh;
    Transform* transform;
    Material* material;

    CoreEngine* mCoreEngine{nullptr};
};

#endif // RESOURCEMANAGER_H
