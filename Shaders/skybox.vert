#version 410 core

layout (location = 0) in vec3 aPos;

out vec3 TexCoords;

uniform mat4 mMatrix;
uniform mat4 vMatrix;
uniform mat4 pMatrix;

void main()
{
    TexCoords = aPos;
    gl_Position = pMatrix * vMatrix * mMatrix * vec4(aPos, 1.0);
}
