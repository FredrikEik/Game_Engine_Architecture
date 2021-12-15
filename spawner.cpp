#include "spawner.h"
#include "cube.h"
#include "gameobject.h"
#include "mainwindow.h"
#include <ctime>

/**
 * @brief Spawner::Spawner
 * @param ObjFactory
 * @param TheMainWindow
 */
Spawner::Spawner(ObjectFactory* ObjFactory, MainWindow* TheMainWindow)
{
    ObjSpawnFactory = ObjFactory;
    mMainWindow = TheMainWindow;
}

/**
 * @brief Spawner::spawnRow
 * @param rowLength
 */
void Spawner::spawnRow(int rowLength)
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

    spawnHindrances(rowLength);
}

/**
 * @brief Spawner::spawnHindrances
 * @param hindranceLength
 */
void Spawner::spawnHindrances(int hindranceLength)
{
    for(int i = 15; i < hindranceLength; i+=rand()%15+10 /*i+=5*/)
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
                mMainWindow->addObjectToWorldList("Hindrance");
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
                mMainWindow->addObjectToWorldList("Hindrance");
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
                mMainWindow->addObjectToWorldList("Hindrance");
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
                mMainWindow->addObjectToWorldList("Hindrance");
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
                mMainWindow->addObjectToWorldList("Hindrance");
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
                mMainWindow->addObjectToWorldList("Hindrance");
                break;
            }
            SpawnedHindrance->mTexture = 5;
            SpawnedHindrance->mName = "hindrance";
            //qDebug() << "Current i: " << i << "  Hindrance type: " << HindranceType;
        }
    }
}

/**
 * @brief Spawner::updateHindrances
 */
void Spawner::updateHindrances()
{

    int i = rand()%15+10;
    HindranceType = rand()%6+1;
    float biggestBack = 0;

    for (int i = 0; i < spawnedHindrances.size(); i++)
    {
        if(spawnedHindrances[i]->TransformComp->mMatrix.getPosition().z < biggestBack)
            biggestBack = spawnedHindrances[i]->TransformComp->mMatrix.getPosition().z;
    }

    if(SpawnedHindrance)
    {
        gsl::Vector3D temp = SpawnedHindrance->TransformComp->mMatrix.getPosition();
        temp.x = rand()%5+1;
        SpawnedHindrance->TransformComp->mTrueScaleMatrix.setPosition(temp.x, temp.y, biggestBack - i);
        SpawnedHindrance->getTransformComp()->Scal.setX(rand()%5+1);
        SpawnedHindrance->getTransformComp()->Scal.setY(1.f);
        SpawnedHindrance->getTransformComp()->Scal.setZ(1.f);
        SpawnedHindrance->getTransformComp()->mMatrix = SpawnedHindrance->getTransformComp()->mTrueScaleMatrix;
        SpawnedHindrance->getTransformComp()->mMatrix.scale(SpawnedHindrance->getTransformComp()->Scal.getX(),
                                                         SpawnedHindrance->getTransformComp()->Scal.getY(),
                                                         SpawnedHindrance->getTransformComp()->Scal.getZ());
    }
}

/**
 * @brief Spawner::update
 * @param z
 */
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

    for (int i = 0; i < spawnedHindrances.size(); i++)
    {
        if (spawnedHindrances[i]->TransformComp->mMatrix.getPosition().z - 5 > z)
        {
            SpawnedHindrance = spawnedHindrances[i];
            updateHindrances();
        }
    }
}

/**
 * @brief Spawner::resetSpawner
 */
void Spawner::resetSpawner()
{
    for (int i = 0; i < spawnedObjects.size(); i += 3)
    {
        gsl::Vector3D temp;
        temp = spawnedObjects[i]->TransformComp->mMatrix.getPosition();
        spawnedObjects[i]->TransformComp->mMatrix.setPosition(temp.x, temp.y, -(i / 3 * 5));
        spawnedObjects[i]->TransformComp->mTrueScaleMatrix.setPosition(temp.x, temp.y, -(i / 3 * 5));
        temp = spawnedObjects[i + 1]->TransformComp->mMatrix.getPosition();
        spawnedObjects[i + 1]->TransformComp->mMatrix.setPosition(temp.x, temp.y, -(i / 3 * 5));
        spawnedObjects[i + 1]->TransformComp->mTrueScaleMatrix.setPosition(temp.x, temp.y, -(i / 3 * 5));
        temp = spawnedObjects[i + 2]->TransformComp->mMatrix.getPosition();
        spawnedObjects[i + 2]->TransformComp->mMatrix.setPosition(temp.x, temp.y, -(i / 3 * 5));
        spawnedObjects[i + 2]->TransformComp->mTrueScaleMatrix.setPosition(temp.x, temp.y, -(i / 3 * 5));
    }

    for(int i = 0; i < spawnedHindrances.size(); i++)
    {
        int zOffset = rand()%15+10;
        gsl::Vector3D temp = spawnedHindrances[i]->TransformComp->mMatrix.getPosition();
        temp.x = rand()%5+1;
        if(i == 0)
        {
            spawnedHindrances[i]->TransformComp->mTrueScaleMatrix.setPosition(temp.x, temp.y, -zOffset);
        }
        else
        {
            spawnedHindrances[i]->TransformComp->mTrueScaleMatrix.setPosition(temp.x, temp.y,
                                                                          spawnedHindrances[i - 1]->getTransformComp()->mMatrix.getPosition().z - zOffset);
        }
        spawnedHindrances[i]->getTransformComp()->Scal.setX(rand()%5+1);
        spawnedHindrances[i]->getTransformComp()->Scal.setY(1.f);
        spawnedHindrances[i]->getTransformComp()->Scal.setZ(1.f);
        spawnedHindrances[i]->getTransformComp()->mMatrix = spawnedHindrances[i]->getTransformComp()->mTrueScaleMatrix;
        spawnedHindrances[i]->getTransformComp()->mMatrix.scale(spawnedHindrances[i]->getTransformComp()->Scal.getX(),
                                                         spawnedHindrances[i]->getTransformComp()->Scal.getY(),
                                                         spawnedHindrances[i]->getTransformComp()->Scal.getZ());
    }
}
