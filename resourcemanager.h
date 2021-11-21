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
class MaterialComponent;
class Texture;

class ResourceManager  : public QOpenGLFunctions_4_1_Core {

public:
    ResourceManager();


    ResourceManager *getInstance();
    //class GameObject *CreateObject(std::string filepath);
    GameObject *CreateObject(std::string filepath, bool UsingLOD = false, std::string textureFilepath = " ");
    //void readObj(std::string &filename, MeshComponent *MeshComp);
//    void init(MeshComponent &MeshComp);

    void saveScene(std::vector<GameObject*> &objects, std::string &levelName);
    void loadScene(std::vector<GameObject *> &objects,GameObject* &player, GameObject* &light, std::string &levelName);
    std::map<QString,std::vector<GameObject*>> mLevels;

    void setUpAllTextures();
    void getAllMeshNames();
    void getAllLevelNames();

    std::vector<std::string> mTextureNames;
    std::vector<std::string> mMeshNames;
    std::vector<std::string> mLevelNames;
private:
    int CreateMaterial(std::string textureName);
    GameObject* tempGO{nullptr};
    MeshHandler* mMeshHandler{nullptr};
    ResourceManager *mInstance{nullptr};

    int setMaterial(std::string textureName);
    std::map<std::string, int> mTextureMap;


//    MaterialComponent *tempComp{nullptr};
    std::map<std::string, MaterialComponent*> mMaterialMap;
    Texture *mTextures[gsl::NumberOfTextures]{nullptr};
    std::vector<Texture*> mVectorTextures;
    unsigned int textureIDcounter{0};

    std::vector<GameObject*> mObjects;
    std::map<std::string, GameObject> mObjectsMeshesMap;

    //bool checkIfAllreadyExist(std::string &filename, MeshComponent *tempMesh);
    void createXYZ(MeshComponent *MeshComp);

public:
    static int objectIDcounter;

    int meshCompCounter{0};
//    VisualObject MakeObject(MeshComponent &meshComp, MaterialComponent &material,
//                            TransformComponent &transComp);
//    VisualObject MakeObject(bool mesh, bool material, bool transform);
};




#endif // RESOURCEMANAGER_H
