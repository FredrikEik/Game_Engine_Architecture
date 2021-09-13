#ifndef GAMEOBJECTMANAGER_H
#define GAMEOBJECTMANAGER_H

#include <QOpenGLFunctions_4_1_Core>
#include "gameobject.h"
#include "components.h"

using namespace std;

class GameObjectManager : public QOpenGLFunctions_4_1_Core
{
public:
    static GameObjectManager& getInstance();

    void manageObjects();

    class GameObject* addObject(std::string name = "DefaultGameObject", int id = 0, bool TransformID = 0, int MeshID = 0, int MaterialID = 0, bool GravityID = 0);

private:
    GameObjectManager();                            //singleton
    void operator=(GameObjectManager&){};           //Assignment operator
    GameObjectManager(const GameObjectManager&){};  //Copy constructor

    //int readObj(std::string filename);
    int makeAxis();
    int makeTriangle();

    void initMesh(MeshComponent & tempMeshComp);

    //std::map<std::string, unsigned int> mMeshComponentMap;
    std::vector<MeshComponent> mMeshComponents;

};
#endif // GAMEOBJECTMANAGER_H
