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
    //TODO: do this in the proper component
   //glDeleteVertexArrays( 1, &mVAO );
    //glDeleteBuffers( 1, &mVBO );
}

void GameObject::checkLodDistance(gsl::Vector3D distance, float cameraDistance)
{
    if (abs(distance.x) < cameraDistance && abs(distance.z) < cameraDistance)
    {
       // qDebug() << abs(distance.x);
       // qDebug() << abs(distance.z);
        BadLod = false;
       // qDebug() << BadLod;
    }
    else
    {
        BadLod = true;
       // qDebug() << BadLod;
    }
}
