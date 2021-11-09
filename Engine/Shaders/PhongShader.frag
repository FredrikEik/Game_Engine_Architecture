#version 460 core

out vec4 fragmentColor;     //color sent to screen
in vec3 normalTransposed;
in vec3 fragmentPosition;
in vec3 color;

uniform float ambientStrength = 0.1f;
uniform float lightStrength = 0.7f;
uniform float objectAlpha = 1.f;
uniform vec3 lightColor = vec3(0.8, 0.8, 0.3); // ~yellow
uniform vec3 objectColor = vec3(0.7, 0.7, 0.7); // grey
uniform vec3 cameraPosition = vec3(50.f, 5.f, 50.f);

uniform float specularStrength = 0.5;
uniform int specularExponent = 64;

uniform vec3 lightPosition = vec3(100.f, 50.f, 200.f);

uniform int bUsingTexture = 0;
uniform sampler2D textureSampler;

void main() {
    vec2 UV = vec2(color[0], color[1]);
    // Ambient light
    vec3 ambient = ambientStrength * lightColor;

    //Diffuse lighting
    vec3 normalCorrected = normalize(normalTransposed);
    vec3 lightDirection = normalize(lightPosition-fragmentPosition);
    float angleFactor = max(dot(normalCorrected, lightDirection), 0.f);

    // Branchless way of swapping between texture and color.
    vec3 diffuse = angleFactor * objectColor * color * lightColor * lightStrength * (1-bUsingTexture) +
                    angleFactor * objectColor * texture(textureSampler, UV).rgb * lightColor * lightStrength * bUsingTexture;

    // specular light
    float specFactor = 0.0;
    vec3 viewDirection = normalize(cameraPosition-fragmentPosition);
    if(angleFactor > 0.f)
    {
        vec3 reflectDirection = reflect(-lightDirection, normalCorrected);
        specFactor = pow(max(dot(viewDirection, reflectDirection), 0.0), specularExponent);
    }
    vec3 specular = specFactor * lightColor * specularStrength;

    vec3 result = ambient + diffuse + specular;

    fragmentColor = vec4(result, objectAlpha);
}