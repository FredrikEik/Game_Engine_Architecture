#ifndef PHONGSHADER_H
#define PHONGSHADER_H

#include "shader.h"

class PhongShader : public Shader
{
public:
    PhongShader(const GLchar *vertexPath, const GLchar *fragmentPath);

    virtual void setup() override;
};

#endif // PHONGSHADER_H
