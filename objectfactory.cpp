#include "objectfactory.h"
#include "cube.h"
#include "triangle.h"
#include "xyz.h"
#include "objimport.h"
#include "planeimport.h"
#include "octahedronball.h"
#include "rollingball.h"
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
    else if (objectName == "Plane")
    {
        if (EXISTS("Plane"))
        {
            willCreateObject = new PlaneImport("");
            willCreateObject->setMeshComponent(static_cast<MeshComponent*>(storedMeshes["Plane"]));
            qDebug() << "plane mesh extracted";
        }
        else
        {
            willCreateObject = new PlaneImport("../GEA2021/Assets/test_las.txt");
            storedMeshes.insert(std::pair("Plane", willCreateObject->getMeshComp()));
            qDebug() << "plane mesh saved";
        }
        p = willCreateObject;
        willCreateObject->mTexture = 1;
    }
    else if (objectName == "Ball")
    {
        willCreateObject = new OctahedronBall(5);
        qDebug() << "ball mesh created";
        willCreateObject->mTexture = 3;
    }
    else if (objectName == "RollingBall")
    {
        willCreateObject = new RollingBall(5, p);
        qDebug() << "RollingBall created";
        willCreateObject->mTexture = 3;
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
