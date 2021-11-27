#ifndef GAMEOBJECTMANAGER_H
#define GAMEOBJECTMANAGER_H


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
class ShaderHandler;

class GameObjectManager //: public QOpenGLFunctions_4_1_Core
{
public:
    static GameObjectManager& getInstance();

    GameObject* addObject(std::string name = "DefaultGameObject");
    bool addComponent(std::string assetName, GameObject* ownerObject);

    void setUpAllTextures();
    void setUpAllSounds();
    void setUpAllMeshes();

    void setUpAllMaterials();
    void setUpAllShaders();


    MeshData makeLineBox(std::string MeshName);
    MeshData makeCircleSphere(float radius, bool rgbColor);
    MeshData makeFrustum(const struct Frustum &frustumIn);

//    void makePointObject(MeshData temp); //Turn points into a object that can be drawn.

    MeshHandler* mMeshHandler{nullptr};

    std::map<std::string, unsigned int> mShaderMap;
    std::vector<ShaderHandler*> mShaders;

    MaterialComponent* getMaterial(std::string materialName);
    TextureHandler *mTextureHandler{nullptr};

    std::map<std::string, unsigned int> mMaterialMap;
    std::vector<MaterialComponent> mMaterials;

private:
    GameObjectManager();                            //singleton
    void operator=(GameObjectManager&){};           //Assignment operator
    GameObjectManager(const GameObjectManager&){};  //Copy constructor

    gsl::AssetType findAssetType(std::string assetName);

    SoundComponent* makeSoundComponent(std::string assetName);

    std::map<std::string, unsigned int> mSoundBufferMap;
    std::vector<WaveRawData> mWaveBuffers;

    class Logger* mLogger{nullptr};

};
#endif // GAMEOBJECTMANAGER_H
