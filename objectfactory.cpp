#include "objectfactory.h"
#include "cube.h"
#include "triangle.h"
#include "objimport.h"

ObjectFactory::ObjectFactory()
{

}

void ObjectFactory::createObject(std::string objectName)
{
    GameObject* willCreateObject;

    if(objectName == "Cube")
    {
        willCreateObject = new Cube;
    }

    else if (objectName == "Triangle")
    {
        willCreateObject = new Triangle;
    }

    else if (objectName == "Goat")
    {
        willCreateObject = new ObjImport("../GEA2021/Assets/goat.obj");
    }

    willCreateObject->init();
    mGameObject.push_back(willCreateObject);
    return;
}
