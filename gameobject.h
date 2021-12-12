#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "components.h"
#include <qstring.h>

class GameObject
{
public:
    std::string name{0};
    std::string filepath{"-"};
    int id{0};

    TransformComponent* mTransformComp{nullptr};
    MeshComponent* mMeshComp{nullptr};
    MaterialComponent* mMaterialComp{nullptr};
    CollisionComponent* mCollisionComp{nullptr};
    MeshComponent* mCollisionLines{nullptr};
    AIComponent* mAIComponent{nullptr};
    BallPhysicsComponent* mBallPhysicsComp{nullptr};
};

#endif // GAMEOBJECT_H
