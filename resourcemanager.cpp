#include "resourcemanager.h"

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <qvector3d.h>
#include <qvector2d.h>
#include <vertex.h>
#include "gameobject.h"
#include "meshhandler.h"
#include "components.h"
#include "constants.h"
#include "texture.h"

#include <QOpenGLFunctions>

#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QDir>
#include <QJsonDocument>

ResourceManager::ResourceManager()
{
    // >> MAYBE I STILL NEED THIS
    // Create the first object just to get into the forloop
//    tempGO = new GameObject();
//    tempGO = new GameObject();
//    tempGO->mMeshComp = new MeshComponent();
//    tempGO->mTransformComp = new TransformComponent();
//    tempGO->mTransformComp->mMatrix.setToIdentity();
//    tempGO->mMaterialComp = new MaterialComponent();

//    //Burde ikke hardkode dette
//    tempGO->mMaterialComp->mShaderProgram = 1;
//    tempGO->mMaterialComp->mTextureUnit = 1;
//    mObjectsMap.insert(std::pair<std::string, GameObject>{"baseInvisible",*tempGO});


    mMeshHandler = new MeshHandler();
}

GameObject* ResourceManager::CreateObject(std::string filepath, bool UsingLOD, std::string textureName)
{
    // Loops through all the objects, if it finds it it will create a new component with the same mesh component.
    // if it does not find it in the map, it will create a new object with a unique meshComp.

    // TO DO, fix the hardcoded matirial stuff.


    tempGO = new GameObject();

    tempGO->filepath = filepath;

    tempGO->mTransformComp = new TransformComponent();
    tempGO->mTransformComp->mMatrix.setToIdentity();



    tempGO->mMaterialComp = new MaterialComponent();
    tempGO->mMaterialComp->mTextureName = textureName;
    tempGO->mMaterialComp->mTextureUnit = CreateMaterial(textureName);
    if(textureName == "plain" || textureName == " ")
    {
        tempGO->mMaterialComp->mShaderProgram = 0;
    }else
    {
        tempGO->mMaterialComp->mShaderProgram = 2;
    }


    //shoud not hardcode
    //tempGO->mMaterialComp->mTextureUnit=0;

    //CreateMaterial(textureFilepath);


    auto foundAtIndex = mObjectsMeshesMap.find(filepath);

    if(foundAtIndex != mObjectsMeshesMap.end()){
        tempGO->mMeshComp = foundAtIndex->second.mMeshComp;
        tempGO->mMeshComp->bUsingLOD = UsingLOD;
        tempGO->mCollisionComp = foundAtIndex->second.mCollisionComp;
        tempGO->mCollisionLines = foundAtIndex->second.mCollisionLines;
        mObjectsMeshesMap.insert(std::pair<std::string, GameObject>{filepath + std::to_string(objectIDcounter) ,*tempGO});
    }else{
        tempGO->mMeshComp = new MeshComponent();
        tempGO->mMeshComp->bUsingLOD = UsingLOD;
        tempGO->mCollisionLines = new MeshComponent();
        tempGO->mCollisionComp = new CollisionComponent();
        mObjectsMeshesMap.insert(std::pair<std::string, GameObject>{filepath ,*tempGO});
        if(UsingLOD)
        {
            mMeshHandler->readFile(filepath, tempGO->mMeshComp, 0, tempGO->mCollisionComp,tempGO->mCollisionLines );
            mMeshHandler->readFile(filepath, tempGO->mMeshComp, 1, tempGO->mCollisionComp,tempGO->mCollisionLines );
            mMeshHandler->readFile(filepath, tempGO->mMeshComp, 2, tempGO->mCollisionComp,tempGO->mCollisionLines );
        }else{
            if (filepath.find(".obj") != std::string::npos)
            {
                mMeshHandler->readFile(filepath, tempGO->mMeshComp, 0, tempGO->mCollisionComp,tempGO->mCollisionLines );
            }
            if (filepath.find("xyz") != std::string::npos)
            {
                mMeshHandler->createXYZAxis(tempGO->mMeshComp,tempGO->mCollisionComp, tempGO->mCollisionLines);
            }
            if (filepath.find("test_las.txt") != std::string::npos)
            {
                mMeshHandler->createCreateTerrain(filepath, tempGO->mMeshComp,tempGO->mCollisionComp, tempGO->mCollisionLines);
            }
        }
        meshCompCounter++;
    }


    //Not working atm
//    if(filepath == "xyz" || filepath == "XYZ")
//    {
//        createXYZ(tempGO->mMeshComp);
//    }

    //Burde ikke hardkode dette
//    tempGO->mMaterialComp->mShaderProgram = 0;
//    tempGO->mMaterialComp->mTextureUnit = 0;



    std::string tempName = filepath;
    // Hardcoded filesize just to make "XYZ" work
    if(filepath != "xyz" && filepath != gsl::ProjectFolderName + "test_las.txt")
    {
        tempName.erase(0,25);
        tempName.erase(tempName.end()-4,tempName.end());
    }

    tempGO->name = tempName + " ID: " + std::to_string(objectIDcounter);

    tempGO->id = objectIDcounter;


    //qDebug() << "Number of objects in map:" << mObjectsMap.size();
    //qDebug() << "Number of unique meshcomps:" << meshCompCounter;
    objectIDcounter++;
    return tempGO;
}

