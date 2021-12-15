#ifndef COLORSHADER_H
#define COLORSHADER_H

#include "shaderhandler.h"
#include "math_constants.h"
#include "constants.h"
#include "vector2d.h"
#include "vector3d.h"
#include "matrix3x3.h"
#include "matrix4x4.h"
#include "components.h"

class colorshader : public ShaderHandler
{
public:
    colorshader(const std::string &shaderN);

    void setupShader() override;
    void use(gsl::Matrix4x4 &modelMatrixIn, struct ecs::Material * = nullptr) override;
};

#endif // COLORSHADER_H
