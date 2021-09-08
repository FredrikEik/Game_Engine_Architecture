#ifndef PLAINSHADER_H
#define PLAINSHADER_H

#include "shader.h"

class PlainShader : public Shader
{
public:
    PlainShader(const GLchar *vertexPath, const GLchar *fragmentPath);

    virtual void setup() override;
};

#endif // PLAINSHADER_H
