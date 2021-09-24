#include "plane.h"
#include "mariocube.h"
#include "triangle.h"
#include "cube.h"
#include "factory.h"
#include "bigplane.h"
#include "objreader.h"
#include <QDebug>

#define EXISTS(x) storedMeshes.find(x) != storedMeshes.end()

Factory::Factory()
{

}

void Factory::createObject(std::string objectName)
{

    GameObject* objectToCreate;

    if(objectName == "Cube")
    {
        objectToCreate = new Cube;
        if(EXISTS("Cube")) //If cube mesh exists
        {
        objectToCreate->setMeshComponent(static_cast<MeshComponent*>(storedMeshes["Cube"]));
        }
        objectToCreate->getMaterialComponent()->mShaderProgram = 1;
        objectToCreate->getMaterialComponent()->mTextureUnit = 1;
    }

    else if(objectName == "Plane")
    {
        objectToCreate = new Plane;
        if(EXISTS("Plane")) //If Plane mesh exists
        {
        objectToCreate->setMeshComponent(static_cast<MeshComponent*>(storedMeshes["Plane"]));
        }
        objectToCreate->getMaterialComponent()->mShaderProgram = 1;
        objectToCreate->getMaterialComponent()->mTextureUnit = 1;
    }

    else if(objectName == "Triangle")
    {
        objectToCreate = new Triangle;
        if(EXISTS("Triangle")) //If Triangle mesh exists
        {
        objectToCreate->setMeshComponent(static_cast<MeshComponent*>(storedMeshes["Triangle"]));
        }
        objectToCreate->getMaterialComponent()->mShaderProgram = 1;
        objectToCreate->getMaterialComponent()->mTextureUnit = 1;
    }

    else if(objectName == "MarioCube")
    {
        objectToCreate = new MarioCube;
        if(EXISTS("MarioCube")) //If MarioCube mesh exists
        {
        objectToCreate->setMeshComponent(static_cast<MeshComponent*>(storedMeshes["MarioCube"]));
        }
        objectToCreate->getMaterialComponent()->mShaderProgram = 1;
        objectToCreate->getMaterialComponent()->mTextureUnit = 1;
    }
    else if(objectName == "BigPlane")
    {
        objectToCreate = new BigPlane;
        if(EXISTS("BigPlane"))
        {
            objectToCreate->setMeshComponent(static_cast<MeshComponent*>(storedMeshes["BigPlane"]));
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
     if(EXISTS(nickName)){
        qDebug() << storedMeshes.size();
        }
        else {
            ObjReader objReader;
            MeshComponent* newMesh = new MeshComponent();
            objReader.readFile(fileName, &newMesh->mVertices, &newMesh->mIndices);
            storedMeshes.insert(std::pair<std::string, MeshComponent*>{nickName, newMesh});
        }
}
