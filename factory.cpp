#include "plane.h"
#include "mariocube.h"
#include "triangle.h"
#include "cube.h"
#include "camera.h"
#include "sphere.h"
#include "factory.h"
#include "objreader.h"
#include "constants.h"
#include "skybox.h"
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
        cubecounter++;
        objectToCreate->mObjectType = "Cube";
        objectToCreate->mObjectName = "Cube " + std::to_string(cubecounter);
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
        planecounter++;
        objectToCreate->mObjectType = "Plane";
        objectToCreate->mObjectName = "Plane " + std::to_string(planecounter);
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
        trianglecounter++;
        objectToCreate->mObjectType = "Triangle";
        objectToCreate->mObjectName = "Triangle " + std::to_string(trianglecounter);
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
        mariocounter++;
        objectToCreate->mObjectType = "MarioCube";
        objectToCreate->mObjectName = "MarioCube " + std::to_string(mariocounter);
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
        spherecounter++;
        objectToCreate->mObjectType = "Sphere";
        objectToCreate->mObjectName = "Sphere " + std::to_string(spherecounter);
    }
    else if(objectName == "Camera")
    {
        objectToCreate = new Camera(45, 4/3);
        objectToCreate->getMeshComponent();
        objectToCreate->getMaterialComponent()->mShaderProgram = 0;
        objectToCreate->getMaterialComponent()->mTextureUnit = 0;
        cameracounter++;
        objectToCreate->mObjectType = "Camera";
        objectToCreate->mObjectName = "Camera " + std::to_string(cameracounter);
    }
    else if(objectName == "Skybox")
    {
        objectToCreate = new Skybox("../GEA2021/Assets/skybox.obj");
        objectToCreate->getMeshComponent();
        objectToCreate->getMaterialComponent()->mShaderProgram = 2;
        objectToCreate->getMaterialComponent()->mTextureUnit = 2;

        //skyboxcounter++;
        objectToCreate->mObjectName = "Skybox";
    }

    else{return nullptr;}

    objectToCreate->init();
    objectToCreate->ID = mAvailableIDs.front(); //Give ID to GameObject
    mAvailableIDs.pop();                        //Pop ID from queue of available IDs

    mGameObjects.push_back(objectToCreate);
    return objectToCreate;
}

void Factory::saveMesh(std::string fileName, std::string nickName)
{
    if(EXISTS(nickName)){
        //qDebug() << storedMeshes.size();
    }
    else {
        ObjReader objReader;
        MeshComponent* newMesh = new MeshComponent();
        objReader.readFile(fileName, &newMesh->mVertices, &newMesh->mIndices);
        storedMeshes.insert(std::pair<std::string, MeshComponent*>{nickName, newMesh});
    }
}

void Factory::openLevel(Level level)
{
    std::multimap objects = level.objectsInLevel;

    for (auto it = objects.begin(); it != objects.end(); it++)
    {
        GameObject* temp = createObject(it->first);

        SpawnSettings settings = it->second;
        //Set initial location
        temp->move(settings.initialPos.getX(), settings.initialPos.getY(), settings.initialPos.getZ());
        //Set initial scale
        temp->getTransformComponent()->mMatrix.scale(settings.initialScale);
        //Set initial rotation
        temp->getTransformComponent()->mMatrix.rotateX(settings.initialRot.x);
        temp->getTransformComponent()->mMatrix.rotateY(settings.initialRot.y);
        temp->getTransformComponent()->mMatrix.rotateZ(settings.initialRot.z);
    }
}

