﻿#include "objectfactory.h"
#include "cube.h"
#include "triangle.h"
#include "xyz.h"
#include "objimport.h"
#include "surface.h"
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
        willCreateObject->mTexture = 2;
        //mVisualObjects.push_back(new Surface(("../GEA2021/test_las.txt")));
    }

    else if (objectName == "Triangle")
    {
        willCreateObject = new Triangle;
        willCreateObject->mTexture = 1;
    }

    else if (objectName == "Goat")
    {
        if (EXISTS("Goat"))
        {
            willCreateObject = new ObjImport("");
            willCreateObject->setMeshComponent(static_cast<MeshComponent*>(storedMeshes["Goat"]));
            qDebug() << "goat mesh extracted";
        }
        else
        {
            willCreateObject = new ObjImport("../GEA2021/Assets/goat.obj");
            storedMeshes.insert(std::pair("Goat", willCreateObject->getMeshComp()));
            qDebug() << "goat mesh saved";
        }
        willCreateObject->mTexture = 3;
        willCreateObject->getCollisionComp()->max = gsl::Vector3D(0.3f, 1.4f, 0.5f);
        willCreateObject->getCollisionComp()->min = gsl::Vector3D(-0.3f, .1f, -0.5f);
    }
    else if (objectName == "Surface")
    {
            //willCreateObject = new ObjImport(".../GEA2021/terrainpoints.txt");
            //storedMeshes.insert(std::pair("Surface", willCreateObject->getMeshComp()));
            //mVisualObjects.push_back(new Surface(("../GEA2021/terrainpoints.txt")));
        if (EXISTS("Surface"))
        {
            willCreateObject = new Surface("");
            willCreateObject->setMeshComponent(static_cast<MeshComponent*>(storedMeshes["Surface"]));
            qDebug() << "surface mesh extracted";
        }
        else
        {
            willCreateObject = new Surface("../GEA2021/test_las.txt");
            storedMeshes.insert(std::pair("Surface", willCreateObject->getMeshComp()));
            qDebug() << "surface mesh saved";
        }
        willCreateObject->mTexture = 2;

    }
    else
        return;

    /*Trying to create each new object at an added position so
     *  all objects is not in the same place. Needs some work..*/
    //willCreateObject->getTransformComp()->mMatrix.translateX(+0.5f);
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
        ObjImport *objImport {nullptr};
        MeshComponent* newMesh = new MeshComponent();
        objImport->readFile(fileName/*, &newMesh->mVertices, &newMesh->mIndices*/);
        storedMeshes.emplace(std::pair<std::string, MeshComponent*>{nickName, newMesh});
    }
}
