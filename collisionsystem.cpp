#include "collisionsystem.h"
#include "components.h"
#include "gameobject.h"

#include <vector>

CollisionSystem::CollisionSystem()
{

}

bool CollisionSystem::isColliding(std::vector<GameObject*> objects)
{
    // !! Hardcoded that player is object nr 0
    QVector3D playerPos {objects[0]->mTransformComp->mMatrix.getPosition().x,
                objects[0]->mTransformComp->mMatrix.getPosition().y,
                objects[0]->mTransformComp->mMatrix.getPosition().z};

    QVector3D playerMaxCorner = objects[0]->mCollisionComp->maxCorner;
    QVector3D playerMinCorner = objects[0]->mCollisionComp->minCorner;
    for(int i{1}; i < objects.size() ; i++)
    {
        QVector3D otherPos = {objects[i]->mTransformComp->mMatrix.getPosition().x,
                              objects[i]->mTransformComp->mMatrix.getPosition().y,
                              objects[i]->mTransformComp->mMatrix.getPosition().z};
        QVector3D otherMaxCorner = objects[i]->mCollisionComp->maxCorner;
        QVector3D otherMinCorner = objects[i]->mCollisionComp->minCorner;

        if(playerPos.x() + playerMaxCorner.x() >= otherPos.x() + otherMinCorner.x() &&
                otherPos.x() + otherMaxCorner.x() >= playerPos.x() + playerMinCorner.x())
        {
            return true;
        }
    }
    return false;
}


