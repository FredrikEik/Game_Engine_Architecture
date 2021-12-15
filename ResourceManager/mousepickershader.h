#ifndef MOUSEPICKERSHADER_H
#define MOUSEPICKERSHADER_H
#include "shaderhandler.h"
#include "math_constants.h"
#include "constants.h"
#include "vector2d.h"
#include "vector3d.h"
#include "matrix3x3.h"
#include "matrix4x4.h"
#include "components.h"


class mousepickershader : public ShaderHandler
{
public:
    mousepickershader(const std::string &shaderName);

    void setupShader() override;
    void use(gsl::Matrix4x4 &modelMatrixIn, struct ecs::Material * = nullptr) override;

    GLint mPickerColor{-1};
};

#endif // MOUSEPICKERSHADER_H
