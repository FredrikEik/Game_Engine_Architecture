#ifndef GAMEOBJECTMANAGER_H
#define GAMEOBJECTMANAGER_H

#include <QOpenGLFunctions_4_1_Core>
#include <string>
#include <map>
#include <vector>

#include "components.h"
#include "constants.h"
#include "ResourceManager/soundhandler.h"

//forward declaration
class GameObject;
class MeshHandler;
class TextureHandler;
struct MeshData;

class GameObjectManager : public QOpenGLFunctions_4_1_Core
{
public:
    static GameObjectManager& getInstance();

    GameObject* addObject(std::string name = "DefaultGameObject");
    bool addComponent(std::string assetName, GameObject* ownerObject);

    void setUpAllTextures();

    MeshData makeLineBox(std::string MeshName);
    MeshData makeCircleSphere(float radius, bool rgbColor);

    MeshHandler* mMeshHandler{nullptr};

private:
    GameObjectManager();                            //singleton
    void operator=(GameObjectManager&){};           //Assignment operator
    GameObjectManager(const GameObjectManager&){};  //Copy constructor

    gsl::AssetType findAssetType(std::string assetName);

    SoundComponent* makeSoundComponent(std::string assetName);

    std::map<std::string, unsigned int> mSoundBufferMap;
    std::vector<WaveRawData> mWaveBuffers;

    TextureHandler *mTextureHandler{nullptr};

};
#endif // GAMEOBJECTMANAGER_H
