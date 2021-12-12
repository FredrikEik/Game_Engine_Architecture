#version 460 core
layout(location = 0) in vec4 aPos;   // 1st attribute buffer = vertex positions
layout(location = 2) in vec2 aTexCoord;   // 2nd attribute buffer = normals

out vec2 TexCoords;

uniform mat4 u_model;                    //the matrix for the model
uniform mat4 u_view;
uniform mat4 u_projection;


void main() {

   TexCoords = aTexCoord;
   gl_Position = u_projection * u_view * u_model * aPos;
}