#ifndef SPAWNER_H
#define SPAWNER_H

#include "iostream"
#include "gameobject.h"
#include "objectfactory.h"
#include "renderwindow.h"
#include "mainwindow.h"

class Spawner
{
public:
    Spawner(ObjectFactory* ObjFactory);

    void SpawnRow();
    //void addObjectToEditor(std::string object);

private:

    MainWindow *mMainWindow{nullptr};
    ObjectFactory* ObjSpawnFactory;
    GameObject* SpawnedObject {nullptr};
};

#endif // SPAWNER_H
