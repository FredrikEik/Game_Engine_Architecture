#include "level.h"

Level::Level(QObject *parent) : QObject(parent)
{

}

Level::Level(Factory& factory)
{
    mFactory = &factory;
}

void Level::spawnObjectAtLocation(std::string objectName, float x, float y, float z)
{
    GameObject *temp = mFactory->createObject(objectName);
    gsl::Vector3D location{x,y,z};
    temp->getTransformComponent()->mMatrix.translate(location);
}
