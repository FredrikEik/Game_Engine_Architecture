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
    GameObject *CreateObject(std::string filepath, bool bUsingLOD = false);
    //void readObj(std::string &filename, MeshComponent *MeshComp);
//    void init(MeshComponent &MeshComp);
private:
    GameObject* tempGO;
    MeshHandler* mMeshHandler;
    std::vector<GameObject*> mObjects;
    std::unordered_map<std::string, GameObject> mObjectsMap;
    void createXYZ(MeshComponent *MeshComp);
public:
    static int objectIDcounter;


//    VisualObject MakeObject(MeshComponent &meshComp, MaterialComponent &material,
//                            TransformComponent &transComp);
//    VisualObject MakeObject(bool mesh, bool material, bool transform);
};




#endif // RESOURCEMANAGER_H
