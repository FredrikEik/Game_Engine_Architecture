#ifndef OBJECTFACTORY_H
#define OBJECTFACTORY_H

#include "iostream"
#include "gameobject.h"

class ObjectFactory
{
public:
    ObjectFactory();
    std::vector<GameObject*> mGameObject;
    void createObject(std::string objectName);
};

#endif // OBJECTFACTORY_H
