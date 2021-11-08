#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <QOpenGLFunctions_4_1_Core>

#include "components.h"
#include "meshhandler.h"
#include <typeindex>
#include <unordered_map>
//#include "string"

class GameObject;
class MeshHandler;

class ResourceManager  : public QOpenGLFunctions_4_1_Core {

public:
    ResourceManager();


    ResourceManager &getInstance();
    //class GameObject *CreateObject(std::string filepath);
    GameObject *CreateObject(std::string filepath, bool UsingLOD = false);
    //void readObj(std::string &filename, MeshComponent *MeshComp);
//    void init(MeshComponent &MeshComp);

    void saveScene(std::vector<GameObject*> &objects);
    void loadScene(std::vector<GameObject *> &objects,GameObject* &player, GameObject* &light);
    std::vector<std::pair<QString,std::vector<GameObject*>>> mLevels;

private:
    GameObject* tempGO;
    MeshHandler* mMeshHandler;
    std::vector<GameObject*> mObjects;
    std::map<std::string, GameObject> mObjectsMap;

    bool checkIfAllreadyExist(std::string &filename, MeshComponent *tempMesh);
    void createXYZ(MeshComponent *MeshComp);

public:
    static int objectIDcounter;

    int meshCompCounter{0};
//    VisualObject MakeObject(MeshComponent &meshComp, MaterialComponent &material,
//                            TransformComponent &transComp);
//    VisualObject MakeObject(bool mesh, bool material, bool transform);
};




#endif // RESOURCEMANAGER_H
