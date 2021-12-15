#include "plane.h"
#include "mariocube.h"
#include "triangle.h"
#include "cube.h"
#include "camera.h"
#include "sphere.h"
#include "light.h"
#include "factory.h"
#include "objreader.h"
#include "constants.h"
#include "skybox.h"
#include "trianglesurface.h"
#include "player.h"
#include "billboard.h"
#include <QDebug>

#define EXISTS(x) storedMeshes.find(x) != storedMeshes.end()

Factory::Factory()
{
    for (uint32_t ID = 0; ID < gsl::MAX_OBJECTS; ++ID)
    {
        mAvailableIDs.push(ID);
    }
}

GameObject* Factory::createObject(gsl::ObjectType objectType)
{
    GameObject *objectToCreate;

    switch(objectType)
    {
    case gsl::CUBE:
        objectToCreate = new Cube;
        if(EXISTS("Cube")) //If cube mesh exists
        {
            objectToCreate->setMeshComponent(static_cast<MeshComponent*>(storedMeshes["Cube"]));
        }
        objectToCreate->getMaterialComponent()->mShaderProgram = 1;
        objectToCreate->getMaterialComponent()->mTextureUnit = 1;
        objectToCreate->getSphereCollisionComponent()->center = gsl::Vector3D( 0.0f,  0.0f,  0.0f);
        objectToCreate->getSphereCollisionComponent()->radius = 0.7;
        cubecounter++;
        objectToCreate->mObjectType = gsl::CUBE;
        objectToCreate->mObjectName = "Cube " + std::to_string(cubecounter);
        //static_cast<Cube*>(objectToCreate)->init();
        break;
    case gsl::PLANE:
        objectToCreate = new Plane;
        if(EXISTS("Plane")) //If Plane mesh exists
        {
            objectToCreate->setMeshComponent(static_cast<MeshComponent*>(storedMeshes["Plane"]));
        }
        objectToCreate->getMaterialComponent()->mShaderProgram = 2;
        objectToCreate->getMaterialComponent()->mTextureUnit = 1;
        planecounter++;
        objectToCreate->mObjectType = gsl::PLANE;
        objectToCreate->mObjectName = "Plane " + std::to_string(planecounter);
        //static_cast<Plane*>(objectToCreate)->init();
        break;
    case gsl::TRIANGLE:
        objectToCreate = new Triangle;
        if(EXISTS("Triangle")) //If Triangle mesh exists
        {
            objectToCreate->setMeshComponent(static_cast<MeshComponent*>(storedMeshes["Triangle"]));
        }
        objectToCreate->getMaterialComponent()->mShaderProgram = 1;
        objectToCreate->getMaterialComponent()->mTextureUnit = 1;
        trianglecounter++;
        objectToCreate->mObjectType = gsl::TRIANGLE;
        objectToCreate->mObjectName = "Triangle " + std::to_string(trianglecounter);
        //static_cast<Triangle*>(objectToCreate)->init();
        break;

    case gsl::MARIOCUBE:

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
        objectToCreate->mObjectType = gsl::MARIOCUBE;
        objectToCreate->mObjectName = "MarioCube " + std::to_string(mariocounter);
        //static_cast<MarioCube*>(objectToCreate)->init();
        break;

    case gsl::SPHERE:
        objectToCreate = new Sphere;
        if(EXISTS("Sphere")) //If Sphere mesh exists
        {
            objectToCreate->setMeshComponent(static_cast<MeshComponent*>(storedMeshes["Sphere"]));
        }
        objectToCreate->getMaterialComponent()->mShaderProgram = 2;
        objectToCreate->getMaterialComponent()->mTextureUnit = 1;
        objectToCreate->getSphereCollisionComponent()->center = gsl::Vector3D( 0.0f,  0.0f,  0.0f);
        objectToCreate->getSphereCollisionComponent()->radius = 0.25;
        spherecounter++;
        objectToCreate->mObjectType = gsl::SPHERE;
        objectToCreate->mObjectName = "Sphere " + std::to_string(spherecounter);
        //static_cast<Sphere*>(objectToCreate)->init();

    case gsl::CAMERA:

        objectToCreate = new Camera(45, 4/3);
        objectToCreate->getMaterialComponent()->mShaderProgram = 0;
        objectToCreate->getMaterialComponent()->mTextureUnit = 0;
        cameracounter++;
        objectToCreate->mObjectType = gsl::CAMERA;
        objectToCreate->mObjectName = "Camera " + std::to_string(cameracounter);
        //static_cast<Camera*>(objectToCreate)->init();
        break;
    case gsl::SKYBOX:
        objectToCreate = new Skybox("../GEA2021/Assets/skybox.obj");
        objectToCreate->getMaterialComponent()->mShaderProgram = 3;
        objectToCreate->getMaterialComponent()->mTextureUnit = 9;

        //skyboxcounter++;
        objectToCreate->mObjectType = gsl::SKYBOX;
        objectToCreate->mObjectName = "Skybox";
        //static_cast<Skybox*>(objectToCreate)->init();
        break;

    case gsl::LIGHT:
        objectToCreate = new Light;
        if(EXISTS("Light")) //If Light mesh exists
        {
        objectToCreate->setMeshComponent(static_cast<MeshComponent*>(storedMeshes["Light"]));
        }
        objectToCreate->getMaterialComponent()->mShaderProgram = 1;
        objectToCreate->getMaterialComponent()->mTextureUnit = 1;
        lightCounter++;
        objectToCreate->mObjectType = gsl::LIGHT;
        objectToCreate->mObjectName = "Light " + std::to_string(lightCounter);
        //static_cast<Light*>(objectToCreate)->init();
        break;
    case gsl::PLAYER:
        objectToCreate = new Player;
        if(EXISTS("Wario"))
        {
        objectToCreate->setMeshComponent(static_cast<MeshComponent*>(storedMeshes["Wario"]));
        }
        objectToCreate->getMaterialComponent()->mShaderProgram = 1;
        objectToCreate->getMaterialComponent()->mTextureUnit = 0;
        objectToCreate->mObjectType = gsl::PLAYER;
        objectToCreate->mObjectName = "Player";
		break;
	case gsl::TRIANGLESURFACE:
        objectToCreate = new TriangleSurface("../GEA2021/Assets/test_las.txt");
        //objectToCreate = new TriangleSurface("../GEA2021/Assets/LAS/kongehuset.txt");
        if(EXISTS("TriangleSurface"))
        {
        objectToCreate->setMeshComponent(static_cast<MeshComponent*>(storedMeshes["TriangleSurface"]));
        }
        objectToCreate->getMaterialComponent()->mShaderProgram = 2;
        objectToCreate->getMaterialComponent()->mTextureUnit = 3;
        objectToCreate->mObjectType = gsl::TRIANGLESURFACE;
        objectToCreate->mObjectName = "TriangleSurface";
		break;
	case gsl::BILLBOARD:
		objectToCreate = new Billboard;
        if(EXISTS("Billboard"))
        {
            objectToCreate->setMeshComponent(static_cast<MeshComponent*>(storedMeshes["Billboard"]));
        }
        objectToCreate->getMaterialComponent()->mShaderProgram = 1;
        objectToCreate->getMaterialComponent()->mTextureUnit = 8;
        objectToCreate->isHUD = true;
		objectToCreate->mObjectType = gsl::BILLBOARD;
        objectToCreate->mObjectName = "HUD";
        break;
    default:
        return nullptr;
		
    }

    objectToCreate->init();
    objectToCreate->ID = mAvailableIDs.front(); //Give ID to GameObject
    mAvailableIDs.pop();                        //Pop ID from queue of available IDs
    //gameObjects.push_back(objectToCreate);
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
        temp->getTransformComponent()->mMatrix.setScale(settings.initialScale.x, settings.initialScale.y, settings.initialScale.z);
        //Set initial rotation
        temp->getTransformComponent()->mMatrix.setRotation(settings.initialRot.x, settings.initialRot.y, settings.initialRot.z);
    }
}

