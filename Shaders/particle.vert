#version 410 core

//Tatt fra Johannes sitt prosjekt
layout(location = 0) in vec4 posAttr;
layout(location = 1) in vec4 colAttr;
layout(location = 2) in vec3 vertexAttr;
layout(location = 3) in vec2 lifeAttr;
layout(location = 4) in vec2 sizeAttr;
layout(location = 5) in vec4 endColorAttr;

out vec4 col;
uniform mat4 mMatrix;
uniform mat4 vMatrix;
uniform mat4 pMatrix;

void main()
{
    float timeBlend = (lifeAttr.y - lifeAttr.x) / lifeAttr.y;
    float size = mix(sizeAttr.x, sizeAttr.y, timeBlend);

    vec3 cameraRight = vec3(vMatrix[0][0], vMatrix[1][0], vMatrix[2][0]);
    vec3 cameraUp = vec3(vMatrix[0][1], vMatrix[1][1], vMatrix[2][1]);
    vec3 pos = vertexAttr * size;

    vec3 vertexPosition = vec3(posAttr) + (cameraRight * pos.x) + (cameraUp * pos.y);

    col = mix(colAttr, endColorAttr, timeBlend);
//  col = vec4(1,1,1,1);

    gl_Position = pMatrix * vMatrix * vec4(vertexPosition, 1.0);
}
