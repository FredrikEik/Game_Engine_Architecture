#include "visualobject.h"
#include "shader.h"

VisualObject::VisualObject()
{
}

VisualObject::~VisualObject()
{
   glDeleteVertexArrays( 1, &mMeshComp->mVAO );
   glDeleteBuffers( 1, &mMeshComp->mVBO );
}

void VisualObject::init()
{
}
