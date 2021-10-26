#include "resourcemanager.h"

#include <sstream>
#include <fstream>
#include <vector>
#include <QString>
#include <QDir>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

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
#include "shaderhandler.h"
#include "logger.h"

ResourceManager::ResourceManager()
{
    mLogger = Logger::getInstance();    //Have to do this, else program will crash
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

    //copy all potential LOD levels:
    //std::copy faster than for loop and safer and ca as fast as memcpy
    std::copy(mMeshHandler->mMeshes.at(meshIndex).mVAO, mMeshHandler->mMeshes.at(meshIndex).mVAO+3, tempMesh->mVAO);
    std::copy(mMeshHandler->mMeshes.at(meshIndex).mVertexCount, mMeshHandler->mMeshes.at(meshIndex).mVertexCount+3, tempMesh->mVertexCount);
    std::copy(mMeshHandler->mMeshes.at(meshIndex).mIndexCount, mMeshHandler->mMeshes.at(meshIndex).mIndexCount+3, tempMesh->mIndexCount);

    tempMesh->mDrawType = mMeshHandler->mMeshes.at(meshIndex).mDrawType;
    tempMesh->mColliderRadius = mMeshHandler->mMeshes.at(meshIndex).mColliderRadius;
    tempObject->mMesh = tempMesh;

    tempObject->mMaterial = getMaterial("Default");
    tempObject->mTransform = new TransformComponent();
    return tempObject; //temporary to get to compile
}

bool ResourceManager::addComponent(std::string assetName, GameObject *ownerObject)
{
    if(!ownerObject)
    {
        mLogger->logText("Trying to add Component to non-existing GameObject", LColor::DAMNERROR);
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
            mLogger->logText("Error loading wave file!", LColor::DAMNERROR);
            return nullptr;
        }
        //update mMeshComponentMap with new asset
        mWaveBuffers.emplace_back(*waveData);
        soundIndex = mWaveBuffers.size()-1;
        mSoundBufferMap.emplace(assetName, soundIndex);
    }

    SoundComponet *tempSource = new SoundComponet();
    if(soundIndex > -1)
    {
        tempSource->mSource = SoundHandler::makeALSource(mWaveBuffers.at(soundIndex).mALBuffer);
    }
    return tempSource;
}

void ResourceManager::setUpAllTextures()
{   
    //Making default texture
    mTextureHandler->makeTexture();

    //Regular .bmp textures read from file
    QDir tempDir((gsl::TextureFilePath).c_str());
    if(tempDir.exists())
    {
        QStringList filters;
        filters << "*.bmp";
        tempDir.setNameFilters(filters);
        mLogger->logText( std::to_string(tempDir.entryInfoList().size()) +
                          " - .bmp textures will be read from " + gsl::TextureFilePath);


        //read all regular textures
        for(QFileInfo &var : tempDir.entryInfoList())
        {
            mLogger->logText("Texture name: " + var.fileName().toStdString());
            mTextureHandler->makeTexture(var.fileName().toStdString());
        }
    }
    else
    {
        mLogger->logText("*** ERROR reading textures *** : Asset-folder " +
                         gsl::TextureFilePath + " does not exist!", LColor::DAMNERROR);
    }
}

void ResourceManager::setUpAllSounds()
{
    int soundIndex{-1};
    WaveRawData* waveData{nullptr}; //will be instansiated inside loadWave

    QDir tempDir((gsl::SoundFilePath).c_str());
    if(tempDir.exists())
    {
        QStringList filters;
        filters << "*.wav";
        tempDir.setNameFilters(filters);
        mLogger->logText( std::to_string(tempDir.entryInfoList().size()) +
                          " - .wav files will be read from " + gsl::SoundFilePath);

        //read all wav files
        for(QFileInfo &var : tempDir.entryInfoList())
        {
            mLogger->logText("Sound name: " + var.fileName().toStdString());
            waveData = SoundHandler::loadWave(var.fileName().toStdString());
            if (!waveData)
            {
                mLogger->logText("Error loading wave file!", LColor::DAMNERROR);
            }
            //update mWaveBuffers with new asset
            mWaveBuffers.emplace_back(*waveData);
            soundIndex = mWaveBuffers.size()-1;
            mSoundBufferMap.emplace(var.fileName().toStdString(), soundIndex);
        }
    }
    else
    {
        mLogger->logText("*** ERROR reading sounds *** : Asset-folder " + gsl::SoundFilePath +
                         " does not exist!", LColor::DAMNERROR);
    }
}

void ResourceManager::setUpAllMeshes()
{
    QDir tempDir((gsl::MeshFilePath).c_str());
     if(tempDir.exists())
     {
         QStringList filters;
         filters << "*.obj";
         tempDir.setNameFilters(filters);
         mLogger->logText( std::to_string(tempDir.entryInfoList().size()) + " - .obj files located in " + gsl::MeshFilePath);

         //read all wav files
         for(QFileInfo &var : tempDir.entryInfoList())
         {
             //filtering out LODS
             auto isLOD  = var.fileName().toStdString().find(gsl::LODLevelPrefix);
             if(isLOD != std::string::npos)
                 continue;

             mLogger->logText("Mesh name: " + var.fileName().toStdString());
             mMeshHandler->makeMesh(var.fileName().toStdString());
         }
     }
     else
     {
         mLogger->logText("*** ERROR reading meshes *** : Asset-folder " +
                          gsl::MeshFilePath + " does not exist!", LColor::DAMNERROR);
     }
}

