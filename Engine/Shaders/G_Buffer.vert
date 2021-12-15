#version 460 core
layout(location = 0) in vec3 aPos;   // 1st attribute buffer = vertex positions
layout(location = 1) in vec3 aNormals;
layout(location = 2) in vec2 aTexCoord;   


out vec2 TexCoords;
out vec3 FragPos;
out vec3 Normals;

uniform mat4 u_model;                    //the matrix for the model
uniform mat4 u_view;
uniform mat4 u_projection;

void main() {

   vec4 worldPos = u_model * vec4(aPos, 1.0);
   FragPos = worldPos.xyz; 
   mat3 normalMatrix = transpose(inverse(mat3(u_model)));

   Normals = normalMatrix * aNormals;
   TexCoords = aTexCoord; // rotateUV(aTexCoord, 180, vec2(0.5,0.5));
   gl_Position = u_projection * u_view * worldPos;
}

