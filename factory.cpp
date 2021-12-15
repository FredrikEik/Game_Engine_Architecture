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
#include <QDebug>

#define EXISTS(x) storedMeshes.find(x) != storedMeshes.end()

Factory::Factory()
{
    for (uint32_t ID = 0; ID < gsl::MAX_OBJECTS; ++ID)
    {
        mAvailableIDs.push(ID);
    }
}
template <class T>
T* Factory::createObject(gsl::ObjectType objectType)
{
    //Doesn't work
    T* newObject;

    switch(objectType)
    {
    case gsl::CUBE:
        newObject = new Cube;
        newObject->getMaterialComponent()->mShaderProgram = 1;
        newObject->getMaterialComponent()->mTextureUnit = 1;
        newObject->getSphereCollisionComponent()->center = gsl::Vector3D( 0.0f,  0.0f,  0.0f);
        newObject->getSphereCollisionComponent()->radius = 0.7;
        cubecounter++;
        newObject->mObjectType = "Cube";
        newObject->mObjectName = "Cube " + std::to_string(cubecounter);
        break;

    case gsl::PLANE:
        newObject = new Plane;
        newObject->getMaterialComponent()->mShaderProgram = 2;
        newObject->getMaterialComponent()->mTextureUnit = 1;
        planecounter++;
        newObject->mObjectType = "Plane";
        newObject->mObjectName = "Plane " + std::to_string(planecounter);
        //static_cast<Plane*>(objectToCreate)->init();
        break;

    case gsl::TRIANGLE:
        newObject = new Triangle;
        newObject->getMaterialComponent()->mShaderProgram = 1;
        newObject->getMaterialComponent()->mTextureUnit = 1;
        trianglecounter++;
        newObject->mObjectType = "Triangle";
        newObject->mObjectName = "Triangle " + std::to_string(trianglecounter);
        break;

    case gsl::MARIOCUBE:
        newObject = new MarioCube;
        if(EXISTS("MarioCube")) //If MarioCube mesh exists
        {
            newObject->setMeshComponent(static_cast<MeshComponent*>(storedMeshes["MarioCube"]));
        }
        newObject->getMaterialComponent()->mShaderProgram = 1;
        newObject->getMaterialComponent()->mTextureUnit = 1;
        newObject->getSphereCollisionComponent()->center = gsl::Vector3D( 0.0f,  0.0f,  0.0f);
        newObject->getSphereCollisionComponent()->radius = 0.5;
        mariocounter++;
        newObject->mObjectType = "MarioCube";
        newObject->mObjectName = "MarioCube " + std::to_string(mariocounter);
        //static_cast<MarioCube*>(objectToCreate)->init();
        break;

    case gsl::SPHERE:
        newObject = new Sphere;
        if(EXISTS("Sphere")) //If Sphere mesh exists
        {
            newObject->setMeshComponent(static_cast<MeshComponent*>(storedMeshes["Sphere"]));
        }
        newObject->getMaterialComponent()->mShaderProgram = 2;
        newObject->getMaterialComponent()->mTextureUnit = 1;
        newObject->getSphereCollisionComponent()->center = gsl::Vector3D( 0.0f,  0.0f,  0.0f);
        newObject->getSphereCollisionComponent()->radius = 0.25;
        spherecounter++;
        newObject->mObjectType = "Sphere";
        newObject->mObjectName = "Sphere " + std::to_string(spherecounter);
        //static_cast<Sphere*>(objectToCreate)->init();
        break;

    case gsl::CAMERA:
        newObject = new Camera(45, 4/3);
        newObject->getMaterialComponent()->mShaderProgram = 0;
        newObject->getMaterialComponent()->mTextureUnit = 0;
        cameracounter++;
        newObject->mObjectType = "Camera";
        newObject->mObjectName = "Camera " + std::to_string(cameracounter);
        //static_cast<Camera*>(objectToCreate)->init();
        break;

    case gsl::SKYBOX:
        newObject = new Skybox("../GEA2021/Assets/skybox.obj");
        newObject->getMaterialComponent()->mShaderProgram = 3;
        newObject->getMaterialComponent()->mTextureUnit = 2;
        skyboxcounter++;
        newObject->mObjectType = "Skybox";
        newObject->mObjectName = "Skybox";
        break;
    case gsl::LIGHT:
        newObject = new Light;
        if(EXISTS("Light")) //If Light mesh exists
        {
        newObject->setMeshComponent(static_cast<MeshComponent*>(storedMeshes["Light"]));
        }
        newObject->getMaterialComponent()->mShaderProgram = 1;
        newObject->getMaterialComponent()->mTextureUnit = 1;
        lightCounter++;
        newObject->mObjectType = "Light";
        newObject->mObjectName = "Light " + std::to_string(lightCounter);
        //static_cast<Light*>(objectToCreate)->init();
        break;

    case gsl::PLAYER:
        newObject = new Player;
        if(EXISTS("Wario"))
        {
        newObject->setMeshComponent(static_cast<MeshComponent*>(storedMeshes["Wario"]));
        }
        newObject->getMaterialComponent()->mShaderProgram = 1;
        newObject->getMaterialComponent()->mTextureUnit = 0;
        newObject->mObjectName = "Player";
        //static_cast<Player*>(objectToCreate)->init();
        break;

    case gsl::TRIANGLESURFACE:
        newObject = new TriangleSurface("../GEA2021/Assets/test_las.txt");
        //objectToCreate = new TriangleSurface("../GEA2021/Assets/LAS/kongehuset.txt");
        newObject->getMaterialComponent()->mShaderProgram = 2;
        newObject->getMaterialComponent()->mTextureUnit = 5;
        newObject->mObjectType = "TriangleSurface";
        newObject->mObjectName = "TriangleSurface";
        //static_cast<TriangleSurface*>(objectToCreate)->init();
        break;
    default:
        break;
    }
    newObject->init();
    return newObject;
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
        objectToCreate->getMaterialComponent()->mTextureUnit = 2;

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
        //static_cast<Player*>(objectToCreate)->init();
        break;

    case gsl::TRIANGLESURFACE:
        objectToCreate = new TriangleSurface("../GEA2021/Assets/test_las.txt");
        //objectToCreate = new TriangleSurface("../GEA2021/Assets/LAS/kongehuset.txt");
        objectToCreate->getMaterialComponent()->mShaderProgram = 2;
        objectToCreate->getMaterialComponent()->mTextureUnit = 5;
        objectToCreate->mObjectType = gsl::TRIANGLESURFACE;
        objectToCreate->mObjectName = "TriangleSurface";
        //static_cast<TriangleSurface*>(objectToCreate)->init();
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

