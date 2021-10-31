#include "spawner.h"
#include "cube.h"
#include "gameobject.h"
#include "mainwindow.h"

Spawner::Spawner(ObjectFactory* ObjFactory, MainWindow* TheMainWindow)
{
    ObjSpawnFactory = ObjFactory;
    mMainWindow = TheMainWindow;
}

void Spawner::SpawnRow()
{
    ObjSpawnFactory->createObject("Cube");
    SpawnedObject = ObjSpawnFactory->mGameObject.back();
    if(SpawnedObject)
    {
        SpawnedObject->getTransformComp()->mTrueScaleMatrix.translateX(0.0f);
        SpawnedObject->getTransformComp()->mMatrix.translateX(0.0f);
        mMainWindow->addObjectToWorldList("Cube");
    }
    ObjSpawnFactory->createObject("Cube");
    SpawnedObject = ObjSpawnFactory->mGameObject.back();
    if(SpawnedObject)
    {
        SpawnedObject->getTransformComp()->mTrueScaleMatrix.translateX(1.0f);
        SpawnedObject->getTransformComp()->mMatrix.translateX(1.0f);
        mMainWindow->addObjectToWorldList("Cube");
    }
    ObjSpawnFactory->createObject("Cube");
    SpawnedObject = ObjSpawnFactory->mGameObject.back();
    if(SpawnedObject)
    {
        SpawnedObject->getTransformComp()->mTrueScaleMatrix.translateX(2.0f);
        SpawnedObject->getTransformComp()->mMatrix.translateX(2.0f);
        mMainWindow->addObjectToWorldList("Cube");
    }
}

/*void Spawner::addObjectToEditor(std::string object)
{
    if(SpawnedObject)
    {
        mMainWindow->addObjectToWorldList(object);
    }
}*/
