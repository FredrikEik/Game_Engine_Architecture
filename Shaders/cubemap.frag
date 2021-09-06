#version 410 core
#extension GL_NV_shadow_samplers_cube : enable

out vec3 fragmentColor;

in vec3 textureCoords;
uniform samplerCube cubeMap;

void main()
{
    fragmentColor = textureCube(cubeMap, textureCoords).rgb;
}
