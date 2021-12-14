#version 460 core
layout(location = 0) in vec3 aPos;   // 1st attribute buffer = vertex positions
layout(location = 2) in vec2 aTexCoord;   // 2nd attribute buffer = normals

out vec2 TexCoords;


void main() {

   TexCoords = aTexCoord;
   gl_Position = vec4(aPos, 1.0);
}