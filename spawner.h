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

    void spawnRow(int rowLength);
    void spawnHindrances(int hindranceLength);
    void updateHindrances();
    //void addObjectToEditor(std::string object);

    void update(float z);
    int HindranceType = 0;
    void resetSpawner();

private:
    MainWindow *mMainWindow {nullptr};
    ObjectFactory* ObjSpawnFactory;
    GameObject* SpawnedObject {nullptr};
    std::vector<GameObject*> spawnedObjects;
    GameObject* SpawnedHindrance {nullptr};
    std::vector<GameObject*> spawnedHindrances;


};

#endif // SPAWNER_H
