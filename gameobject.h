#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "components.h"

class GameObject
{
public:
    std::string name{0};

    TransformComponent* mTransformComp{nullptr};
    MeshComponent* mMeshComp{nullptr};
    MaterialComponent* mMaterialComp{nullptr};
    CollisionComponent* mCollisionComp{nullptr};
    MeshComponent* mCollisionLines{nullptr};
};

#endif // GAMEOBJECT_H
