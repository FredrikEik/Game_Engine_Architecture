#version 410 core

layout(location = 0) in vec3 positionIn;   // 1st attribute buffer = vertex positions
layout(location = 1) in vec3 vertexNormal;   // 2nd attribute buffer = normals
layout(location = 2) in vec2 vertexUV;

//out vec4 color;      //color sent to rest of program
out vec3 fragmentPosition;
out vec3 normalTransposed;
out vec2 UV;

uniform mat4 modelMatrix;                    //the matrix for the model
uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;


void main() {
    UV = vertexUV;

    fragmentPosition = vec3(modelMatrix * vec4(positionIn, 1.0));
    normalTransposed = mat3(transpose(inverse(modelMatrix))) * vertexNormal;
    //normalTransposed = vertexNormal;

    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(positionIn, 1.0);     //calculate the position of the model

}
