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

ResourceManager::ResourceManager()
{
    SoundSystem::getInstance();    //makes sure the SoundManager is made - needed before adding sounds
    mMeshHandler = new MeshHandler();
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
    MeshComponent* tempMesh = new MeshComponent();
    //Dangerous, because mMeshes vector can resize and will move pointers:
    tempMesh->mVAO = mMeshHandler->mMeshes.at(meshIndex).mVAO;
    tempMesh->mVertexCount = mMeshHandler->mMeshes.at(meshIndex).mVertexCount;
    tempMesh->mIndexCount = mMeshHandler->mMeshes.at(meshIndex).mIndexCount;
    tempMesh->mDrawType = mMeshHandler->mMeshes.at(meshIndex).mDrawType;
    tempObject->mMesh = tempMesh;

    tempObject->mMaterial = new MaterialComponent();
    tempObject->mTransform = new TransformComponent();
    return tempObject; //temporary to get to compile
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

    return gsl::NOASSETTYPE;
}

SoundComponet *ResourceManager::makeSoundComponent(std::string assetName)
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

    SoundComponet *tempSource = new SoundComponet();
    if(waveData)
    {
        tempSource->mSource = SoundHandler::makeALSource(mWaveBuffers.at(soundIndex).mALBuffer);
    }
    return tempSource;
}