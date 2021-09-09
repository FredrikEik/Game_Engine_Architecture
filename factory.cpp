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
        objectToCreate->getMaterialComponent()->mShaderProgram = 1;
        objectToCreate->getMaterialComponent()->mTextureUnit = 1;
    }
    else if(objectName == "Plane")
    {
        objectToCreate = new Plane;
        objectToCreate->getMaterialComponent()->mShaderProgram = 1;
        objectToCreate->getMaterialComponent()->mTextureUnit = 1;
    }
    else if(objectName == "Triangle")
    {
        objectToCreate = new Triangle;
        objectToCreate->getMaterialComponent()->mShaderProgram = 1;
        objectToCreate->getMaterialComponent()->mTextureUnit = 1;
    }
    else if(objectName == "MarioCube")
    {
        objectToCreate = new MarioCube;
        objectToCreate->getMaterialComponent()->mShaderProgram = 1;
        objectToCreate->getMaterialComponent()->mTextureUnit = 1;
    }
    else{return;}


    objectToCreate->init();
    mGameObjects.push_back(objectToCreate);
    return;
}

    void ResourceManager::saveMesh(std::string fileName, std::string nickName)
    {
    if(storedMeshes.find(nickName) == storedMeshes.end()){
        ObjReader objReader;
        MeshComponent* newMesh = new MeshComponent();
        objReader.readFile(fileName, &newMesh->mVertices, &newMesh->mIndices);
        storedMeshes.insert(std::pair<std::string, MeshComponent*>{nickName, newMesh});
    }
    else if(storedMeshes.find(nickName) != storedMeshes.end()){
        qDebug() << "Herroin";
        qDebug() << storedMeshes.size();
        }
}
