#include "spawner.h"
#include "cube.h"
#include "gameobject.h"

Spawner::Spawner(ObjectFactory* ObjFactory)
{
    ObjSpawnFactory = ObjFactory;
}

void Spawner::SpawnRow()
{
    ObjSpawnFactory->createObject("Cube");
    SpawnedObject = ObjSpawnFactory->mGameObject.back();
    if(SpawnedObject)
        SpawnedObject->getTransformComp()->mMatrix.translateX(0.0f);
    ObjSpawnFactory->createObject("Cube");
    SpawnedObject = ObjSpawnFactory->mGameObject.back();
    if(SpawnedObject)
        SpawnedObject->getTransformComp()->mMatrix.translateX(1.0f);
    ObjSpawnFactory->createObject("Cube");
    SpawnedObject = ObjSpawnFactory->mGameObject.back();
    if(SpawnedObject)
        SpawnedObject->getTransformComp()->mMatrix.translateX(2.0f);

}
