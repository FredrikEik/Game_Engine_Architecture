#version 460 core
layout(location = 0) in vec4 aPos;   // 1st attribute buffer = vertex positions
layout(location = 1) in vec4 aColor;
layout(location = 2) in vec2 aTexCoord;   // 2nd attribute buffer = normals
//layout(location = 3) in vec2 vertexUV;
//out vec4 color;      //color sent to rest of pipeline
out vec3 fragmentPosition;
out vec3 normalTransposed;
out vec3 color;


uniform mat4 u_model;                    //the matrix for the model
uniform mat4 u_view;
uniform mat4 u_projection;


void main() {
    //color = colorIn;   //passing on the vertex color
    fragmentPosition = vec3(modelMatrix * vec4(positionIn, 1.0));
    normalTransposed = mat3(transpose(inverse(modelMatrix))) * vertexNormal; // TODO: This should be done on the CPU and sent in via uniform instead
    //normalTransposed = vertexNormal;
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(positionIn, 1.0);     //calculate the position of the model
    color = colorIn;
}