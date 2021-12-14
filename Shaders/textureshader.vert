#version 410 core
layout(location = 0) in vec4 positionIn;
layout(location = 1) in vec4 colorIn;
layout(location = 2) in vec2 vertexUV;

out vec4 color;
out vec2 UV;
uniform mat4 mMatrix;
uniform mat4 vMatrix;
uniform mat4 pMatrix;
uniform int HUD;

void main() {
   color = colorIn;
   UV = vertexUV;

   if (HUD == 1){
    gl_Position = mMatrix * positionIn;
    }

   else
   gl_Position = pMatrix * vMatrix * mMatrix * positionIn;
}
