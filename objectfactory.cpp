#include "objectfactory.h"
#include "cube.h"
#include "triangle.h"
#include "xyz.h"
#include "objimport.h"

#define EXISTS(x) storedMeshes.find(x), != storedMeshes.end()

ObjectFactory::ObjectFactory()
{

}

void ObjectFactory::createObject(std::string objectName)
{
    GameObject* willCreateObject;

    if(objectName == "Cube")
    {
        willCreateObject = new Cube;
//        if (EXISTS("Cube"))
//        {
//            willCreateObject->getMeshComp(static_cast<MeshComponent*>(storedMeshes["Cube"]));
//        }
    }

    else if (objectName == "Triangle")
    {
        willCreateObject = new Triangle;
    }

    else if (objectName == "Goat")
    {
        willCreateObject = new ObjImport("../GEA2021/Assets/goat.obj");
    }

//    else if (objectName == "xyz")
//    {
//        willCreateObject = new XYZ;
//    }
    else
        return;

    willCreateObject->init();
    mGameObject.push_back(willCreateObject);
    return;
}
