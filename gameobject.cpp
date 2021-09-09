#include "gameobject.h"

GameObject::GameObject()
{
    meshComp = new MeshComponent();
    transformComp = new TransformComponent();
    materialComp = new MaterialComponent();
    transformComp->mMatrix.setToIdentity();
}

GameObject::~GameObject()
{
    //TODO: do this in the proper component
   //glDeleteVertexArrays( 1, &mVAO );
   //glDeleteBuffers( 1, &mVBO );
}
