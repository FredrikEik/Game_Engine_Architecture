#version 410 core

layout (location = 0) in vec3 vpos;
layout (location = 1) in vec4 col;
layout (location = 2) in vec2 vert;


uniform mat4 mMatrix;
uniform mat4 vMatrix;
uniform mat4 pMatrix;

out vec3 texCoords;

void main()
{
  texCoords = vpos;

  mat3 viewWithoutTranslation = mat3(vMatrix);
  mat4 viewFinished = mat4(viewWithoutTranslation);

  gl_Position = pMatrix * vMatrix * mMatrix *vec4(vpos, 1.0);
}
