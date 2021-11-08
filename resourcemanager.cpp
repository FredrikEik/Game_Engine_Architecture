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
#include "constants.h"

#include <QOpenGLFunctions>

#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
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

GameObject* ResourceManager::CreateObject(std::string filepath, bool UsingLOD)
{
    // Loops through all the objects, if it finds it it will create a new component with the same mesh component.
    // if it does not find it in the map, it will create a new object with a unique meshComp.

    // TO DO, fix the hardcoded matirial stuff.

    // IMPORTANT Need to fix the map so it stores all of them, now it stores only the last one with same name.
    // Change it to a vector maybe,


    tempGO = new GameObject();

    tempGO->filepath = filepath;

    //find the first version of the object, hence "1"vv

    tempGO->mMaterialComp = new MaterialComponent();
    tempGO->mMaterialComp->mTextureUnit=0;
    tempGO->mTransformComp = new TransformComponent();
    tempGO->mTransformComp->mMatrix.setToIdentity();



    auto foundAtIndex = mObjectsMap.find(filepath);


    if(foundAtIndex != mObjectsMap.end()){
        tempGO->mMeshComp = foundAtIndex->second.mMeshComp;
        tempGO->mCollisionComp = foundAtIndex->second.mCollisionComp;
        tempGO->mCollisionLines = foundAtIndex->second.mCollisionLines;
        mObjectsMap.insert(std::pair<std::string, GameObject>{filepath + std::to_string(objectIDcounter) ,*tempGO});
    }else{
        tempGO->mMeshComp = new MeshComponent();
        tempGO->mMeshComp->bUsingLOD = UsingLOD;
        tempGO->mCollisionLines = new MeshComponent();
        tempGO->mCollisionComp = new CollisionComponent();
        mObjectsMap.insert(std::pair<std::string, GameObject>{filepath ,*tempGO});
        if(UsingLOD)
        {
            mMeshHandler->readFile(filepath, tempGO->mMeshComp, 0, tempGO->mCollisionComp,tempGO->mCollisionLines );
            mMeshHandler->readFile(filepath, tempGO->mMeshComp, 1, tempGO->mCollisionComp,tempGO->mCollisionLines );
            mMeshHandler->readFile(filepath, tempGO->mMeshComp, 2, tempGO->mCollisionComp,tempGO->mCollisionLines );
        }else{
            mMeshHandler->readFile(filepath, tempGO->mMeshComp, 0, tempGO->mCollisionComp,tempGO->mCollisionLines );
        }
        meshCompCounter++;
    }


    //Not working atm
    if(filepath == "xyz" || filepath == "XYZ")
    {
        createXYZ(tempGO->mMeshComp);
    }

    //Burde ikke hardkode dette
    tempGO->mMaterialComp->mShaderProgram = 0;
    tempGO->mMaterialComp->mTextureUnit = 0;



    std::string tempName = filepath;
    // Hardcoded filesize just to make "XYZ" work
//    if(filepath.size() > 15)
//    {
        tempName.erase(0,25);
        tempName.erase(tempName.end()-4,tempName.end());
//    }
    tempGO->name = tempName + " ID: " + std::to_string(objectIDcounter);

    tempGO->id = objectIDcounter;


    //qDebug() << "Number of objects in map:" << mObjectsMap.size();
    //qDebug() << "Number of unique meshcomps:" << meshCompCounter;
    objectIDcounter++;
    return tempGO;
}

void ResourceManager::saveScene(std::vector<GameObject *> &objects)
{
    if(mLevels.empty())
    {
        mLevels.push_back(std::pair<QString,std::vector<GameObject*>>("level1",objects));
    }
    QFile saveFile(QStringLiteral("save.json"));

    // open file
    if (!saveFile.open(QIODevice::WriteOnly)) {
        qWarning("Couldn't open save file.");
        return;
    }
    QJsonObject coreJsonObject;

    QJsonArray levelArray;
    for(unsigned long long i = 0; i < mLevels.size();i++)
    {
        QJsonObject level;
        QJsonArray objectarray;
        level["levelname"] = mLevels[i].first;
        for(auto it : objects)
        {
            QJsonObject levelObjects;
            levelObjects["filepath"] = it->filepath.c_str();
            // transform
            levelObjects["positionx"] = double(it->mTransformComp->mMatrix.getPosition().x);
            levelObjects["positiony"] = double(it->mTransformComp->mMatrix.getPosition().y);
            levelObjects["positionz"] = double(it->mTransformComp->mMatrix.getPosition().z);

            levelObjects["shader"] = int(it->mMaterialComp->mShaderProgram);

            objectarray.append(levelObjects);
        }
        level["objects"] = objectarray;
        levelArray.append(level);
    }
    coreJsonObject["levels"] = levelArray;
    saveFile.write(QJsonDocument(coreJsonObject).toJson());
    saveFile.close();
}

void ResourceManager::loadScene(std::vector<GameObject *> &objects, GameObject* &player, GameObject* &light)
{
    QFile loadFile(QStringLiteral("save.json"));

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
        for(int levelindex = 0; levelindex < levelArray.size();levelindex++)
        {
            QJsonObject levelObjectsJsonObject = levelArray[levelindex].toObject();
            QString levelname;

            if (levelObjectsJsonObject.contains("levelname") && levelObjectsJsonObject["levelname"].isString())
                levelname = levelObjectsJsonObject["levelname"].toString();

            if (levelObjectsJsonObject.contains("objects") && levelObjectsJsonObject["objects"].isArray())
            {
                QJsonArray objectsArray = levelObjectsJsonObject["objects"].toArray();
                objects.clear();

                for(int objectindex = 0; objectindex < objectsArray.size();objectindex++)
                {
                    QJsonObject singleObject = objectsArray[objectindex].toObject();
                    GameObject *gameObj {nullptr};
                    float x = 0,y = 0,z = 0;

                    if (singleObject.contains("filepath") && singleObject["filepath"].isString())
                    {
                        gameObj = CreateObject(singleObject["filepath"].toString().toStdString());

                        //might not need this
                        gameObj->filepath = singleObject["filepath"].toString().toStdString();

                    }

                    if (singleObject.contains("positionx") && singleObject["positionx"].isDouble())
                        x = float(singleObject["positionx"].toDouble());

                    if (singleObject.contains("positiony") && singleObject["positiony"].isDouble())
                        y = float(singleObject["positiony"].toDouble());

                    if (singleObject.contains("positionz") && singleObject["positionz"].isDouble())
                        z = float(singleObject["positionz"].toDouble());

                    gameObj->mTransformComp->mMatrix.setPosition(x,y,z);

                    if (singleObject.contains("shader") && singleObject["shader"].isDouble())
                    {
                        //gameObj->mMaterialComp->mShaderProgram = gsl::Shaders(singleObject["shader"].toString().toInt());
                        gameObj->mMaterialComp->mShaderProgram = singleObject["shader"].toInt();
                    }

                    if(objectindex == 0)
                    {
                        player = gameObj;
                    }
                    if(objectindex == 1)
                        light = gameObj;

                    objects.push_back(gameObj);
                }
                mLevels.push_back(std::pair<QString,std::vector<GameObject*>>(levelname,objects));
            }
        }
    }
    loadFile.close();
}

ResourceManager &ResourceManager::getInstance()
{
    static ResourceManager *mInstance = new ResourceManager();
    return *mInstance;
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

