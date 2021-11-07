#version 410 core

layout(location = 0) in vec3 posAttr;
layout(location = 1) in vec3 normalAttr;
layout(location = 2) in vec2 uvAttr;     //Used if object has a texture.


//out vec4 col;                          //Using normals instead of colors for now.

out vec3 Normal;
out vec3 normalTransposed;
out vec3 FragPosition;

uniform mat4 mMatrix;
uniform mat4 vMatrix;
uniform mat4 pMatrix;

void main()
{
   Normal = normalAttr;
   FragPosition = vec3(mMatrix * vec4(posAttr, 1.0));
   normalTransposed = mat3(transpose(inverse(mMatrix))) * Normal;

   gl_Position = pMatrix * vMatrix * mMatrix * vec4(posAttr, 1.0);
}
