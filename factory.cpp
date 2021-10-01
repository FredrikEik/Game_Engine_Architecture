#include "plane.h"
#include "mariocube.h"
#include "triangle.h"
#include "cube.h"
#include "sphere.h"
#include "factory.h"
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
        static_cast<Cube*>(objectToCreate)->getBoxCollisionComponent()->max = gsl::Vector3D( 0.5f,  0.5f,  0.5f);
        static_cast<Cube*>(objectToCreate)->getBoxCollisionComponent()->min = gsl::Vector3D(-0.5f, -0.5f, -0.5f);
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
        static_cast<MarioCube*>(objectToCreate)->getBoxCollisionComponent()->max = gsl::Vector3D( 0.55f,  0.55f,  0.55f);
        static_cast<MarioCube*>(objectToCreate)->getBoxCollisionComponent()->min = gsl::Vector3D(-0.55f, -0.55f, -0.55f);
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
        static_cast<Sphere*>(objectToCreate)->getSphereCollisionComponent()->center = gsl::Vector3D( 0.0f,  0.0f,  0.0f);
        static_cast<Sphere*>(objectToCreate)->getSphereCollisionComponent()->radius = 0.25;
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