void ResourceManager::setUpAllMaterials()
{
    mLogger->logText("Loading materials", LColor::HIGHLIGHT);

    //First material is hardcoded to be like this and called "Default"
    MaterialComponent defaultMaterial;
    auto result = mShaderMap.find("plain");
    if (result != mShaderMap.end()) {        //found!!!
        defaultMaterial.mShaderProgram = result->second;
    }
    else
        mLogger->logText("Shader for default material not found!", LColor::DAMNERROR);
    defaultMaterial.mColor = gsl::Vector3D{1.f, 0.301f, 0.933f}; //pink

    mMaterials.push_back(defaultMaterial); //this should performe a copy...
    mMaterialMap.emplace("Default", 0);

    //****************** Read materials from json file **************************
    //this code have a lot of checking code, so it is long.
    //probably should be broken up into more functions

    QFile loadFile(QString((gsl::AssetFilePath + "materials.json").c_str()));
    if (!loadFile.open(QIODevice::ReadOnly)) {
        mLogger->logText("Couldn't open materials.json file for reading.", LColor::DAMNERROR);
        return;
    }
    mLogger->logText("  materials.json file opened for reading.");
    QByteArray saveData = loadFile.readAll();   //read whole file
    QJsonDocument loadDoc(QJsonDocument::fromJson(saveData));   //convert to json document
    QJsonObject jsonObject = loadDoc.object();  //read first object == whole thing

    //should contain Materials as first property and this should be an array
    if (jsonObject.contains("Materials") && jsonObject["Materials"].isArray())
    {
        QJsonArray materialArray = jsonObject["Materials"].toArray();
        //go through each material in file
        int materialIndex{0};   //used to print log afterwards
        for ( ; materialIndex < materialArray.size(); ++materialIndex) {
            MaterialComponent temp;

            QJsonObject materialObject = materialArray[materialIndex].toObject();   //first material as object
            QString name;   //this is pushed as the key to the mMaterialMap later
            if (materialObject.contains("name") && materialObject["name"].isString())
                name = materialObject["name"].toString();
            if (materialObject.contains("shader") && materialObject["shader"].isString())
            {
                QString shader = materialObject["shader"].toString();
                auto result = mShaderMap.find(shader.toStdString());
                //if already made
                if (result != mShaderMap.end()) {        //found!!!
                    temp.mShaderProgram = result->second;
                }
            }
            if (materialObject.contains("texture") && materialObject["texture"].isString())
            {
                QString texture = materialObject["texture"].toString();
                auto result = mTextureHandler->mTextureMap.find(texture.toStdString());
                //if already made
                if (result != mTextureHandler->mTextureMap.end()) {        //found!!!
                    temp.mTextureUnit = result->second;
                }
            }
            if (materialObject.contains("color") && materialObject["color"].isArray())
            {
                QJsonArray colorArray = materialObject["color"].toArray();
                if (colorArray[0].isDouble())
                    temp.mColor.x = colorArray[0].toDouble();
                if (colorArray[1].isDouble())
                    temp.mColor.y = colorArray[1].toDouble();
                if (colorArray[2].isDouble())
                    temp.mColor.z = colorArray[2].toDouble();
            }
            mMaterials.push_back(temp); //this should performe a copy...
            mMaterialMap.emplace(name.toStdString(), materialIndex + 1);
        }
        mLogger->logText("  " + std::to_string(materialIndex) + " materials read from file");
    }
    else
        mLogger->logText("materials.json file does not contain a Materials object", LColor::DAMNERROR);
}

void ResourceManager::setUpAllShaders()
{
    //TODO: This function is very manual and hardcoded
    //should be automated to read all shaders in path

    //Compile shaders:
    //NB: hardcoded path to files! You have to change this if you change directories for the project.
    //Qt makes a build-folder besides the project folder. That is why we go down one directory
    // (out of the build-folder) and then up into the project folder.
    ShaderHandler *tempShader = new ShaderHandler((gsl::ShaderFilePath + "plainshader.vert").c_str(),
            (gsl::ShaderFilePath + "plainshader.frag").c_str());
    tempShader->mName = "PlainShader";
    mShaders.push_back(tempShader);
    std::string tempString;
    tempString += "Plain shader program id: " + std::to_string(mShaders[0]->mProgram);
    mLogger->logText(tempString);
    mShaders.back()->setupShader(false);

    mShaderMap.emplace("plain", 0);

    tempShader = new ShaderHandler((gsl::ShaderFilePath + "textureshader.vert").c_str(),
                                    (gsl::ShaderFilePath + "textureshader.frag").c_str());
    tempShader->mName = "TextureShader";
    mShaders.push_back(tempShader);
    tempString.clear();
    tempString += "Texture shader program id: " + std::to_string(mShaders[1]->mProgram);
    mLogger->logText(tempString);
    mShaders[1]->setupShader(true);
    mShaderMap.emplace("texture", 1);
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

MaterialComponent *ResourceManager::getMaterial(std::string materialName)
{
    int index{-1};
    auto result = mMaterialMap.find(materialName);
    if (result != mMaterialMap.end()) {        //found!!!
        index = result->second;
        return &mMaterials[index];
    }
    else
    {
        mLogger->logText("Material \"" + materialName + "\" not found. Using default.", LColor::WARNING);
        return &mMaterials[0];
    }
}
