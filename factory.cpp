#include "plane.h"
#include "mariocube.h"
#include "triangle.h"
#include "cube.h"
#include "factory.h"
#include "objreader.h"
#include <QDebug>


Factory::Factory()
{

}

void Factory::createObject(std::string objectName)
{
    GameObject* objectToCreate;

    if(objectName == "Cube")
    {
        objectToCreate = new Cube;
        if(storedMeshes.find("Cube") != storedMeshes.end()) //If cube mesh exists
        {
        objectToCreate->setMeshComponent(static_cast<MeshComponent*>(storedMeshes["Cube"]));
        }
        objectToCreate->getMaterialComponent()->mShaderProgram = 1;
        objectToCreate->getMaterialComponent()->mTextureUnit = 1;
    }

    else if(objectName == "Plane")
    {
        objectToCreate = new Plane;
        if(storedMeshes.find("Plane") != storedMeshes.end()) //If Plane mesh exists
        {
        objectToCreate->setMeshComponent(static_cast<MeshComponent*>(storedMeshes["Plane"]));
        }
        objectToCreate->getMaterialComponent()->mShaderProgram = 1;
        objectToCreate->getMaterialComponent()->mTextureUnit = 1;
    }

    else if(objectName == "Triangle")
    {
        objectToCreate = new Triangle;
        if(storedMeshes.find("Triangle") != storedMeshes.end()) //If Triangle mesh exists
        {
        objectToCreate->setMeshComponent(static_cast<MeshComponent*>(storedMeshes["Triangle"]));
        }
        objectToCreate->getMaterialComponent()->mShaderProgram = 1;
        objectToCreate->getMaterialComponent()->mTextureUnit = 1;
    }

    else if(objectName == "MarioCube")
    {
        objectToCreate = new MarioCube;
        if(storedMeshes.find("MarioCube") != storedMeshes.end()) //If MarioCube mesh exists
        {
        objectToCreate->setMeshComponent(static_cast<MeshComponent*>(storedMeshes["MarioCube"]));
        }
        objectToCreate->getMaterialComponent()->mShaderProgram = 1;
        objectToCreate->getMaterialComponent()->mTextureUnit = 1;
    }
    else{return;}


    objectToCreate->init();
    mGameObjects.push_back(objectToCreate);
    return;
}

    void Factory::saveMesh(std::string fileName, std::string nickName)
    {
    if(storedMeshes.find(nickName) == storedMeshes.end()){
        ObjReader objReader;
        MeshComponent* newMesh = new MeshComponent();
        objReader.readFile(fileName, &newMesh->mVertices, &newMesh->mIndices);
        storedMeshes.insert(std::pair<std::string, MeshComponent*>{nickName, newMesh});
    }
//    else if(storedMeshes.find(nickName) != storedMeshes.end()){
//        qDebug() << storedMeshes.size();

//        }
}
