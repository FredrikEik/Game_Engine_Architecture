#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "components.h"

class GameObject
{
public:
    std::string name{0};
    int id{0};

    TransformComponent* mTransformComp{nullptr};
    MeshComponent* mMeshComp{nullptr};
    MaterialComponent* mMaterialComp{nullptr};
    CollisionComponent* mCollisionComp{nullptr};
    MeshComponent* mCollisionLines{nullptr};
    //BallPhysicsComponent* mBallPhysicsComp{nullptr};
};

#endif // GAMEOBJECT_H
