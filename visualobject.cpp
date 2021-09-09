#include "visualobject.h"
#include "shader.h"
#include "string"

VisualObject::VisualObject()
{
}

VisualObject::VisualObject(std::string name)
    : mName(name)
{
    ID = ID++;
}

VisualObject::~VisualObject()
{
   glDeleteVertexArrays( 1, &mMeshComp->mVAO );
   glDeleteBuffers( 1, &mMeshComp->mVBO );
}

void VisualObject::init()
{
}
