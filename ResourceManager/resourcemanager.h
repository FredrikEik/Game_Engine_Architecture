#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

//#include <QOpenGLFunctions_4_1_Core>
#include <string>
#include <map>
#include <vector>

#include "components.h"
#include "constants.h"
#include "soundhandler.h"

//forward declaration
class GameObject;
class MeshHandler;
class TextureHandler;
struct MeshData;

class ResourceManager // : public QOpenGLFunctions_4_1_Core
{
public:
    static ResourceManager& getInstance();

    GameObject* addObject(std::string meshName);
    bool addComponent(std::string assetName, GameObject* ownerObject);

    void setUpAllTextures();

    //TODO: These functions do nothing other than forward to MeshHandeler - maybe delete?
    MeshData makeLineBox(std::string meshName);
    MeshData makeCircleSphere(float radius, bool rgbColor);
    MeshData makeFrustum(const struct Frustum &frustumIn);

    MeshHandler* mMeshHandler{nullptr};

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

    class Logger* mLogger{nullptr};
};

#endif // RESOURCEMANAGER_H
