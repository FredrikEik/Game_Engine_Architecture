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
        objectToCreate->getMaterialComponent()->mShaderProgram = 1;
        objectToCreate->getMaterialComponent()->mTextureUnit = 1;
    }
    else if(objectName == "Plane")
    {
        objectToCreate = new Plane;
        objectToCreate->getMaterialComponent()->mShaderProgram = 1;
        objectToCreate->getMaterialComponent()->mTextureUnit = 1;
    }
    else if(objectName == "Triangle")
    {
        objectToCreate = new Triangle;
        objectToCreate->getMaterialComponent()->mShaderProgram = 1;
        objectToCreate->getMaterialComponent()->mTextureUnit = 1;
    }
    else if(objectName == "MarioCube")
    {
        objectToCreate = new MarioCube;
        objectToCreate->getMaterialComponent()->mShaderProgram = 1;
        objectToCreate->getMaterialComponent()->mTextureUnit = 1;
    }
    else{return;}

    objectToCreate->init();
    mGameObjects.push_back(objectToCreate);
    return;
}