void ResourceManager::saveScene(std::vector<GameObject *> &objects, std::string &levelName)
{
    if(levelName.empty())
    {
        qDebug() << "You need a level name to save!";
        return;
    }
    mLevels.insert(std::pair<QString,std::vector<GameObject*>>(QString::fromStdString(levelName),objects));

    QFile saveFile(QString((gsl::AssetFilePath + "scenes.json").c_str()));

    // open file
    if (!saveFile.open(QIODevice::WriteOnly)) {
        qWarning("Couldn't open save file.");
        return;
    }
    QJsonObject coreJsonObject;

    QJsonArray levelArray;
    for(auto it : mLevels)
    {
        //auto foundAtIndex = mLevels.find(QString::fromStdString(levelName));


        QJsonObject level;
        QJsonArray objectarray;
        level["levelname"] = it.first;
        for(auto it2 : it.second)
        {
            QJsonObject levelObjects;
            levelObjects["filepath"] = it2->filepath.c_str();
            levelObjects["texturename"] = it2->mMaterialComp->mTextureName.c_str();
            // transform
            levelObjects["positionx"] = double(it2->mTransformComp->mMatrix.getPosition().x);
            levelObjects["positiony"] = double(it2->mTransformComp->mMatrix.getPosition().y);
            levelObjects["positionz"] = double(it2->mTransformComp->mMatrix.getPosition().z);

            levelObjects["rotationx"] = double(it2->mTransformComp->mRotation.getX());
            levelObjects["rotationy"] = double(it2->mTransformComp->mRotation.getY());
            levelObjects["rotationz"] = double(it2->mTransformComp->mRotation.getZ());

            levelObjects["scalex"] = double(it2->mTransformComp->mScale.getX());
            levelObjects["scaley"] = double(it2->mTransformComp->mScale.getY());
            levelObjects["scalez"] = double(it2->mTransformComp->mScale.getZ());

            levelObjects["shader"] = int(it2->mMaterialComp->mShaderProgram);
            levelObjects["usingLOD"] = it2->mMeshComp->bUsingLOD;

            objectarray.append(levelObjects);
        }
        level["objects"] = objectarray;
        levelArray.append(level);

    }
    coreJsonObject["levels"] = levelArray;
    saveFile.write(QJsonDocument(coreJsonObject).toJson());
    saveFile.close();
}

