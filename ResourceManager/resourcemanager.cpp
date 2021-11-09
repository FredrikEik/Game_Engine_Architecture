    #include "resourcemanager.h"

#include <sstream>
#include <fstream>
#include <vector>
#include <QString>
#include <QDebug>

#include "components.h"
#include "vector3d.h"
#include "vector2d.h"
#include "vertex.h"
#include "gameobject.h"
#include "constants.h"
#include "soundsystem.h"
#include "soundhandler.h"
#include "meshhandler.h"
#include "texturehandler.h"
#include "coreengine.h"

ResourceManager::ResourceManager()
{
    SoundSystem::getInstance();    //makes sure the SoundManager is made - needed before adding sounds
    mMeshHandler = new MeshHandler();
    mTextureHandler = new TextureHandler();
}

ResourceManager &ResourceManager::getInstance()
{
    static ResourceManager *mInstance = new ResourceManager();
    return *mInstance;
}

GameObject *ResourceManager::addObject(std::string meshName)
{
    //Make the mesh from the string in
    int meshIndex = mMeshHandler->makeMesh(meshName);

    //Make the GameObject
    GameObject* tempObject = new GameObject();

    //Add standard components to GameObject
    ecs::MeshComponent* tempMesh = new ecs::MeshComponent();

    //run through all potential LOD levels:
    for(int i{0}; i<3; i++)
    {
        //Dangerous, because mMeshes vector can resize and will move pointers:
        tempMesh->mVAO[i] = mMeshHandler->mMeshes.at(meshIndex).mVAO[i];
        tempMesh->mVertexCount[i] = mMeshHandler->mMeshes.at(meshIndex).mVertexCount[i];
        tempMesh->mIndexCount[i] = mMeshHandler->mMeshes.at(meshIndex).mIndexCount[i];
    }
    tempMesh->mDrawType = mMeshHandler->mMeshes.at(meshIndex).mDrawType;
    tempMesh->mColliderRadius = mMeshHandler->mMeshes.at(meshIndex).mColliderRadius;
    tempObject->mMesh = tempMesh;

    tempObject->mMaterial = new ecs::MaterialComponent();
    tempObject->mTransform = new ecs::TransformComponent();
    return tempObject; //temporary to get to compile
}
bool ResourceManager::checkCollision(GameObject* obj1, GameObject * obj2)
{
    //length between 2 objects from center to center
    gsl::Vector3D lengthVec = (obj1->mTransform->mMatrix.getPosition() -
            obj2->mTransform->mMatrix.getPosition() ) * 3.14;

    float distance = lengthVec.length();


    float radiusOfObjects = obj1->mMesh->mColliderRadius + obj2->mMesh->mColliderRadius;



         if(distance > radiusOfObjects)
         {
            return false;
         }
         else
         {
            return true;
         }

}

bool ResourceManager::addComponent(std::string assetName, GameObject *ownerObject)
{
    if(!ownerObject)
    {
        qDebug() << "Trying to add Component to non-existing GameObject";
        return false;
    }

    gsl::AssetType assetType = findAssetType(assetName);

    switch (assetType) {
    case  gsl::NOASSETTYPE:
        return false;
        break;
    case gsl::OBJ:
        break;
    case gsl::FBX:
        break;
    case gsl::BMP:
        break;
    case gsl::WAV:
        ownerObject->mSoundComponent = makeSoundComponent(assetName);
        break;
    }

    return true;
}

gsl::AssetType ResourceManager::findAssetType(std::string assetName)
{
    if (assetName.find(".obj") != std::string::npos)
       return gsl::OBJ;
    if (assetName.find(".fbx") != std::string::npos)
       return gsl::FBX;
    if (assetName.find(".bmp") != std::string::npos)
       return gsl::BMP;
    if (assetName.find(".wav") != std::string::npos)
       return gsl::WAV;
//    if(assetName.find(".js") != std::string::npos)
//        return gsl::

    return gsl::NOASSETTYPE;
}

ecs::SoundComponet *ResourceManager::makeSoundComponent(std::string assetName)
{
    int soundIndex{-1};
    WaveRawData* waveData{nullptr}; //will be instansiated inside loadWave

    //check if asset is made:
    auto result = mSoundBufferMap.find(assetName);
    //if already made
    if (result != mSoundBufferMap.end()) {        //found!!!
        soundIndex = result->second;
    }
    //not made, make new
    else {
        waveData = SoundHandler::loadWave(assetName);
        if (!waveData)
        {
            qDebug() << "Error loading wave file!";
            return nullptr;
        }
        //update mMeshComponentMap with new asset
        mWaveBuffers.emplace_back(*waveData);
        soundIndex = mWaveBuffers.size()-1;
        mSoundBufferMap.emplace(assetName, soundIndex);
    }

    ecs::SoundComponet *tempSource = new ecs::SoundComponet();
    if(waveData)
    {
        tempSource->mSource = SoundHandler::makeALSource(mWaveBuffers.at(soundIndex).mALBuffer);
    }
    return tempSource;
}

void ResourceManager::setUpAllTextures()
{
    //should probably run thru all textures found in TextureFilePath
    mTextureHandler->makeTexture();
    mTextureHandler->makeTexture("hund.bmp");
    mTextureHandler->makeTexture("goat.bmp");
}

MeshData ResourceManager::makeLineBox(std::string meshName)
{
    return mMeshHandler->makeLineBox(meshName);
}

MeshData ResourceManager::makeCircleSphere(float radius, bool rgbColor)
{
    return mMeshHandler->makeCircleSphere(radius, rgbColor);
}
