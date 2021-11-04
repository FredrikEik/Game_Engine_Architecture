#include "collisionsystem.h"
#include "components.h"
#include "gameobject.h"

#include <vector>

CollisionSystem::CollisionSystem()
{

}

bool CollisionSystem::isColliding(std::vector<GameObject*> objects, QVector3D playerNextPos)
{
    // !! Hardcoded that player is object nr 0
//    QVector3D playerPos {objects[0]->mTransformComp->mMatrix.getPosition().x,
//                objects[0]->mTransformComp->mMatrix.getPosition().y,
//                objects[0]->mTransformComp->mMatrix.getPosition().z};
    QVector3D playerPos = playerNextPos;

    QVector3D playerMaxCorner = objects[0]->mCollisionComp->maxCorner;
    QVector3D playerMinCorner = objects[0]->mCollisionComp->minCorner;

    for(unsigned long long i{1}; i < objects.size() ; i++)
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
        qDebug() << "obj nr: " << i;
   qDebug() << "max: " << otherPos + otherMaxCorner << "min: " << otherPos + otherMinCorner;
   qDebug() << "pos: " << otherPos;
        if(xAxis && zAxis && yAxis)
        {
            qDebug() << i;
            return true;
        }
    }
    return false;
}


