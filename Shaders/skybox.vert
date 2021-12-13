#version 410 core
layout (location = 0) in vec3 aPos;

out vec3 Texturecoordinates;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main(void)
{
    Texturecoordinates = vec3(model * vec4(aPos, 1.0));
    vec4 position = projection * view * model * vec4(aPos, 1.0);
    gl_Position = position.xyww;
}
