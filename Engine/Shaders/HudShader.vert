#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec4 Color;
out vec2 UV;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

uniform vec4 u_center;

void main()
{
//	Color = vec4(1,0.2,0.7,0.5);
    vec3 center = vec3(u_center);
    vec3 pos = vec3(aPos);
    pos.x = pos.x * u_center.z;
    pos.y = pos.y * u_center.w;
    gl_Position = vec4(center.xy,0, 0) + vec4(pos.xy, 0, 1);
    UV = aTexCoord;
}