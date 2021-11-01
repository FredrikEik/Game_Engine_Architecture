#ifndef CUBEMAPSHADER_H
#define CUBEMAPSHADER_H

#include "shader.h"

class CubemapShader : public Shader
{
public:
    CubemapShader(const GLchar *vertexPath, const GLchar *fragmentPath);

    virtual void setup() override;
};

#endif // CUBEMAPSHADER_H
