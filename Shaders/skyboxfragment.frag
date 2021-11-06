#version 410 core

in vec3 texCoords;
uniform samplerCube cubeSampler;
out vec4 fragColor;

void main()
{
  fragColor = texture(cubeSampler, texCoords);
}
