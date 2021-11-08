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
    Spawner(ObjectFactory* ObjFactory, MainWindow* TheMainWindow);

    void SpawnRow(int rowLength);
    //void addObjectToEditor(std::string object);

    void update(float z);
    void resetSpawner();

private:
    MainWindow *mMainWindow {nullptr};
    ObjectFactory* ObjSpawnFactory;
    GameObject* SpawnedObject {nullptr};
    std::vector<GameObject*> spawnedObjects;

};

#endif // SPAWNER_H
