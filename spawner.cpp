#include "spawner.h"
#include "cube.h"
#include "gameobject.h"
#include "mainwindow.h"
#include <ctime>

Spawner::Spawner(ObjectFactory* ObjFactory, MainWindow* TheMainWindow)
{
    ObjSpawnFactory = ObjFactory;
    mMainWindow = TheMainWindow;
}

void Spawner::SpawnRow(int rowLength)
{
    std::srand(time(0));
    for(int i = 0; i < rowLength; i+=5)
    {
        //FLOOR OF MAP
        ObjSpawnFactory->createObject("Cube");
        spawnedObjects.push_back(ObjSpawnFactory->mGameObject.back());
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
        spawnedObjects.push_back(ObjSpawnFactory->mGameObject.back());
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
        spawnedObjects.push_back(ObjSpawnFactory->mGameObject.back());
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

    for(int i = 10; i < rowLength; i+=rand()%15+10 /*i+=5*/)
    {
        HindranceType = rand()%6+1;
        //int x = 1 + std::rand()/((RAND_MAX + 1u)/2);
        ObjSpawnFactory->createObject("Cube");
        spawnedHindrances.push_back(ObjSpawnFactory->mGameObject.back());
        SpawnedHindrance = ObjSpawnFactory->mGameObject.back();
        if(SpawnedHindrance)
        {
            switch(HindranceType)
            {
            case 1:
                SpawnedHindrance->getTransformComp()->mTrueScaleMatrix.translateX(3.f);
                SpawnedHindrance->getTransformComp()->mTrueScaleMatrix.translateY(1.0f);
                SpawnedHindrance->getTransformComp()->mTrueScaleMatrix.translateZ(-i);
                SpawnedHindrance->getTransformComp()->Scal.setX(5.f);
                SpawnedHindrance->getTransformComp()->Scal.setY(1.f);
                SpawnedHindrance->getTransformComp()->Scal.setZ(1.f);
                SpawnedHindrance->getTransformComp()->mMatrix = SpawnedHindrance->getTransformComp()->mTrueScaleMatrix;
                SpawnedHindrance->getTransformComp()->mMatrix.scale(SpawnedHindrance->getTransformComp()->Scal.getX(),
                                                                 SpawnedHindrance->getTransformComp()->Scal.getY(),
                                                                 SpawnedHindrance->getTransformComp()->Scal.getZ());
                mMainWindow->addObjectToWorldList("Large Hindrance Cube");
                break;
            case 2:
                SpawnedHindrance->getTransformComp()->mTrueScaleMatrix.translateX(2.f);
                SpawnedHindrance->getTransformComp()->mTrueScaleMatrix.translateY(1.0f);
                SpawnedHindrance->getTransformComp()->mTrueScaleMatrix.translateZ(-i);
                SpawnedHindrance->getTransformComp()->Scal.setX(1.f);
                SpawnedHindrance->getTransformComp()->Scal.setY(1.f);
                SpawnedHindrance->getTransformComp()->Scal.setZ(1.f);
                SpawnedHindrance->getTransformComp()->mMatrix = SpawnedHindrance->getTransformComp()->mTrueScaleMatrix;
                SpawnedHindrance->getTransformComp()->mMatrix.scale(SpawnedHindrance->getTransformComp()->Scal.getX(),
                                                                 SpawnedHindrance->getTransformComp()->Scal.getY(),
                                                                 SpawnedHindrance->getTransformComp()->Scal.getZ());
                mMainWindow->addObjectToWorldList("Small Hindrance Cube Left");
                break;
            case 3:
                SpawnedHindrance->getTransformComp()->mTrueScaleMatrix.translateX(3.f);
                SpawnedHindrance->getTransformComp()->mTrueScaleMatrix.translateY(1.0f);
                SpawnedHindrance->getTransformComp()->mTrueScaleMatrix.translateZ(-i);
                SpawnedHindrance->getTransformComp()->Scal.setX(1.f);
                SpawnedHindrance->getTransformComp()->Scal.setY(1.f);
                SpawnedHindrance->getTransformComp()->Scal.setZ(1.f);
                SpawnedHindrance->getTransformComp()->mMatrix = SpawnedHindrance->getTransformComp()->mTrueScaleMatrix;
                SpawnedHindrance->getTransformComp()->mMatrix.scale(SpawnedHindrance->getTransformComp()->Scal.getX(),
                                                                 SpawnedHindrance->getTransformComp()->Scal.getY(),
                                                                 SpawnedHindrance->getTransformComp()->Scal.getZ());
                mMainWindow->addObjectToWorldList("Small Hindrance Cube Middle");
                break;
            case 4:
                SpawnedHindrance->getTransformComp()->mTrueScaleMatrix.translateX(4.f);
                SpawnedHindrance->getTransformComp()->mTrueScaleMatrix.translateY(1.0f);
                SpawnedHindrance->getTransformComp()->mTrueScaleMatrix.translateZ(-i);
                SpawnedHindrance->getTransformComp()->Scal.setX(1.f);
                SpawnedHindrance->getTransformComp()->Scal.setY(1.f);
                SpawnedHindrance->getTransformComp()->Scal.setZ(1.f);
                SpawnedHindrance->getTransformComp()->mMatrix = SpawnedHindrance->getTransformComp()->mTrueScaleMatrix;
                SpawnedHindrance->getTransformComp()->mMatrix.scale(SpawnedHindrance->getTransformComp()->Scal.getX(),
                                                                 SpawnedHindrance->getTransformComp()->Scal.getY(),
                                                                 SpawnedHindrance->getTransformComp()->Scal.getZ());
                mMainWindow->addObjectToWorldList("Small Hindrance Cube Right");
                break;
            case 5:
                SpawnedHindrance->getTransformComp()->mTrueScaleMatrix.translateX(1.5f);
                SpawnedHindrance->getTransformComp()->mTrueScaleMatrix.translateY(1.0f);
                SpawnedHindrance->getTransformComp()->mTrueScaleMatrix.translateZ(-i);
                SpawnedHindrance->getTransformComp()->Scal.setX(3.f);
                SpawnedHindrance->getTransformComp()->Scal.setY(1.f);
                SpawnedHindrance->getTransformComp()->Scal.setZ(1.f);
                SpawnedHindrance->getTransformComp()->mMatrix = SpawnedHindrance->getTransformComp()->mTrueScaleMatrix;
                SpawnedHindrance->getTransformComp()->mMatrix.scale(SpawnedHindrance->getTransformComp()->Scal.getX(),
                                                                 SpawnedHindrance->getTransformComp()->Scal.getY(),
                                                                 SpawnedHindrance->getTransformComp()->Scal.getZ());
                mMainWindow->addObjectToWorldList("Middle Hindrance Cube Left");
                break;
            case 6:
                SpawnedHindrance->getTransformComp()->mTrueScaleMatrix.translateX(4.f);
                SpawnedHindrance->getTransformComp()->mTrueScaleMatrix.translateY(1.0f);
                SpawnedHindrance->getTransformComp()->mTrueScaleMatrix.translateZ(-i);
                SpawnedHindrance->getTransformComp()->Scal.setX(3.f);
                SpawnedHindrance->getTransformComp()->Scal.setY(1.f);
                SpawnedHindrance->getTransformComp()->Scal.setZ(1.f);
                SpawnedHindrance->getTransformComp()->mMatrix = SpawnedHindrance->getTransformComp()->mTrueScaleMatrix;
                SpawnedHindrance->getTransformComp()->mMatrix.scale(SpawnedHindrance->getTransformComp()->Scal.getX(),
                                                                 SpawnedHindrance->getTransformComp()->Scal.getY(),
                                                                 SpawnedHindrance->getTransformComp()->Scal.getZ());
                mMainWindow->addObjectToWorldList("Middle Hindrance Cube Right");
                break;
            }
            //qDebug() << "Current i: " << i << "  Hindrance type: " << HindranceType;
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

void Spawner::SpawnHindrances(int hindranceLength)
{
    /*for(int i = 20; i < hindranceLength; i+=rand()%15+10)
    {
        HindranceType = rand()%2+1;
        //int x = 1 + std::rand()/((RAND_MAX + 1u)/2);
        ObjSpawnFactory->createObject("Cube");
        spawnedHindrances.push_back(ObjSpawnFactory->mGameObject.back());
        SpawnedHindrance = ObjSpawnFactory->mGameObject.back();
        if(SpawnedHindrance)
        {
            switch(HindranceType)
            {
            case 1:
                SpawnedHindrance->getTransformComp()->mTrueScaleMatrix.translateX(3.f);
                SpawnedHindrance->getTransformComp()->mTrueScaleMatrix.translateY(1.0f);
                SpawnedHindrance->getTransformComp()->mTrueScaleMatrix.translateZ(-i);
                SpawnedHindrance->getTransformComp()->Scal.setX(5.f);
                SpawnedHindrance->getTransformComp()->Scal.setY(1.f);
                SpawnedHindrance->getTransformComp()->Scal.setZ(1.f);
                SpawnedHindrance->getTransformComp()->mMatrix = SpawnedHindrance->getTransformComp()->mTrueScaleMatrix;
                SpawnedHindrance->getTransformComp()->mMatrix.scale(SpawnedHindrance->getTransformComp()->Scal.getX(),
                                                                 SpawnedHindrance->getTransformComp()->Scal.getY(),
                                                                 SpawnedHindrance->getTransformComp()->Scal.getZ());
                mMainWindow->addObjectToWorldList("Large Hindrance Cube");
            case 2:
                SpawnedHindrance->getTransformComp()->mTrueScaleMatrix.translateX(3.f);
                SpawnedHindrance->getTransformComp()->mTrueScaleMatrix.translateY(1.0f);
                SpawnedHindrance->getTransformComp()->mTrueScaleMatrix.translateZ(-i);
                SpawnedHindrance->getTransformComp()->Scal.setX(1.f);
                SpawnedHindrance->getTransformComp()->Scal.setY(1.f);
                SpawnedHindrance->getTransformComp()->Scal.setZ(1.f);
                SpawnedHindrance->getTransformComp()->mMatrix = SpawnedHindrance->getTransformComp()->mTrueScaleMatrix;
                SpawnedHindrance->getTransformComp()->mMatrix.scale(SpawnedHindrance->getTransformComp()->Scal.getX(),
                                                                 SpawnedHindrance->getTransformComp()->Scal.getY(),
                                                                 SpawnedHindrance->getTransformComp()->Scal.getZ());
                mMainWindow->addObjectToWorldList("Small Hindrance Cube");
            }

        }

    }*/
}

void Spawner::update(float z)
{
    for (int i = 0; i < spawnedObjects.size(); i++)
    {
        if (spawnedObjects[i]->TransformComp->mMatrix.getPosition().z - 5 > z)
        {
            gsl::Vector3D temp = spawnedObjects[i]->TransformComp->mMatrix.getPosition();
            spawnedObjects[i]->TransformComp->mMatrix.setPosition(temp.x, temp.y, temp.z - ((spawnedObjects.size()/3) * 5));
            spawnedObjects[i]->TransformComp->mTrueScaleMatrix.setPosition(temp.x, temp.y, temp.z - ((spawnedObjects.size()/3) * 5));
        }
    }

    for(int i = 0; i < spawnedHindrances.size(); i++)
    {
        if(spawnedHindrances[i]->TransformComp->mMatrix.getPosition().z - 5 > z)
        {
            ObjSpawnFactory->mGameObject.erase(ObjSpawnFactory->mGameObject.begin());
            mMainWindow->removeObjectFromWorldList();
        }
    }
}
