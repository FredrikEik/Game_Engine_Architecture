#version 410 core
layout(location = 0) in vec3 posAttr;
layout(location = 1) in vec4 colAttr;
layout(location = 2) in vec2 vertexUV;

out vec3 UV;
uniform mat4 mMatrix;
uniform mat4 vMatrix;
uniform mat4 pMatrix;

void main() {


    UV = posAttr;
    gl_Position = pMatrix * vMatrix * mMatrix * vec4(posAttr, 1.0);

}
