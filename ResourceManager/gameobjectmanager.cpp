#include "ResourceManager/gameobjectmanager.h"

#include <sstream>
#include <fstream>
#include <vector>
#include <QString>
#include <QDebug>
#include <QList>

#include "components.h"
#include "vector3d.h"
#include "vector2d.h"
#include "vertex.h"
#include "gameobject.h"
#include "constants.h"
#include "soundsystem.h"
#include "ResourceManager/soundhandler.h"
#include "ResourceManager/meshhandler.h"
#include "ResourceManager/texturehandler.h"
#include "mainwindow.h"


////Forward declaration
//class SoundSystem;

GameObjectManager::GameObjectManager()
{
    SoundSystem::getInstance(); //makes sure the SoundManager is made - needed before adding sounds
    mMeshHandler = new MeshHandler();
    mTextureHandler = new TextureHandler();

    //QList<MainWindow*> GameObjectList;
}
GameObjectManager &GameObjectManager::getInstance()
{
    static GameObjectManager *mInstance = new GameObjectManager();
    return *mInstance;
}

GameObject *GameObjectManager::addObject(std::string meshName)
{
    //Make the mesh from the string in
    int meshIndex = mMeshHandler->makeMesh(meshName);

    //Make the GameObject
    GameObject* currentGameObject = new GameObject();

    //Add the standard components to GameObject
    MeshComponent* currentMesh = new MeshComponent();

    //run through all the potential lod levels
    for(int i{0}; i<3; i++)
    {
        currentMesh->mVAO[i] = mMeshHandler->mMeshes.at(meshIndex).mVAO[1];
        currentMesh->mVertexCount[i] = mMeshHandler->mMeshes.at(meshIndex).mVertexCount[1];
        currentMesh->mIndexCount[i] = mMeshHandler->mMeshes.at(meshIndex).mIndexCount[1];
    }
    currentMesh->mDrawType = mMeshHandler->mMeshes.at(meshIndex).mDrawType;
    currentMesh->mColliderRadius = mMeshHandler->mMeshes.at(meshIndex).mColliderRadius;
    currentGameObject->mMesh = currentMesh;

    currentGameObject->mMaterial = new MaterialComponent();
    currentGameObject->mTransform = new TransformComponent;

    return currentGameObject;
}

bool GameObjectManager::addComponent(std::string assetName, GameObject *ownerObject)
{
    if(!ownerObject)
    {
        qDebug() << "Trying to add Component to non-existing GameObject";
        return false;
    }

    gsl::AssetType assetType = findAssetType(assetName);

    switch(assetType)
    {
        case gsl::NOASSETTYPE:
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

gsl::AssetType GameObjectManager::findAssetType(std::string assetName)
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

SoundComponent *GameObjectManager::makeSoundComponent(std::string assetName)
{
    int soundIndex{-1};
    WaveRawData *waveData{nullptr};

    auto result = mSoundBufferMap.find(assetName);

    if (result != mSoundBufferMap.end())
    {
        soundIndex = result->second;
    }
    else
    {
        waveData = SoundHandler::loadWave(assetName);
        if (!waveData)
        {
            qDebug() << "Error loading wave file!";
            return nullptr;
        }
        mWaveBuffers.emplace_back(*waveData);
        soundIndex = mWaveBuffers.size()-1;
        mSoundBufferMap.emplace(assetName, soundIndex);
    }

    SoundComponent *tempSource = new SoundComponent();
    if (waveData)
    {
        tempSource->mSource = SoundHandler::makeALSource(mWaveBuffers.at(soundIndex).mALBuffer);
    }
    return tempSource;
}

void GameObjectManager::setUpAllTextures()
{
    mTextureHandler->makeTexture();
    mTextureHandler->makeTexture("Hund.bmp");
}

MeshData GameObjectManager::makeLineBox(std::string meshName)
{
    return mMeshHandler->makeLineBox(meshName);
}

MeshData GameObjectManager::makeCircleSphere(float radius, bool rgbColor)
{
    return mMeshHandler->makeCircleSphere(radius, rgbColor);
}
