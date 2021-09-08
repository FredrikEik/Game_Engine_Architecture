#ifndef COMPONENTS_H
#define COMPONENTS_H

#include <QOpenGLFunctions_4_1_Core>
#include "constants.h"
#include "matrix4x4.h"
#include "gltypes.h"
#include "vertex.h"



struct TransformComponent
{
public:
    TransformComponent();
    gsl::Matrix4x4 mMatrix;
};

struct MeshComponent
{
public:
    MeshComponent();

    std::vector<Vertex> mVertices;
    std::vector<GLuint> mIndices;

    GLuint mVAO{0};
    GLuint mVBO{0};
    GLuint mEAB{0};

    GLint mMatrixUniform{0};
};

struct TextureComponent
{
    TextureComponent();

    GLuint mShaderProgram{0};
    GLint mTextureUnit{0};
};

struct CollisionComponent
{

};


#endif // COMPONENTS_H
