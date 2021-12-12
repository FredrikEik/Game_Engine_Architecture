#version 460 core
layout(location = 0) in vec4 aPos;   // 1st attribute buffer = vertex positions
layout(location = 1) in vec3 aNormals;
layout(location = 2) in vec2 aTexCoord;   // 2nd attribute buffer = normals


out vec3 FragPos;
out vec3 Normals;
out vec2 TexCoords;

uniform mat4 u_model;                    //the matrix for the model
uniform mat4 u_view;
uniform mat4 u_projection;


void main() {

   FragPos = aPos.xyz;
   mat3 normalMatrix = transpose(inverse(mat3(u_model)));
   Normals = normalMatrix * aNormals;
   TexCoords = aTexCoord;
   gl_Position = u_projection * u_view * u_model * aPos;
}