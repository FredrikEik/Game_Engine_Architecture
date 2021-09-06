#version 330 core
layout (location = 0) in vec3 positionIn;

out vec3 textureCoords;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;

void main()
{
    vec3 pos = vec3(-positionIn.x, -positionIn.y, positionIn.z); //Go the right way, dammit!
    textureCoords = pos;
    mat4 viewWithoutPos = viewMatrix;
    viewWithoutPos[3] = vec4(0,0,0,1);
    gl_Position = projectionMatrix * viewWithoutPos * vec4(positionIn, 1.0);
}
