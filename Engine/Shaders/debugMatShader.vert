#version 460 core
layout (location = 0) in vec4 aPos;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec2 aTexCoord;

out vec4 Color;
out vec2 texCord;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

void main()
{
	texCord = aTexCoord;
	Color = vec4(1,0.2,0.7,0.5);
	gl_Position = u_projection * u_view * u_model * aPos;
}