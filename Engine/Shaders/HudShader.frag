#version 460 core

in vec4 Color;
in vec2 UV;

out vec4 fragmentColor;
uniform sampler2D textureSampler;

void main()
{
    fragmentColor = texture2D(textureSampler, UV).rgba;
}