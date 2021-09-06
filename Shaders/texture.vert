#version 410 core

layout(location = 0) in vec4 positionIn;
layout(location = 1) in vec4 colorIn;
layout(location = 2) in vec2 vertexUV;

out vec4 col;
out vec2 UV;
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main()
{
    col = colorIn;
    UV = vertexUV;
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * positionIn;
}
