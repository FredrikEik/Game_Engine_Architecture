#include "resourcemanager.h"

#include <sstream>
#include <fstream>
#include <vector>
#include <QString>
#include <QDebug>
#include <QDir>

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
    MeshComponent* tempMesh = new MeshComponent();

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

    tempObject->mMaterial = new MaterialComponent();
    tempObject->mTransform = new TransformComponent();
    tempObject->mCollider = new ColliderComponent();

    return tempObject; //temporary to get to compile

}


bool ResourceManager::addCollider(std::string ColliderType, GameObject* obj)
{


    if(ColliderType == "sphere")
    {
            obj->mCollider->sphereCollider = true;
            qDebug() << "added sphere collider";
    }



    return true;
}

bool ResourceManager::checkCollision(GameObject* obj1, GameObject * obj2)
{
    //length between 2 objects from center to center
    gsl::Vector3D lengthVec = (obj1->mTransform->mMatrix.getPosition() -
            obj2->mTransform->mMatrix.getPosition() ) * 4.5;


    //sphere collider
    if(obj1->mCollider->sphereCollider && obj2->mCollider->sphereCollider)
    {
        mCollider->distance = lengthVec.length();
        mCollider->radiusOfObjects = obj1->mMesh->mColliderRadius + obj2->mMesh->mColliderRadius;

             if(mCollider->distance > mCollider->radiusOfObjects)
             {
                return false;
             }
             else
             {
                return true;
             }

    }

}

void ResourceManager::update(const float dt)
{
    //mMeshHandler->updateParticles(dt);
    //qDebug() << "suppose to work";

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

void ResourceManager::setUpAllTextures()
{



    mTextureHandler->makeTexture();

    //Regular .bmp textures read from file
    QDir tempDir((gsl::TextureFilePath).c_str());
    if(tempDir.exists())
    {
        QStringList filters;
        filters << "*.bmp";
        tempDir.setNameFilters(filters);
       // qDebug() << std::to_string(tempDir.entryInfoList().size()) +
        //                  " - .bmp textures will be read from " + gsl::TextureFilePath;


        //read all regular textures
        for(QFileInfo &var : tempDir.entryInfoList())
        {
           // qDebug() << "Texture name: " + var.fileName().toStdString();
            mTextureHandler->makeTexture(var.fileName().toStdString());
        }
    }
//    else
//    {
//       // qDebug() << "*** ERROR reading textures *** : Asset-folder " +
//       //                  gsl::TextureFilePath + " does not exist!";
//    }
}

MeshData ResourceManager::makeLineBox(std::string meshName)
{
    return mMeshHandler->makeLineBox(meshName);
}

MeshData ResourceManager::makeCircleSphere(float radius, bool rgbColor)
{
    return mMeshHandler->makeCircleSphere(radius, rgbColor);
}

MeshData ResourceManager::makeFrustum(const Frustum &frustumIn)
{
    return mMeshHandler->makeFrustum(frustumIn);
}




