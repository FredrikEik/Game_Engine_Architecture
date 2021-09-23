#ifndef COMPONENTTYPES_H
#define COMPONENTTYPES_H

#include "gltypes.h"
#include <vertex.h>
#include "matrix4x4.h"



struct Transform
{
public:
    gsl::Matrix4x4 mMatrix;

};

struct Mesh
{
public:
       GLuint mVAO{0};
       GLuint mVBO{0};
       GLuint mEAB{0};
       GLint mVertexCount{-1};
       GLint mIndexCount{-1};
       GLenum mDrawType{GL_TRIANGLES};
       std::vector<Vertex> mVertices;      //this should be moved out of component
       std::vector<GLuint> mIndices;
};

struct Material
{
public:
    GLuint mShaderProgram{0};
    GLint mTextureUnit{0};
};

struct Sound
{
public:

};

#endif // COMPONENTTYPES_H
