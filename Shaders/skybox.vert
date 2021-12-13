#version 410 core
layout (location = 0) in vec3 aPos;

out vec3 Texturecoordinates;
uniform mat4 projection;
uniform mat4 view;
//uniform mat4 model;

void main(void)
{

    vec3 position = vec3(-aPos.x, -aPos.y, -aPos.z);
    Texturecoordinates = position;

    mat4 viewWithPositionRemoved = view;
    viewWithPositionRemoved[3] = vec4(0,0,0,1);
    gl_Position = projection * viewWithPositionRemoved * vec4(position, 1.0);

//    Texturecoordinates = vec3(model * vec4(aPos, 1.0));
//    vec3 position = projection * view * model * vec4(aPos, 1.0);
//    gl_Position = position.xyww;
}
