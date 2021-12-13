#version 410 core
layout(location = 0) in vec4 positionIn;
layout(location = 1) in vec4 colorIn;
layout(location = 2) in vec2 vertexUV;

out vec4 color;
out vec2 UV;
uniform mat4 mMatrix;
uniform mat4 vMatrix;
uniform mat4 pMatrix;
uniform int HudElement;

void main() {
   color = colorIn;
   UV = vertexUV;
   if (HudElement == 1)
   {
      gl_Position = mMatrix *positionIn;
      //gl_Position = vec4(gl_Position.x, gl_Position.y, gl_Position.z, 0.999f);
   }
   else
   {
       gl_Position = pMatrix * vMatrix * mMatrix * positionIn;
   }
}
