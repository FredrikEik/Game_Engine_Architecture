﻿#include "objectfactory.h"
#include "cube.h"
#include "triangle.h"
#include "xyz.h"
#include "objimport.h"
#include <QDebug>

#define EXISTS(x) storedMeshes.find(x) != storedMeshes.end()

ObjectFactory::ObjectFactory()
{

}

void ObjectFactory::createObject(std::string objectName)
{
    GameObject* willCreateObject;

    if(objectName == "Cube")
    {
        willCreateObject = new Cube;
        if (EXISTS("Cube"))
        {
            willCreateObject->setMeshComponent(static_cast<MeshComponent*>(storedMeshes["Cube"]));
        }
        willCreateObject->getTransformComp()->mMatrix.translateX(-0.5f);
    }

    else if (objectName == "Triangle")
    {
        willCreateObject = new Triangle;
        if (EXISTS("Triangle"))
        {
            willCreateObject->setMeshComponent(static_cast<MeshComponent*>(storedMeshes["Triangle"]));
        }
        willCreateObject->getTransformComp()->mMatrix.translateX(1.f);
    }

    else if (objectName == "Goat")
    {
        willCreateObject = new ObjImport("../GEA2021/Assets/goat.obj");
        if (EXISTS("Goat"))
        {
            willCreateObject->setMeshComponent(static_cast<MeshComponent*>(storedMeshes["Goat"]));
        }
        willCreateObject->getTransformComp()->mMatrix.translateX(2.f);
    }
    else
        return;

    /*Trying to create each new object at an added position so
     *  all objects is not in the same place. Needs some work..*/
//    willCreateObject->getTransformComp()->mMatrix.translateX(0.5f);
    willCreateObject->init();
    mGameObject.push_back(willCreateObject);
    return;
}

void ObjectFactory::saveMesh(std::string fileName, std::string nickName)
{
    if(EXISTS(nickName))
    {
        qDebug() << storedMeshes.size();
    }
    else
    {
        ObjImport *objImport;
        MeshComponent* newMesh = new MeshComponent();
        objImport->readFile(fileName/*, &newMesh->mVertices, &newMesh->mIndices*/);
        storedMeshes.emplace(std::pair<std::string, MeshComponent*>{nickName, newMesh});
    }
}