void ResourceManager::loadScene(std::vector<GameObject *> &objects, GameObject* &player, GameObject* &light, std::string &levelName)
{
    QFile loadFile(QString((gsl::AssetFilePath + "scenes.json").c_str()));

    // open file
    if (!loadFile.open(QIODevice::ReadOnly)) {
        qWarning("Couldn't open save file.");
    }
    // levelarray
    QByteArray saveData = loadFile.readAll();

    QJsonDocument loadDoc(QJsonDocument::fromJson(saveData));

    // CoreObject
    QJsonObject loadDocJsonObject = loadDoc.object();


    if (loadDocJsonObject.contains("levels") && loadDocJsonObject["levels"].isArray())
    {
        QJsonArray levelArray = loadDocJsonObject["levels"].toArray();
        mLevels.clear();

        // Check if it exists
        int found{0};
        for(int levelindex = 0; levelindex < levelArray.size();levelindex++)
        {
            QString QSlevelname;
            QJsonObject levelObjectsJsonObject = levelArray[levelindex].toObject();
            if (levelObjectsJsonObject.contains("levelname") && levelObjectsJsonObject["levelname"].isString())
                QSlevelname = levelObjectsJsonObject["levelname"].toString();

            if(QSlevelname == QString::fromStdString(levelName))
            {
                found++;
            }
        }
        if(found == 0)
        {
            qDebug() << "Level Not Found";
            loadFile.close();
            return;
        }

        objects.clear();


        for(int levelindex = 0; levelindex < levelArray.size();levelindex++)
        {
            QString QSlevelname;
            QJsonObject levelObjectsJsonObject = levelArray[levelindex].toObject();

            if (levelObjectsJsonObject.contains("levelname") && levelObjectsJsonObject["levelname"].isString())
                QSlevelname = levelObjectsJsonObject["levelname"].toString();



            if (levelObjectsJsonObject.contains("objects") && levelObjectsJsonObject["objects"].isArray())
            {
                QJsonArray objectsArray = levelObjectsJsonObject["objects"].toArray();

                for(int objectindex = 0; objectindex < objectsArray.size();objectindex++)
                {
                    QJsonObject singleObject = objectsArray[objectindex].toObject();
                    GameObject *gameObj {nullptr};
                    float x = 0,y = 0,z = 0, rotx = 0, roty = 0, rotz = 0,scalx = 0, scaly = 0, scalz = 0;


                    if (singleObject.contains("filepath") && singleObject["filepath"].isString())
                    {
                        if (singleObject.contains("usingLOD") && singleObject["usingLOD"].isBool())
                        {
                            if(singleObject.contains("texturename") && singleObject["texturename"].isString())
                            {

                            gameObj = CreateObject(singleObject["filepath"].toString().toStdString(),singleObject["usingLOD"].toBool(),singleObject["texturename"].toString().toStdString());

                            gameObj->mMeshComp->bUsingLOD = singleObject["usingLOD"].toBool();
                            }
                        }


                        //might not need this
                        gameObj->filepath = singleObject["filepath"].toString().toStdString();

                    }

                    if (singleObject.contains("positionx") && singleObject["positionx"].isDouble())
                        x = float(singleObject["positionx"].toDouble());

                    if (singleObject.contains("positiony") && singleObject["positiony"].isDouble())
                        y = float(singleObject["positiony"].toDouble());

                    if (singleObject.contains("positionz") && singleObject["positionz"].isDouble())
                        z = float(singleObject["positionz"].toDouble());

                    if (singleObject.contains("rotationx") && singleObject["rotationx"].isDouble())
                    {
                        rotx = float(singleObject["rotationx"].toDouble());
                        float x = rotx - gameObj->mTransformComp->mRotation.getX();
                        gameObj->mTransformComp->mRotation.setX(x+gameObj->mTransformComp->mRotation.getX());
                        gameObj->mTransformComp->mMatrix.rotateX(x);
                    }


                    if (singleObject.contains("rotationy") && singleObject["rotationy"].isDouble())
                    {
                        roty = float(singleObject["rotationy"].toDouble());
                        float y = roty - gameObj->mTransformComp->mRotation.getY();
                        gameObj->mTransformComp->mRotation.setY(y+gameObj->mTransformComp->mRotation.getY());
                        gameObj->mTransformComp->mMatrix.rotateY(y);
                    }

                    if (singleObject.contains("rotationz") && singleObject["rotationz"].isDouble())
                    {
                        rotz = float(singleObject["rotationz"].toDouble());
                        float z = rotz - gameObj->mTransformComp->mRotation.getZ();
                        gameObj->mTransformComp->mRotation.setZ(z+gameObj->mTransformComp->mRotation.getZ());
                        gameObj->mTransformComp->mMatrix.rotateZ(z);
                    }

                    if (singleObject.contains("scalex") && singleObject["scalex"].isDouble())
                    {
                        scalx =float(singleObject["scalex"].toDouble());
                    }

                    if (singleObject.contains("scaley") && singleObject["scaley"].isDouble())
                    {
                        scaly =float(singleObject["scaley"].toDouble());
                    }

                    if (singleObject.contains("scalez") && singleObject["scalez"].isDouble())
                    {
                        scalz =float(singleObject["scalez"].toDouble());
                    }

                    //scale
                    gameObj->mTransformComp->mMatrix.scale(1/gameObj->mTransformComp->mScale.getX(),
                                                           1/gameObj->mTransformComp->mScale.getY(),
                                                           1/gameObj->mTransformComp->mScale.getZ());
                    gameObj->mTransformComp->mMatrix.scale(scalx,scaly,scalz);
                    gameObj->mTransformComp->mScale.setX(scalx);
                    gameObj->mTransformComp->mScale.setY(scaly);
                    gameObj->mTransformComp->mScale.setZ(scalz);

                    //setposition
                    gameObj->mTransformComp->mMatrix.setPosition(x,y,z);


                    if (singleObject.contains("shader") && singleObject["shader"].isDouble())
                    {
                        //gameObj->mMaterialComp->mShaderProgram = gsl::Shaders(singleObject["shader"].toString().toInt());
                        gameObj->mMaterialComp->mShaderProgram = singleObject["shader"].toInt();
                    }


                    if(objectindex == 1)
                    {
                        player = gameObj;
                    }
                    if(objectindex == 2)
                        light = gameObj;

                    // if the level name chosen in editor is the same as the current in the JSON file, then add them to the array of objects that will be shown in the editor
                    // else it will push the objects in the mLevels map anyway.
                    if(QSlevelname == QString::fromStdString(levelName))
                    {
                        objects.push_back(gameObj);
                    }
                }

                mLevels.insert(std::pair<QString,std::vector<GameObject*>>(QSlevelname,objects));
            }
        }
    }
    loadFile.close();
}


