#ifndef TEXTURESHADER_H
#define TEXTURESHADER_H

#include "shader.h"

class TextureShader : public Shader
{
public:
    TextureShader(const GLchar *vertexPath, const GLchar *fragmentPath);

    virtual void setup() override;

protected:

};

#endif // TEXTURESHADER_H
