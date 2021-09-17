#include "gameobject.h"

GameObject::GameObject()
{
    MeshComp = new MeshComponent();
    TransformComp = new TransformComponent();
    MaterialComp = new MaterialComponent();
    TransformComp->mMatrix.setToIdentity();
}

GameObject::~GameObject()
{

}
