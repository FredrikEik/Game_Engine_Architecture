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

/**
 * @brief The ResourceManager class
 */
class ResourceManager : public QOpenGLFunctions_4_1_Core
{
public:
    static ResourceManager& getInstance();

    ColliderComponent *mCollider = new ColliderComponent();

    GameObject* addObject(std::string meshName);///funksjon for å legge til et objekt, returnerer fra meshhandler
    GameObject* addTerrain(TextureHandler* t,float horSpaceing, float verSpacing, float height);
    /**
     * @brief addCollider legger til kollisjonsvolum
     * @param colliderType i dette tilfelle en sphere collider
     * @param objekte det skal settes på
     */
    bool addCollider(std::string colliderType, GameObject* obj);
    /**
     * @brief addComponent legger til lyd komponent
     * @param assetName er wav filas navn
     * @param ownerObject er objekte den skal settes på
     * @return returnerer false hvis ikke objektet finnes
     */
    bool addComponent(std::string assetName, GameObject* ownerObject);

    void setUpAllTextures(); ///sets up all textures

    MeshData makeLineBox(std::string meshName); ///makes line box
    MeshData makeCircleSphere(float radius, bool rgbColor);
    MeshData makeFrustum(const struct Frustum &frustumIn); ///make frustum draw

    void setSurface(GameObject* surface, GameObject* ball);
    void moveAlongSurface(float dt, GameObject * ball);

    MeshHandler* mMeshHandler{nullptr};

    /** sjekker om 2 objekter kolliderer
     * @param obj1
     * @param obj2
     * @return returnerer true hvis de kolliderer
     */
    bool checkCollision( GameObject* obj1, GameObject * obj2);
    std::map<std::string, unsigned int> mShaderMap;
    std::vector<ShaderHandler*> mShaders;


private:
    ResourceManager();  //singleton
    void operator=(ResourceManager&){};           //Assignment operator
    ResourceManager(const ResourceManager&){};    //Copy constructor

    gsl::AssetType findAssetType(std::string assetName);

    SoundComponet* makeSoundComponent(std::string assetName);///lager lyd komponent
    std::map<std::string, unsigned int> mSoundBufferMap;
    std::vector<WaveRawData> mWaveBuffers;

    TextureHandler *mTextureHandler{nullptr};
};

#endif // RESOURCEMANAGER_H
