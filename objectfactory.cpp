#include "objectfactory.h"
#include "cube.h"
#include "triangle.h"

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
    /**if(objectName == "Triangle")
    {
        willCreateObject = new Triangle;
    }*/

    if(objectName != "")
    {
        willCreateObject->init();
        mGameObject.push_back(willCreateObject);
    }

    return;
}
