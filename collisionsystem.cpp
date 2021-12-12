#include "collisionsystem.h"
#include "components.h"
#include "gameobject.h"

#include <vector>

CollisionSystem::CollisionSystem()
{

}

bool CollisionSystem::isColliding(GameObject *player, std::vector<GameObject*> objects, QVector3D playerNextPos)
{
    // !! Hardcoded that player is object nr 0
//    QVector3D playerPos {objects[0]->mTransformComp->mMatrix.getPosition().x,
//                objects[0]->mTransformComp->mMatrix.getPosition().y,
//                objects[0]->mTransformComp->mMatrix.getPosition().z};
    QVector3D playerPos = playerNextPos;

    QVector3D playerMaxCorner = player->mCollisionComp->maxCorner;
    QVector3D playerMinCorner = player->mCollisionComp->minCorner;

    for(unsigned long long i{2}; i < objects.size() ; i++)
    {
        QVector3D otherPos = {objects[i]->mTransformComp->mMatrix.getPosition().x,
                              objects[i]->mTransformComp->mMatrix.getPosition().y,
                              objects[i]->mTransformComp->mMatrix.getPosition().z};
        QVector3D otherMaxCorner = objects[i]->mCollisionComp->maxCorner;
        QVector3D otherMinCorner = objects[i]->mCollisionComp->minCorner;

        bool xAxis = playerPos.x() + playerMinCorner.x() <= otherPos.x() + otherMaxCorner.x() &&
                otherPos.x() + otherMinCorner.x() < playerPos.x() + playerMaxCorner.x();
        bool zAxis = playerPos.z() + playerMinCorner.z() <= otherPos.z() + otherMaxCorner.z() &&
                otherPos.z() + otherMinCorner.z() < playerPos.z() + playerMaxCorner.z();
        bool yAxis =playerPos.y() + playerMinCorner.y() <= otherPos.y() + otherMaxCorner.y() &&
                otherPos.y() + otherMinCorner.y() <= playerPos.y() + playerMaxCorner.y();
//        qDebug() << "obj nr: " << i;
//   qDebug() << "max: " << otherPos + otherMaxCorner << "min: " << otherPos + otherMinCorner;
//   qDebug() << "pos: " << otherPos;
        if(xAxis && zAxis && yAxis)
        {
            //qDebug() << i;
            return true;
        }
    }
    return false;
}

void CollisionSystem::updateCollisionBox(GameObject *obj)
{
    if(obj->mCollisionComp->bRotated)
    {
        if(obj->mCollisionComp->bSetRotatedOnce)
        {
            obj->mCollisionComp->maxCornerRotated.setX(obj->mCollisionComp->maxCorner.z());
            obj->mCollisionComp->maxCornerRotated.setZ(obj->mCollisionComp->maxCorner.x());

            obj->mCollisionComp->minCornerRotated.setX(obj->mCollisionComp->minCorner.z());
            obj->mCollisionComp->minCornerRotated.setZ(obj->mCollisionComp->minCorner.x());
            obj->mCollisionComp->bSetRotatedOnce = false;
        }
        obj->mCollisionComp->maxCorner.setX(obj->mCollisionComp->maxCornerRotated.x());
        obj->mCollisionComp->maxCorner.setZ(obj->mCollisionComp->maxCornerRotated.z());

        obj->mCollisionComp->minCorner.setX(obj->mCollisionComp->minCornerRotated.x());
        obj->mCollisionComp->minCorner.setZ(obj->mCollisionComp->minCornerRotated.z());
    }
    else
    {
        obj->mCollisionComp->maxCorner.setX(obj->mCollisionComp->maxCorner.x());
        obj->mCollisionComp->maxCorner.setZ(obj->mCollisionComp->maxCorner.z());

        obj->mCollisionComp->minCorner.setX(obj->mCollisionComp->minCorner.x());
        obj->mCollisionComp->minCorner.setZ(obj->mCollisionComp->minCorner.z());
//        qDebug() << "maxcorneNOTROTATED: " << obj->mCollisionComp->maxCorner << "mincornerNOTROTATED: " << obj->mCollisionComp->minCorner;
    }
}


