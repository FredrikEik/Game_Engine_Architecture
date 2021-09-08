#include "visualobject.h"
#include "shader.h"

VisualObject::VisualObject()
{
}

VisualObject::~VisualObject()
{
   glDeleteVertexArrays( 1, &mesh->mVAO );
   glDeleteBuffers( 1, &mesh->mVBO );
}

void VisualObject::init()
{

}
