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

class MaterialComponent
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

    void setupUniforms();


};

struct LightComponent
{
    gsl::Vector3D lightColor();
};

struct BoxCollisionComponent
{
public:
    gsl::Vector3D min, max;

};

struct SphereCollisionComponent
{
public:
    gsl::Vector3D center;
    float radius;
};

struct FrustumCollisionComponent
{
public:
    // near plane
    float nearPlaneLength;
    gsl::Vector3D leftBottomNear;
    gsl::Vector3D rightBottomNear;
    gsl::Vector3D leftTopNear;
    gsl::Vector3D rightTopNear;

    // far plane
    float farPlaneLength;
    gsl::Vector3D leftBottomFar;
    gsl::Vector3D rightBottomFar;
    gsl::Vector3D leftTopFar;
    gsl::Vector3D rightTopFar;

    gsl::Matrix4x4 mMatrix;
};

#endif // COMPONENTS_H
