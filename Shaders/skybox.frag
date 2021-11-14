//Based of LearnOpenGl's Brian Will https://www.youtube.com/watch?v=QYvi1akO_Po
#version 330 core
out vec4 FragColor;

in vec4 Texturecoordinates;

uniform samplerCube skybox;

void main()
{
    FragColor = texture(skybox, Texturecoordinates);
}
