#include "gameobject.h"


void GameObject::move(float x, float y, float z)
{
    mTransform->mMatrix.translate(x, y, z);
}
