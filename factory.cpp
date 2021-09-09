#include "plane.h"
#include "mariocube.h"
#include "triangle.h"
#include "cube.h"
#include "factory.h"


Factory::Factory()
{

}

void Factory::createObject(std::string objectName)
{
    GameObject* objectToCreate;

    if(objectName == "Cube")
    {
        objectToCreate = new Cube;
    }
    else if(objectName == "Plane")
    {
        objectToCreate = new Plane;
    }
    else if(objectName == "Triangle")
    {
        objectToCreate = new Triangle;
    }
    else if(objectName == "MarioCube")
    {
        objectToCreate = new MarioCube;
    }
    else{return;}

    objectToCreate->init();
    mGameObjects.push_back(objectToCreate);
    return;


}
