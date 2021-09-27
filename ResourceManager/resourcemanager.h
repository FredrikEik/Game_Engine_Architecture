#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <QOpenGLFunctions_4_1_Core>
#include <string>
#include <map>
#include <vector>

#include "constants.h"
#include "components.h"
#include "soundmanager.h"

class GameObject;
class MeshHandler;
class TextureHandler;
struct MeshData;

class ResourceManager : public QOpenGLFunctions_4_1_Core
{
public:
    static ResourceManager& getInstance;

    GameObject* addObject(std::string meshName);
    bool addComponent(std::string assetName, GameObject* ownerObject);

    void setUpAllTextures();

    MeshHandler* meshHandler{nullptr};

private:
    ResourceManager();
    void operator=(ResourceManager&){};           //Assignment operator
        ResourceManager(const ResourceManager&){};    //Copy constructor

        gsl::AssetType findAssetType(std::string assetName);

        SoundComponent* makeSoundComponent(std::string assetName);

        //Should these belong to other class - like in MeshHandler?
        std::map<std::string, unsigned int> mSoundBufferMap;
//        std::vector<WaveRawData> mWaveBuffers;

        TextureHandler *mTextureHandler{nullptr};
};

#endif // RESOURCEMANAGER_H
