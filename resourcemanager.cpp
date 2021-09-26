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
#include <QOpenGLFunctions>

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

GameObject* ResourceManager::CreateObject(std::string filepath, bool bUsingLOD)
{
    // Loops through all the objects, if it finds it it will create a new component with the same mesh component.
    // if it does not find it in the map, it will create a new object with a unique meshComp.

    // TO DO, fix the hardcoded matirial stuff.

    // IMPORTANT Need to fix the map so it stores all of them, now it stores only the last one with same name.
    // Change it to a vector maybe,



    for(auto obj : mObjectsMap)
    {
        if(obj.first.find(filepath) != std::string::npos) //if it found the filepath
        {
            tempGO = new GameObject();
            tempGO->mMeshComp = obj.second.mMeshComp;
            tempGO->mTransformComp = new TransformComponent();
            tempGO->mTransformComp->mMatrix.setToIdentity();
            tempGO->mMaterialComp = new MaterialComponent();
            //Burde ikke hardkode dette
            tempGO->mMaterialComp->mShaderProgram = 1;
            tempGO->mMaterialComp->mTextureUnit = 1;

            objectIDcounter++;
            mMeshHandler->readFile(filepath, tempGO->mMeshComp);
            filepath = filepath + std::to_string(objectIDcounter);
            //ResourceManager::init(*tempGO->mMeshComp);
            mObjectsMap.insert(std::pair<std::string, GameObject>{filepath,*tempGO});
            return tempGO;
        }
    }
        tempGO = new GameObject();
        tempGO->mMeshComp = new MeshComponent();
        tempGO->mTransformComp = new TransformComponent();
        tempGO->mTransformComp->mMatrix.setToIdentity();
        tempGO->mMaterialComp = new MaterialComponent();

        if(filepath == "xyz" || filepath == "XYZ")
        {
            createXYZ(tempGO->mMeshComp);
        }
        //Burde ikke hardkode dette
        tempGO->mMaterialComp->mShaderProgram = 1;
        tempGO->mMaterialComp->mTextureUnit = 1;

        mMeshHandler->readFile(filepath, tempGO->mMeshComp);
        //ResourceManager::init(*tempGO->mMeshComp);
        mObjectsMap.insert(std::pair<std::string, GameObject>{filepath,*tempGO});
        qDebug() << "Number of objects in map:" << mObjectsMap.size();
        return tempGO;

}

ResourceManager &ResourceManager::getInstance()
{
    static ResourceManager *mInstance = new ResourceManager();
    return *mInstance;
}

void ResourceManager::createXYZ(MeshComponent *MeshComp)
{
    MeshComp->mVertices.push_back(Vertex{0,0,0,1,0,0});
    MeshComp->mVertices.push_back(Vertex{3,0,0,1,0,0});
    MeshComp->mVertices.push_back(Vertex{0,0,0,0,1,0});
    MeshComp->mVertices.push_back(Vertex{0,3,0,0,1,0});
    MeshComp->mVertices.push_back(Vertex{0,0,0,0,0,1});
    MeshComp->mVertices.push_back(Vertex{0,0,3,0,0,1});
}



int ResourceManager::objectIDcounter = 0;

