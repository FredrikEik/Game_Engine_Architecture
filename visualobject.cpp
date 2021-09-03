#include "visualobject.h"
#include "shader.h"

VisualObject::VisualObject()
{
}

VisualObject::~VisualObject()
{
    //TODO: do this in the proper component
   //glDeleteVertexArrays( 1, &mVAO );
   //glDeleteBuffers( 1, &mVBO );
}

void VisualObject::init()
{
}
