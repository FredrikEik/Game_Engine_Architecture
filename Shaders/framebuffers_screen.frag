#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;

void main()
{
    FragColor = texture(screenTexture, TexCoords);
    float average = 0.2126 * FragColor.r + 0.7152 * FragColor.g + 0.0722 * FragColor.b;
    FragColor = vec4(average, average, average, 1.0);

    FragColor = vec4(vec3(1.0 - texture(screenTexture, TexCoords)), 1.0);
}




/*#version 330 core

out vec4 FragColor;
in vec2 texCoords;


uniform sampler2D screenTexture;


void main()
{   vec4 tex = texture(screenTexture, texCoords);
    float avg = (tex.x + tex.y + tex.z) / 3.0f;
    FragColor = vec4(avg, avg, avg, 1.0f);
}*/






/*#version 330 core

out vec4 FragColor;
in vec2 texCoords;

uniform sampler2D screenTexture;

const float offset_x = 1.0f / 800.0f;
const float offset_y = 1.0f / 800.0f;

vec2 offsets[9] = vec2[]
(
    vec2(-offset_x,  offset_y), vec2( 0.0f,    offset_y), vec2( offset_x,  offset_y),
    vec2(-offset_x,  0.0f),     vec2( 0.0f,    0.0f),     vec2( offset_x,  0.0f),
    vec2(-offset_x, -offset_y), vec2( 0.0f,   -offset_y), vec2( offset_x, -offset_y)
);

float kernel[9] = float[]
(
    1,  1, 1,
    1, -4, 1,
    1,  1, 1
);

void main()
{
    vec3 color = vec3(0.0f);
    for(int i = 0; i < 9; i++)
        color += vec3(texture(screenTexture, texCoords.st + offsets[i])) * kernel[i];
    FragColor = vec4(color, 1.0f);
}*/
