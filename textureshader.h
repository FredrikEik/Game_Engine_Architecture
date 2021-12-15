#ifndef TEXTURESHADER_H
#define TEXTURESHADER_H

#include "shaderhandler.h"
#include "math_constants.h"
#include "constants.h"
#include "vector2d.h"
#include "vector3d.h"
#include "matrix3x3.h"
#include "matrix4x4.h"
#include "components.h"

class textureshader : public ShaderHandler
{
public:
    textureshader(const std::string &shaderName);

    void setupShader() override;
    void use(gsl::Matrix4x4 &modelMatrixIn, struct ecs::Material *materialIn) override;
    GLint mTextureUniform{-1};
};

#endif // TEXTURESHADER_H
