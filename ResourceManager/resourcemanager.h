#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <QOpenGLFunctions_4_1_Core>
#include <string>
#include <map>
#include <vector>

#include "components.h"
#include "constants.h"
#include "soundhandler.h"
#include "collisionhandler.h"

//forward declaration
class GameObject;
class MeshHandler;
class TextureHandler;
struct MeshData;

class ResourceManager : public QOpenGLFunctions_4_1_Core
{
public:
    static ResourceManager& getInstance();

    GameObject* addObject(std::string meshName);
    bool addComponent(std::string assetName, GameObject* ownerObject);

    void setUpAllTextures();

    bool Collided = false;
    MeshData makeLineBox(std::string meshName);
    MeshData makeCircleSphere(float radius, bool rgbColor);

    MeshHandler* mMeshHandler{nullptr};

    bool checkCollision( GameObject* obj1, GameObject * obj2);

    
    //CollisionHandler *mCollisionHandler{nullptr};
    //bool checkCollision(MeshData &linebox1, MeshData &linebox2);

private:
    ResourceManager();  //singleton
    void operator=(ResourceManager&){};           //Assignment operator
    ResourceManager(const ResourceManager&){};    //Copy constructor

    gsl::AssetType findAssetType(std::string assetName);

    SoundComponet* makeSoundComponent(std::string assetName);

    //Should these belong to other class - like in MeshHandler?
    std::map<std::string, unsigned int> mSoundBufferMap;
    std::vector<WaveRawData> mWaveBuffers;

    TextureHandler *mTextureHandler{nullptr};
};

#endif // RESOURCEMANAGER_H
