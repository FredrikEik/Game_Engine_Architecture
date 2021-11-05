#include "spawner.h"
#include "cube.h"
#include "gameobject.h"
#include "mainwindow.h"

Spawner::Spawner(ObjectFactory* ObjFactory, MainWindow* TheMainWindow)
{
    ObjSpawnFactory = ObjFactory;
    mMainWindow = TheMainWindow;
}

void Spawner::SpawnRow(int rowLength)
{
    for(int i = 0; i < rowLength; i+=5)
    {
        //FLOOR OF MAP
        ObjSpawnFactory->createObject("Cube");
        SpawnedObject = ObjSpawnFactory->mGameObject.back();
        if(SpawnedObject)
        {
            SpawnedObject->getTransformComp()->mTrueScaleMatrix.translateX(3.0f);
            SpawnedObject->getTransformComp()->mTrueScaleMatrix.translateZ(-i);
            SpawnedObject->getTransformComp()->Scal.setX(7.f);
            SpawnedObject->getTransformComp()->Scal.setY(1.f);
            SpawnedObject->getTransformComp()->Scal.setZ(5.f);
            SpawnedObject->getTransformComp()->mMatrix = SpawnedObject->getTransformComp()->mTrueScaleMatrix;
            SpawnedObject->getTransformComp()->mMatrix.scale(SpawnedObject->getTransformComp()->Scal.getX(),
                                                             SpawnedObject->getTransformComp()->Scal.getY(),
                                                             SpawnedObject->getTransformComp()->Scal.getZ());
            mMainWindow->addObjectToWorldList("Floor Cube");
        }

        //LEFT ROW OF MAP
        ObjSpawnFactory->createObject("Cube");
        SpawnedObject = ObjSpawnFactory->mGameObject.back();
        if(SpawnedObject)
        {
            SpawnedObject->getTransformComp()->mTrueScaleMatrix.translateY(3.0f);
            SpawnedObject->getTransformComp()->mTrueScaleMatrix.translateZ(-i);
            SpawnedObject->getTransformComp()->Scal.setX(1.f);
            SpawnedObject->getTransformComp()->Scal.setY(5.f);
            SpawnedObject->getTransformComp()->Scal.setZ(5.f);
            SpawnedObject->getTransformComp()->mMatrix = SpawnedObject->getTransformComp()->mTrueScaleMatrix;
            SpawnedObject->getTransformComp()->mMatrix.scale(SpawnedObject->getTransformComp()->Scal.getX(),
                                                             SpawnedObject->getTransformComp()->Scal.getY(),
                                                             SpawnedObject->getTransformComp()->Scal.getZ());
            mMainWindow->addObjectToWorldList("Left Row Cube");
        }

        //RIGHT ROW OF MAP
        ObjSpawnFactory->createObject("Cube");
        SpawnedObject = ObjSpawnFactory->mGameObject.back();
        if(SpawnedObject)
        {
            SpawnedObject->getTransformComp()->mTrueScaleMatrix.translateX(6.0f);
            SpawnedObject->getTransformComp()->mTrueScaleMatrix.translateY(3.0f);
            SpawnedObject->getTransformComp()->mTrueScaleMatrix.translateZ(-i);
            SpawnedObject->getTransformComp()->Scal.setX(1.f);
            SpawnedObject->getTransformComp()->Scal.setY(5.f);
            SpawnedObject->getTransformComp()->Scal.setZ(5.f);
            SpawnedObject->getTransformComp()->mMatrix = SpawnedObject->getTransformComp()->mTrueScaleMatrix;
            SpawnedObject->getTransformComp()->mMatrix.scale(SpawnedObject->getTransformComp()->Scal.getX(),
                                                             SpawnedObject->getTransformComp()->Scal.getY(),
                                                             SpawnedObject->getTransformComp()->Scal.getZ());
            mMainWindow->addObjectToWorldList("Right Row Cube");
        }
    }

    //FLOOR OF MAP
    /*ObjSpawnFactory->createObject("Cube");
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
        SpawnedObject->getTransformComp()->mTrueScaleMatrix.translateX(3.0f);
        //SpawnedObject->getTransformComp()->mMatrix.translateX(3.0f);
        //SpawnedObject->getTransformComp()->mTrueScaleMatrix.scale(7.f, 1.f, 1.f);
        SpawnedObject->getTransformComp()->Scal.setX(7.f);
        SpawnedObject->getTransformComp()->Scal.setY(1.f);
        SpawnedObject->getTransformComp()->Scal.setZ(1.f);
        SpawnedObject->getTransformComp()->mMatrix = SpawnedObject->getTransformComp()->mTrueScaleMatrix;
        SpawnedObject->getTransformComp()->mMatrix.scale(SpawnedObject->getTransformComp()->Scal.getX(),
                                                         SpawnedObject->getTransformComp()->Scal.getY(),
                                                         SpawnedObject->getTransformComp()->Scal.getZ());
        mMainWindow->addObjectToWorldList("Floor Cube");
    }

    //LEFT ROW OF MAP
    ObjSpawnFactory->createObject("Cube");
    SpawnedObject = ObjSpawnFactory->mGameObject.back();
    if(SpawnedObject)
    {
        SpawnedObject->getTransformComp()->mTrueScaleMatrix.translateY(3.0f);
        SpawnedObject->getTransformComp()->Scal.setX(1.f);
        SpawnedObject->getTransformComp()->Scal.setY(5.f);
        SpawnedObject->getTransformComp()->Scal.setZ(1.f);
        SpawnedObject->getTransformComp()->mMatrix = SpawnedObject->getTransformComp()->mTrueScaleMatrix;
        SpawnedObject->getTransformComp()->mMatrix.scale(SpawnedObject->getTransformComp()->Scal.getX(),
                                                         SpawnedObject->getTransformComp()->Scal.getY(),
                                                         SpawnedObject->getTransformComp()->Scal.getZ());
        mMainWindow->addObjectToWorldList("Left Row Cube");
    }

    //RIGHT ROW OF MAP
    ObjSpawnFactory->createObject("Cube");
    SpawnedObject = ObjSpawnFactory->mGameObject.back();
    if(SpawnedObject)
    {
        SpawnedObject->getTransformComp()->mTrueScaleMatrix.translateX(6.0f);
        SpawnedObject->getTransformComp()->mTrueScaleMatrix.translateY(3.0f);
        SpawnedObject->getTransformComp()->Scal.setX(1.f);
        SpawnedObject->getTransformComp()->Scal.setY(5.f);
        SpawnedObject->getTransformComp()->Scal.setZ(1.f);
        SpawnedObject->getTransformComp()->mMatrix = SpawnedObject->getTransformComp()->mTrueScaleMatrix;
        SpawnedObject->getTransformComp()->mMatrix.scale(SpawnedObject->getTransformComp()->Scal.getX(),
                                                         SpawnedObject->getTransformComp()->Scal.getY(),
                                                         SpawnedObject->getTransformComp()->Scal.getZ());
        mMainWindow->addObjectToWorldList("Right Row Cube");
    }*/
}

/*void Spawner::addObjectToEditor(std::string object)
{
    if(SpawnedObject)
    {
        mMainWindow->addObjectToWorldList(object);
    }
}*/
