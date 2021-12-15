#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;


out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;


uniform mat4 u_projection;
uniform mat4 u_view;
uniform mat4 u_model;

uniform int u_reverse_normals = 0;

void main()
{
    FragPos = vec3(u_model * vec4(aPos, 1.0));

    Normal = transpose(inverse(mat3(u_model))) * ((-1.0 * u_reverse_normals) * aNormal) + 
    transpose(inverse(mat3(u_model))) * aNormal * (1 - u_reverse_normals);
    
    TexCoords = aTexCoords;
    gl_Position = u_projection * u_view * u_model * vec4(aPos, 1.0);
}