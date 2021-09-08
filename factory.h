#ifndef FACTORY_H
#define FACTORY_H

#include "iostream"
#include "gameobject.h"

class Factory
{
public:
    Factory();
    std::vector<GameObject*> mGameObjects;
    void createObject(std::string objectName);
};

#endif // FACTORY_H
