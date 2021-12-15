#version 410 core

in vec3 UV;
out vec4 FragColor;
uniform samplerCube skybox;

//uniform samplerCube gCubemapTexture;
void main()
{
    FragColor = texture(skybox, UV);
}
