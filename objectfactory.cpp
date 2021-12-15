#include "objectfactory.h"
#include "cube.h"
#include "triangle.h"
#include "xyz.h"
#include "objimport.h"
#include <QDebug>
#include "text2d.h"

#define EXISTS(x) storedMeshes.find(x) != storedMeshes.end()
/**
 * @brief ObjectFactory::ObjectFactory
 */
ObjectFactory::ObjectFactory()
{
    setOBJindex(-1);
}

/**
 * @brief ObjectFactory::createObject
 * @param objectName
 */
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
        willCreateObject->getCollisionComp()->max = gsl::Vector3D(0.3f, 1.4f, 0.5f); //sets collision box max values
        willCreateObject->getCollisionComp()->min = gsl::Vector3D(-0.3f, .1f, -0.5f); //sets collision box min values
    }
    else if (objectName == "HudSquare")
    {
        willCreateObject = new Cube;
        willCreateObject->mTexture = 1;
        willCreateObject->bHudElement = true;
    }
    else if (objectName == "HudText")
    {
        willCreateObject = new Text2D("SCORE:", 0, 190);
    }
    else
        return;

    willCreateObject->getTransformComp()->mTrueScaleMatrix = willCreateObject->getTransformComp()->mMatrix; // important do not delete
    willCreateObject->init();
    mGameObject.push_back(willCreateObject);

    return;
}

/**
 * @brief ObjectFactory::saveMesh
 * @param fileName
 * @param nickName
 */
void ObjectFactory::saveMesh(std::string fileName, std::string nickName)
{
    if(EXISTS(nickName)){
        qDebug() << storedMeshes.size();
    }
    else {
        ObjImport *objImport {nullptr};
        MeshComponent* newMesh = new MeshComponent();
        objImport->readFile(fileName);
        storedMeshes.emplace(std::pair<std::string, MeshComponent*>{nickName, newMesh});
    }
}
