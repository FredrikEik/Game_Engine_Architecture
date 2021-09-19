#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <QOpenGLFunctions_4_1_Core>
#include <string>
#include <map>
#include <vector>

#include "components.h"
#include "constants.h"
#include "soundhandler.h"

class GameObject;   //forward declaration

class ResourceManager : public QOpenGLFunctions_4_1_Core
{
public:
    static ResourceManager& getInstance();

    GameObject* addObject(std::string meshName);
    bool addComponent(std::string assetName, GameObject* ownerObject);

private:
    ResourceManager();  //singleton
    void operator=(ResourceManager&){};           //Assignment operator
    ResourceManager(const ResourceManager&){};    //Copy constructor

    int readObj(std::string filename);
    int makeAxis();
    int makeTriangle();

    gsl::AssetType findAssetType(std::string assetName);

    void initMesh(MeshComponent & tempMeshComp);

    SoundComponet* makeSoundComponent(std::string assetName);

    std::map<std::string, unsigned int> mMeshComponentMap;
    std::vector<MeshComponent> mMeshComponents;

    std::map<std::string, unsigned int> mSoundBufferMap;
    std::vector<WaveRawData> mWaveBuffers;
};

#endif // RESOURCEMANAGER_H
