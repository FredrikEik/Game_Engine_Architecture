#include "gameobject.h"

GameObject::GameObject()
{
    transformComp.mMatrix.setToIdentity();
}

GameObject::~GameObject()
{
   //TODO: do this in the proper component
   //glDeleteVertexArrays( 1, &meshComp->mVAO );
   //glDeleteBuffers( 1, &meshComp->mVBO );
}

void GameObject::draw()
{
    glBindVertexArray( meshComp.mVAO );
    if(meshComp.mIndexCount > 0)
    {
        glDrawElements(meshComp.mDrawType, meshComp.mIndexCount, GL_UNSIGNED_INT, nullptr);
    }
    else
    {
        glDrawArrays(meshComp.mDrawType, 0, meshComp.mVertexCount);
    }
    glBindVertexArray(0);
}

void GameObject::move(float x, float y, float z)
{
    transformComp.mMatrix.translate(x,y,z);
    sphereCollisionComp.center += gsl::Vector3D(x,y,z);
}
