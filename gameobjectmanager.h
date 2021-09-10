#ifndef GAMEOBJECTMANAGER_H
#define GAMEOBJECTMANAGER_H

#include "xyz.h"
#include "camera.h"
#include "triangle.h"
#include "gameobject.h"
#include "components.h"
#include "renderwindow.h"

using namespace std;

class GameObjectManager : public QOpenGLFunctions_4_1_Core
{
public:
    static GameObjectManager& getInstance();

    void manageObjects();
    GameObject addObject(std::string name = "DefaultGameObject", int id = 0, bool TransformID = 0, int MeshID = 0, int MaterialID = 0, bool GravityID = 0);

private:
    GameObjectManager();                            //singleton
    void operator=(GameObjectManager&){};           //Assignment operator
    GameObjectManager(const GameObjectManager&){};  //Copy constructor

    //int readObj(std::string filename);

};
#endif // GAMEOBJECTMANAGER_H
