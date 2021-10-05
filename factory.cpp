#include "plane.h"
#include "mariocube.h"
#include "triangle.h"
#include "cube.h"
#include "camera.h"
#include "sphere.h"
#include "factory.h"
#include "objreader.h"
#include "constants.h"
#include <QDebug>

#define EXISTS(x) storedMeshes.find(x) != storedMeshes.end()

Factory::Factory()
{
    for (uint32_t ID = 0; ID < gsl::MAX_OBJECTS; ++ID)
    {
        mAvailableIDs.push(ID);
    }
}

GameObject* Factory::createObject(std::string objectName)
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
        objectToCreate->getSphereCollisionComponent()->center = gsl::Vector3D( 0.0f,  0.0f,  0.0f);
        objectToCreate->getSphereCollisionComponent()->radius = 0.5;
        objectToCreate->move(0.0f, 0.0f, -35.f);
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
        objectToCreate->getSphereCollisionComponent()->center = gsl::Vector3D( 0.0f,  0.0f,  0.0f);
        objectToCreate->getSphereCollisionComponent()->radius = 0.5;
    }

    else if(objectName == "Sphere")
    {
        objectToCreate = new Sphere;
        if(EXISTS("Sphere")) //If Sphere mesh exists
        {
        objectToCreate->setMeshComponent(static_cast<MeshComponent*>(storedMeshes["Sphere"]));
        }
        objectToCreate->getMaterialComponent()->mShaderProgram = 1;
        objectToCreate->getMaterialComponent()->mTextureUnit = 1;
        objectToCreate->getSphereCollisionComponent()->center = gsl::Vector3D( 0.0f,  0.0f,  0.0f);
        objectToCreate->getSphereCollisionComponent()->radius = 0.25;
    }
    else if(objectName == "Camera")
    {
        objectToCreate = new Camera(45, 4/3);
        objectToCreate->getMeshComponent();
        objectToCreate->getMaterialComponent()->mShaderProgram = 0;
        objectToCreate->getMaterialComponent()->mTextureUnit = 0;
    }
    else{return nullptr;}

    objectToCreate->init();
    objectToCreate->ID = mAvailableIDs.front(); //Give ID to GameObject
    mAvailableIDs.pop();                        //Pop ID from queue of available IDs

    mGameObjects.insert(std::pair<uint32_t, GameObject*>{objectToCreate->ID, objectToCreate});
    return objectToCreate;
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
