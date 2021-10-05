#include "GameObject.h"

void GameObject::move(float dx, float dy, float dz)
{
    this->transform->mMatrix.translate(dx, dy, dz);
}
