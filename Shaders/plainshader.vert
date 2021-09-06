#version 410 core

layout(location = 0) in vec4 positionIn;   // 1st attribute buffer = vertex positions
layout(location = 1) in vec4 colorIn;   // 2nd attribute buffer = colors
out vec4 color;                           //color sent to rest of pipeline
uniform mat4 modelMatrix;                    //the matrix for the model
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main() {
   color = colorIn;                       //passing on the vertex color
   gl_Position = projectionMatrix * viewMatrix * modelMatrix * positionIn;  //calculate the position of the model
}
