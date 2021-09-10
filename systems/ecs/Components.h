#ifndef COMPONENTS_H
#define COMPONENTS_H


#include "constants.h"
#include "matrix4x4.h"
#include "vertex.h"
#include "gltypes.h"
#include "constants.h"
//namespace ecs{


struct TransformComponent{

    gsl::Matrix4x4 mMatrix;
};

struct MeshComponent{

    std::vector<Vertex> mVertices;
    std::vector<GLuint> mIndices;

    GLint mVertexCount{-1};
    GLint mIndexCount{-1};
    GLuint mVAO{0};
    GLuint mVBO{0};
    GLuint mEAB{0}; //holds the indices (Element Array Buffer - EAB)

     GLenum mDrawType{GL_TRIANGLES};

};

struct MaterialComponent{

    GLuint mShaderProgram{0};
    GLint mTextureUnit{0};
};

struct materialId
{
    MAX_ENTITIES_TYPE mEntityID{MAX_ENTITIES};
    unsigned short mMaterialIndex{0};        //Material 0 = default material

};

struct collider
{
    gsl::Colliders mType{gsl::NOCOLLIDER};

};


#endif // COMPONENTS_H
