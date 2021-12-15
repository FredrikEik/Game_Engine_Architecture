#include "gameobject.h"
#include "math.h"

GameObject::GameObject()
{
    meshComp = new MeshComponent();
    transformComp = new TransformComponent();
    materialComp = new MaterialComponent();
    sphereCollisionComp = new SphereCollisionComponent();
    transformComp->mMatrix.setToIdentity();
}

GameObject::~GameObject()
{

}

void GameObject::checkLodDistance(gsl::Vector3D distance, float cameraDistance)
{
    if (abs(distance.x) < cameraDistance && abs(distance.z) < cameraDistance)
    {

        BadLod = false;
    }
    else
    {
        BadLod = true;
    }
}
