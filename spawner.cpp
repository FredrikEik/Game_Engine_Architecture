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
    if(SpawnedObject)
        SpawnedObject->getTransformComp()->mMatrix.translateX(0.0f);
    //ObjSpawnFactory->createObject("Cube");
    //SpawnedObject->getTransformComp()->mMatrix.translateX(1.0f);
    //ObjSpawnFactory->createObject("Goat");
    //SpawnedObject->getTransformComp()->mMatrix.translateX(2.0f);

}
