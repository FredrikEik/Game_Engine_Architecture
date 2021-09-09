#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "components.h"

class GameObject
{
public:

    TransformComponent* mTransformComp{nullptr};
    MeshComponent* mMeshComp{nullptr};
    MaterialComponent* mMaterialComp{nullptr};
};

#endif // GAMEOBJECT_H
