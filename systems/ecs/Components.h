#ifndef COMPONENTS_H
#define COMPONENTS_H


#include "constants.h"
#include "matrix4x4.h"
#include "vertex.h"
#include "gltypes.h"
#include "constants.h"
//namespace ecs{


struct TransformComponent{
    gsl::Vector3D position{0, 0, 0};
    gsl::Vector3D  rotation{0, 0, 0};
    gsl::Vector3D  scale{1};
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
    float mColliderRadius{0.f};
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
    gsl::Vector3D mMinCenter{-.5f, -.5f, -.5f};     //xyzMin, xyzMax if box, center, radius if sphere
    gsl::Vector3D mMaxRadius{.5f, .5f, .5f};
    bool bIsDynamic{false}; //static or dynamic collider
};


#endif // COMPONENTS_H
