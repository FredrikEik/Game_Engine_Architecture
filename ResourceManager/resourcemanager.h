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

class GameObject;
class MeshHandler;
class TextureHandler;
class ShaderHandler;
struct MeshData;

class ResourceManager : public QOpenGLFunctions_4_1_Core
{
public:
    static ResourceManager& getInstance();

    ColliderComponent *mCollider = new ColliderComponent();
    //!funksjon for å legge til et objekt, returnerer fra meshhandler
    GameObject* addObject(std::string meshName);
    GameObject* addTerrain(TextureHandler* t,float horSpaceing, float verSpacing, float height);
    bool addCollider(std::string colliderType, GameObject* obj);
    bool addComponent(std::string assetName, GameObject* ownerObject);

    void setUpAllTextures();

    MeshData makeLineBox(std::string meshName);
    MeshData makeCircleSphere(float radius, bool rgbColor);
    MeshData makeFrustum(const struct Frustum &frustumIn);

    void setSurface(GameObject* surface, GameObject* ball);
    void moveAlongSurface(float dt, GameObject * ball);

    MeshHandler* mMeshHandler{nullptr};
    ///sjekker kollisjon, returnerer true hvis kollidert
    bool checkCollision( GameObject* obj1, GameObject * obj2);

    std::map<std::string, unsigned int> mShaderMap;
    std::vector<ShaderHandler*> mShaders;


private:
    ResourceManager();  //singleton
    void operator=(ResourceManager&){};           //Assignment operator
    ResourceManager(const ResourceManager&){};    //Copy constructor

    gsl::AssetType findAssetType(std::string assetName);

    SoundComponet* makeSoundComponent(std::string assetName);
    std::map<std::string, unsigned int> mSoundBufferMap;
    std::vector<WaveRawData> mWaveBuffers;

    TextureHandler *mTextureHandler{nullptr};
};

#endif // RESOURCEMANAGER_H
