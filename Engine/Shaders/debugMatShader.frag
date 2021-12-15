#version 460 core

in vec4 Color;
in vec2 texCord;
out vec4 FragColor;

uniform sampler2D u_tex_diffuse1;
uniform sampler2D u_tex_specular1;

void main()
{
    vec4 AlbedoSpec;
    AlbedoSpec.rgb = texture(u_tex_diffuse1, texCord).rgb;
    AlbedoSpec.a = texture(u_tex_specular1, texCord).r;

	FragColor = AlbedoSpec;
}