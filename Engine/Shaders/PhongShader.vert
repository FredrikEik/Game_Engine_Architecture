#version 460 core
layout(location = 0) in vec3 aPos;   // 1st attribute buffer = vertex positions
layout(location = 1) in vec3 aNormals;
layout(location = 2) in vec2 aTexCoord;   // 2nd attribute buffer = normals

out vec3 fragmentPosition;
out vec3 normalTransposed;
out vec4 color;
out vec2 UV;


uniform mat4 u_model;                    //the matrix for the model
uniform mat4 u_view;
uniform mat4 u_projection;


void main() {

    fragmentPosition = vec3(u_model * vec4(aPos, 1.0));
    normalTransposed = mat3(transpose(inverse(u_model))) * aNormals;

    gl_Position = u_projection * u_view * u_model * vec4(aPos, 1.0);     //calculate the position of the model
    color = vec4(aNormals, 1);
    UV = aTexCoord;

}