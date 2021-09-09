#include "objectfactory.h"
#include "cube.h"

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

    willCreateObject->init();
    mGameObject.push_back(willCreateObject);
    return;
}
