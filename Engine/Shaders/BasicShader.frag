#version 460 core
out vec4 FragColor;

in vec4 Color;

uniform vec4 uColor;

void main()
{
	FragColor = uColor;
}