int ResourceManager::CreateMaterial(std::string textureName)
{
    initializeOpenGLFunctions();

//    mTextures[textureIDcounter] = new Texture("hund.bmp");
//    glActiveTexture(GL_TEXTURE0 + textureIDcounter);
//    glBindTexture(GL_TEXTURE_2D, mTextures[textureIDcounter]->mGLTextureID);
//    textureIDcounter++;
//    mTextures[textureIDcounter] = new Texture("cocademon3.bmp");
//    glActiveTexture(GL_TEXTURE0 + textureIDcounter);
//    glBindTexture(GL_TEXTURE_2D, mTextures[textureIDcounter]->mGLTextureID);
//    textureIDcounter++;


    int tempInt{-1};

    auto foundMatIndex = mTextureMap.find(textureName);
    if(foundMatIndex != mTextureMap.end()){
        tempInt = foundMatIndex->second;
        //mTextureMap.insert(std::pair<std::string, MaterialComponent*>{textureFilepath,tempComp});
    }else{
        if (textureName.find(".bmp") != std::string::npos)
        {
            tempInt = textureIDcounter;
            //std::string tempName = textureFilepath.erase(0,25);
            mTextureNames.push_back(textureName);
            mVectorTextures.push_back(new Texture(textureName));
//            new Texture(textureFilepath);

            glActiveTexture(GL_TEXTURE0 + textureIDcounter);
            glBindTexture(GL_TEXTURE_2D, mVectorTextures.at(textureIDcounter)->mGLTextureID);
            mTextureMap.insert(std::pair<std::string, int>{textureName,tempInt});
            textureIDcounter++;
        }
    }
    if(tempInt == 0 || textureName == " " /*|| textureName == "plain"*/)
    {
        return 0;
    }
    return tempInt-1;
//    tempGO->mMaterialComp = new MaterialComponent();
    //    tempGO->mMaterialComp->mTextureUnit=0;
}

