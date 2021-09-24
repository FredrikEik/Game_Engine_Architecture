#ifndef COMPONENTS_H
#define COMPONENTS_H

#include "constants.h"
#include "matrix4x4.h"
#include "vertex.h"
#include "gltypes.h"


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

    GLenum mDrawType{GL_TRIANGLES};
    GLint mMatrixUniform{0};
};

struct MaterialComponent
{
public:
    MaterialComponent();
    GLuint mShaderProgram;
    GLint mTextureUnit;

    // Texture shader uniforms
    GLint mTextureUniform{-1};

    // Skybox shader uniforms
    GLint mSkyboxUniform{-1};

    // Phong shader uniforms
    GLint mLightColorUniform{-1};
    GLint mObjectColorUniform{-1};
    GLint mAmbientLightStrengthUniform{-1};
    GLint mAmbientColor{-1};
    GLint mLightPositionUniform{-1};
    GLint mCameraPositionUniform{-1};
    GLint mSpecularStrengthUniform{-1};
    GLint mSpecularExponentUniform{-1};
    GLint mLightPowerUniform{-1};
    GLint mConstantUniform{-1};
    GLint mLinearUniform{-1};
    GLint mQuadraticUniform{-1};
    GLint mPhongTextureUniform{-1};

};
struct CollisionComponent
{
public:
    CollisionComponent();
    float cpxOne = 0;
    float cpyOne = 0;
    float cpxTwo = 0;
    float cpyTwo = 0;

    float cpxOneLocal = 0;
    float cpyOneLocal = 0;
    float cpxTwoLocal = 0;
    float cpyTwoLocal = 0;
    QMatrix4x4 mMatrix;
};

#endif // COMPONENTS_H
