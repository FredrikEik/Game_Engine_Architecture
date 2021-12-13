#version 460 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoord;
layout(location = 2) in vec4 aBillboardCenter;
layout(location = 3) in vec4 aColor;
layout(location = 4) in vec2 aUVblend;
layout(location = 5) in vec2 aLife;
layout(location = 6) in vec2 aSize;
layout(location = 7) in vec4 aEndColor;

uniform mat4 modelMatrix;
uniform mat4 u_projection;
uniform mat4 u_view;

uniform bool bSpherical = true;
uniform int bisScreenSpaceOnly = 0;

uniform vec4 u_center;
uniform vec4 u_color;

out vec2 UV;
out vec4 color;

void main(void)
{
    float timeBlend = (aLife.y - aLife.x) / aLife.y;
    float size = mix(aSize.x, aSize.y, timeBlend);

    vec3 billboardCenter = vec3(aBillboardCenter);
    vec3 pos = aPos * size;
//    vec3 pos = aPos * aBillboardCenter.w;

    billboardCenter.x = billboardCenter.x;
    
    vec3 cameraRight = vec3(u_view[0][0], u_view[1][0], u_view[2][0]);
    vec3 cameraUp = vec3(u_view[0][1], u_view[1][1], u_view[2][1]);

//    vec3 vertexPosition = billboardCenter + (cameraRight * aPos.x) + ( vec3(0,1,0) * aPos.y);

       vec3 vertexPosition = billboardCenter + (cameraRight * pos.x) + ( cameraUp * pos.y);


    gl_Position = u_projection * u_view * vec4(vertexPosition, 1.0);
//    gl_Position = aBillboardCenter + vec4(aPos, 1);


    float posX = aUVblend.y - (aUVblend.x*floor(aUVblend.y/aUVblend.x));
    float posY = floor(aUVblend.y / aUVblend.x);
    float rowBlend = 1/aUVblend.x;
    float uvX= (posX / aUVblend.x)+(aTexCoord.x*rowBlend);
    float uvY = (posY / aUVblend.x)+(aTexCoord.y*rowBlend);

    UV = vec2(uvX, uvY);
    
//    color = aEndColor;
    color = mix(aColor, aEndColor, timeBlend);
//    color = vec4(timeBlend, 0, 0, 1);
}