#version 460 core
layout (location = 0) in vec4 aPos;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;
void main()
{
	gl_Position = u_projection * u_view * u_model * aPos;
}