int ResourceManager::setMaterial(std::string textureName)
{
//    MaterialComponent *tempComp{nullptr};

//    auto foundMatIndex = mTextureMap.find(textureName);
//    if(foundMatIndex != mTextureMap.end()){
//        tempComp = foundMatIndex->second;
//        //mTextureMap.insert(std::pair<std::string, MaterialComponent*>{textureFilepath,tempComp});
//    }else{
//        if (textureFilepath.find(".bmp") != std::string::npos)
//        {
//            tempComp = new MaterialComponent();
//            //std::string tempName = textureFilepath.erase(0,25);
//            mTextures[textureIDcounter] = new Texture(textureFilepath);
//            glActiveTexture(GL_TEXTURE0 + textureIDcounter);
//            glBindTexture(GL_TEXTURE_2D, mTextures[textureIDcounter]->mGLTextureID);
//            tempComp->mShaderProgram = 2;
//            tempComp->mTextureUnit = textureIDcounter;
//            mTextureMap.insert(std::pair<std::string, MaterialComponent*>{textureFilepath,tempComp});

//            textureIDcounter++;
//        }
//    }
return 0;
}



void ResourceManager::setUpAllTextures()
{
    //Making default texture
//    CreateMaterial();

    //Regular .bmp textures read from file
    QDir tempDir((gsl::TextureFilePath).c_str());
    if(tempDir.exists())
    {
        QStringList filters;
        filters << "*.bmp";
        tempDir.setNameFilters(filters);


        //read all regular textures
        for(QFileInfo &var : tempDir.entryInfoList())
        {
            CreateMaterial(var.fileName().toStdString());
            qDebug () << "Reading texture: " << var.fileName();
        }
    }
    else
    {
        qDebug() << "ERROR reading texure Failed";
//        qDebug() << "*** ERROR reading textures *** : Asset-folder " <<
//                         gsl::TextureFilePath << " does not exist!";
    }
}

void ResourceManager::getAllMeshNames()
{
    QDir tempDir((gsl::MeshFilePath).c_str());
    if(tempDir.exists())
    {
        QStringList filters;
        filters << "*.obj";
        tempDir.setNameFilters(filters);

        //getting all the names
        for(QFileInfo &var : tempDir.entryInfoList())
        {
            mMeshNames.push_back(var.fileName().toStdString());
            qDebug () << "Reading texture: " << var.fileName();
        }
    }
    else
    {
        qDebug() << "ERROR reading texure Failed";
//        qDebug() << "*** ERROR reading textures *** : Asset-folder " <<
//                         gsl::TextureFilePath << " does not exist!";
    }
}



ResourceManager *ResourceManager::getInstance()
{
    if(!mInstance)
    {
        mInstance = new ResourceManager();
    }
    return mInstance;
}


void ResourceManager::createXYZ(MeshComponent *MeshComp)
{
    MeshComp->mVertices[0].push_back(Vertex{0,0,0,1,0,0});
    MeshComp->mVertices[0].push_back(Vertex{3,0,0,1,0,0});
    MeshComp->mVertices[0].push_back(Vertex{0,0,0,0,1,0});
    MeshComp->mVertices[0].push_back(Vertex{0,3,0,0,1,0});
    MeshComp->mVertices[0].push_back(Vertex{0,0,0,0,0,1});
    MeshComp->mVertices[0].push_back(Vertex{0,0,3,0,0,1});
}




int ResourceManager::objectIDcounter = 0;

