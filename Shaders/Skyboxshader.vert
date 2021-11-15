#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 TexCoords;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 mMatrix;
uniform vec3 POS;
void main()
{
    TexCoords = aPos;
    gl_Position = projection * view *mMatrix* vec4(aPos, 1.0);
}
