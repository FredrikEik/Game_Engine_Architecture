﻿#include "objectfactory.h"
#include "cube.h"
#include "triangle.h"
#include "xyz.h"
#include "objimport.h"
#include <QDebug>

#define EXISTS(x) storedMeshes.find(x) != storedMeshes.end()

ObjectFactory::ObjectFactory()
{
    setOBJindex(-1);
}

void ObjectFactory::createObject(std::string objectName)
{
    GameObject* willCreateObject;

    if(objectName == "Cube")
    {
        willCreateObject = new Cube;
        willCreateObject->getCollisionComp()->max = gsl::Vector3D(0.5f, 0.5f, 0.5f);
        willCreateObject->getCollisionComp()->min = gsl::Vector3D(-0.5f, -0.5f, -0.5f);

        if (EXISTS("Cube"))
        {
            willCreateObject->setMeshComponent(static_cast<MeshComponent*>(storedMeshes["Cube"]));
            qDebug() << "mesh extracted";
        }
        else
        {
            storedMeshes.insert(std::pair("Cube", willCreateObject->getMeshComp()));
            qDebug() << "mesh saved";
        }
    }

    else if (objectName == "Triangle")
    {
        willCreateObject = new Triangle;
        if (EXISTS("Triangle"))
        {
            willCreateObject->setMeshComponent(static_cast<MeshComponent*>(storedMeshes["Triangle"]));
            qDebug() << "triangle mesh extracted";
        }
        else
        {
            storedMeshes.insert(std::pair("Triangle", willCreateObject->getMeshComp()));
            qDebug() << "triangle mesh saved";
        }
    }

    else if (objectName == "Goat")
    {
        willCreateObject = new ObjImport("../GEA2021/Assets/goat.obj");
        if (EXISTS("Goat"))
        {
            willCreateObject->setMeshComponent(static_cast<MeshComponent*>(storedMeshes["Goat"]));
            qDebug() << "goat mesh extracted";
        }
        else
        {
            storedMeshes.insert(std::pair("Goat", willCreateObject->getMeshComp()));
            qDebug() << "goat mesh saved";
        }
    }
    else
        return;

    /*Trying to create each new object at an added position so
     *  all objects is not in the same place. Needs some work..*/
    willCreateObject->getTransformComp()->mMatrix.translateX(+0.5f);
    willCreateObject->getTransformComp()->mTrueScaleMatrix = willCreateObject->getTransformComp()->mMatrix; // important do not delete
    willCreateObject->init();
    mGameObject.push_back(willCreateObject);

    return;
}

void ObjectFactory::saveMesh(std::string fileName, std::string nickName)
{
    if(EXISTS(nickName)){
        qDebug() << storedMeshes.size();
    }
    else {
        ObjImport *objImport;
        MeshComponent* newMesh = new MeshComponent();
        objImport->readFile(fileName/*, &newMesh->mVertices, &newMesh->mIndices*/);
        storedMeshes.emplace(std::pair<std::string, MeshComponent*>{nickName, newMesh});
    }
}
