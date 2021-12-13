//Based of LearnOpenGl's Brian Will https://www.youtube.com/watch?v=QYvi1akO_Po
#version 410 core
out vec4 FragColor;

in vec4 Texturecoordinates;

uniform samplerCube skybox;

void main()
{
    FragColor = texture(skybox, Texturecoordinates);
}

//Tried to get skybox into materials.json
//        {
//            "name": "Skybox",
//            "shader": "Skybox",
//            "texture": "NightCubemap.jpg",
//            "color": [0.5, 0.5, 0.5]
//        }
