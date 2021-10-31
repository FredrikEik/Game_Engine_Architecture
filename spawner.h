#ifndef SPAWNER_H
#define SPAWNER_H

#include "iostream"
#include "gameobject.h"
#include "objectfactory.h"
#include "renderwindow.h"

class Spawner
{
public:
    Spawner(ObjectFactory* ObjFactory);

    void SpawnRow();

private:

    ObjectFactory* ObjSpawnFactory;
    GameObject* SpawnedObject;
};

#endif // SPAWNER_H
