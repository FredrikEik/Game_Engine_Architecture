#version 460 core

in vec2 UV;
in vec4 color;
uniform sampler2D textureSampler;
out vec4 fragmentColor;

void main(void)
{
//    fragmentColor = texture2D(textureSampler, UV).rgba;
    fragmentColor = color;
//    fragmentColor = vec4(1,1,1,1);
}