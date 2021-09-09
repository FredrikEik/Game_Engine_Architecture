#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <QOpenGLFunctions_4_1_Core>

#include "components.h"
#include "visualobject.h"
#include "objmesh.h"
#include <typeindex>
#include <unordered_map>
#include "gameobject.h"
//#include "string"

class ResourceManager  : public QOpenGLFunctions_4_1_Core {

public:
    ResourceManager();

    class GameObject *CreateObject(std::string filepath);
    void readObj(std::string &filename, MeshComponent *MeshComp);
    void init(MeshComponent &MeshComp);
private:

    std::vector<VisualObject*> mObjects;
    //std::vector<std::pair<std::string, int>> mObjects;



//    VisualObject MakeObject(MeshComponent &meshComp, MaterialComponent &material,
//                            TransformComponent &transComp);
//    VisualObject MakeObject(bool mesh, bool material, bool transform);
};



#endif // RESOURCEMANAGER_H
