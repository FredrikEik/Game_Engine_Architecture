#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <QOpenGLFunctions_4_1_Core>
#include <string>
#include <map>
#include <vector>

#include "components.h"
#include "constants.h"
#include "math_constants.h"
#include "constants.h"
#include "vector2d.h"
#include "vector3d.h"
#include "matrix3x3.h"
#include "matrix4x4.h"
#include "soundhandler.h"
#include "shaderhandler.h"

//forward declaration
class GameObject;
class MeshHandler;
class Textures;
struct MeshData;

class ResourceManager : public QOpenGLFunctions_4_1_Core
{
public:
    static ResourceManager& getInstance();

    GameObject* addObject(std::string meshName);
    bool addComponent(std::string assetName, GameObject* ownerObject);

    void setUpAllTextures();

    MeshData makeLineBox(std::string meshName);
    MeshData makeCircleSphere(float radius, bool rgbColor);

    MeshHandler* mMeshHandler{nullptr};

    bool checkCollision( GameObject* obj1, GameObject * obj2);
    void readShaders();
    class ShaderHandler *mShaderProgram[gsl::NumberOfShaders]{nullptr};
    void setScript(MAX_ENTITIES_TYPE entityID, std::string fileName);

    bool Collided = false;

    class Scene *mCurrentScene{nullptr};
    class CoreEngine *mCoreEngine{nullptr};

private:
    ResourceManager();  //singleton
    void operator=(ResourceManager&){};           //Assignment operator
    ResourceManager(const ResourceManager&){};    //Copy constructor

    gsl::AssetType findAssetType(std::string assetName);

    ecs::SoundComponet* makeSoundComponent(std::string assetName);

    //Should these belong to other class - like in MeshHandler?
    std::map<std::string, unsigned int> mSoundBufferMap;
    std::vector<WaveRawData> mWaveBuffers;

    Textures *mTextureHandler{nullptr};
    class GameObject *mGameObject{nullptr};
};

#endif // RESOURCEMANAGER_H
