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
    //FLOOR OF MAP
    ObjSpawnFactory->createObject("Cube");
    SpawnedObject = ObjSpawnFactory->mGameObject.back();
    if(SpawnedObject)
    {
        SpawnedObject->getTransformComp()->mTrueScaleMatrix.translateX(0.0f);
        SpawnedObject->getTransformComp()->mMatrix.translateX(0.0f);
        mMainWindow->addObjectToWorldList("Floor Cube");
    }
    ObjSpawnFactory->createObject("Cube");
    SpawnedObject = ObjSpawnFactory->mGameObject.back();
    if(SpawnedObject)
    {
        SpawnedObject->getTransformComp()->mTrueScaleMatrix.translateX(1.0f);
        SpawnedObject->getTransformComp()->mMatrix.translateX(1.0f);
        mMainWindow->addObjectToWorldList("Floor Cube");
    }
    ObjSpawnFactory->createObject("Cube");
    SpawnedObject = ObjSpawnFactory->mGameObject.back();
    if(SpawnedObject)
    {
        SpawnedObject->getTransformComp()->mTrueScaleMatrix.translateX(2.0f);
        SpawnedObject->getTransformComp()->mMatrix.translateX(2.0f);
        mMainWindow->addObjectToWorldList("Floor Cube");
    }
    ObjSpawnFactory->createObject("Cube");
    SpawnedObject = ObjSpawnFactory->mGameObject.back();
    if(SpawnedObject)
    {
        SpawnedObject->getTransformComp()->mTrueScaleMatrix.translateX(3.0f);
        SpawnedObject->getTransformComp()->mMatrix.translateX(3.0f);
        mMainWindow->addObjectToWorldList("Floor Cube");
    }
    ObjSpawnFactory->createObject("Cube");
    SpawnedObject = ObjSpawnFactory->mGameObject.back();
    if(SpawnedObject)
    {
        SpawnedObject->getTransformComp()->mTrueScaleMatrix.translateX(4.0f);
        SpawnedObject->getTransformComp()->mMatrix.translateX(4.0f);
        mMainWindow->addObjectToWorldList("Floor Cube");
    }
    ObjSpawnFactory->createObject("Cube");
    SpawnedObject = ObjSpawnFactory->mGameObject.back();
    if(SpawnedObject)
    {
        SpawnedObject->getTransformComp()->mTrueScaleMatrix.translateX(5.0f);
        SpawnedObject->getTransformComp()->mMatrix.translateX(5.0f);
        mMainWindow->addObjectToWorldList("Floor Cube");
    }
    ObjSpawnFactory->createObject("Cube");
    SpawnedObject = ObjSpawnFactory->mGameObject.back();
    if(SpawnedObject)
    {
        SpawnedObject->getTransformComp()->mTrueScaleMatrix.translateX(6.0f);
        SpawnedObject->getTransformComp()->mMatrix.translateX(6.0f);
        mMainWindow->addObjectToWorldList("Floor Cube");
    }

    //LEFT ROW OF MAP
    ObjSpawnFactory->createObject("Cube");
    SpawnedObject = ObjSpawnFactory->mGameObject.back();
    if(SpawnedObject)
    {
        SpawnedObject->getTransformComp()->mTrueScaleMatrix.translateY(1.0f);
        SpawnedObject->getTransformComp()->mMatrix.translateY(1.0f);
        mMainWindow->addObjectToWorldList("Left Row Cube");
    }
    ObjSpawnFactory->createObject("Cube");
    SpawnedObject = ObjSpawnFactory->mGameObject.back();
    if(SpawnedObject)
    {
        SpawnedObject->getTransformComp()->mTrueScaleMatrix.translateY(2.0f);
        SpawnedObject->getTransformComp()->mMatrix.translateY(2.0f);
        mMainWindow->addObjectToWorldList("Left Row Cube");
    }
    ObjSpawnFactory->createObject("Cube");
    SpawnedObject = ObjSpawnFactory->mGameObject.back();
    if(SpawnedObject)
    {
        SpawnedObject->getTransformComp()->mTrueScaleMatrix.translateY(3.0f);
        SpawnedObject->getTransformComp()->mMatrix.translateY(3.0f);
        mMainWindow->addObjectToWorldList("Left Row Cube");
    }
    ObjSpawnFactory->createObject("Cube");
    SpawnedObject = ObjSpawnFactory->mGameObject.back();
    if(SpawnedObject)
    {
        SpawnedObject->getTransformComp()->mTrueScaleMatrix.translateY(4.0f);
        SpawnedObject->getTransformComp()->mMatrix.translateY(4.0f);
        mMainWindow->addObjectToWorldList("Left Row Cube");
    }

    //RIGHT ROW OF MAP
    ObjSpawnFactory->createObject("Cube");
    SpawnedObject = ObjSpawnFactory->mGameObject.back();
    if(SpawnedObject)
    {
        SpawnedObject->getTransformComp()->mTrueScaleMatrix.translateX(6.0f);
        SpawnedObject->getTransformComp()->mMatrix.translateX(6.0f);
        SpawnedObject->getTransformComp()->mTrueScaleMatrix.translateY(1.0f);
        SpawnedObject->getTransformComp()->mMatrix.translateY(1.0f);
        mMainWindow->addObjectToWorldList("Right Row Cube");
    }
    ObjSpawnFactory->createObject("Cube");
    SpawnedObject = ObjSpawnFactory->mGameObject.back();
    if(SpawnedObject)
    {
        SpawnedObject->getTransformComp()->mTrueScaleMatrix.translateX(6.0f);
        SpawnedObject->getTransformComp()->mMatrix.translateX(6.0f);
        SpawnedObject->getTransformComp()->mTrueScaleMatrix.translateY(2.0f);
        SpawnedObject->getTransformComp()->mMatrix.translateY(2.0f);
        mMainWindow->addObjectToWorldList("Right Row Cube");
    }
    ObjSpawnFactory->createObject("Cube");
    SpawnedObject = ObjSpawnFactory->mGameObject.back();
    if(SpawnedObject)
    {
        SpawnedObject->getTransformComp()->mTrueScaleMatrix.translateX(6.0f);
        SpawnedObject->getTransformComp()->mMatrix.translateX(6.0f);
        SpawnedObject->getTransformComp()->mTrueScaleMatrix.translateY(3.0f);
        SpawnedObject->getTransformComp()->mMatrix.translateY(3.0f);
        mMainWindow->addObjectToWorldList("Right Row Cube");
    }
    ObjSpawnFactory->createObject("Cube");
    SpawnedObject = ObjSpawnFactory->mGameObject.back();
    if(SpawnedObject)
    {
        SpawnedObject->getTransformComp()->mTrueScaleMatrix.translateX(6.0f);
        SpawnedObject->getTransformComp()->mMatrix.translateX(6.0f);
        SpawnedObject->getTransformComp()->mTrueScaleMatrix.translateY(4.0f);
        SpawnedObject->getTransformComp()->mMatrix.translateY(4.0f);
        mMainWindow->addObjectToWorldList("Right Row Cube");
    }
}

/*void Spawner::addObjectToEditor(std::string object)
{
    if(SpawnedObject)
    {
        mMainWindow->addObjectToWorldList(object);
    }
}*/
