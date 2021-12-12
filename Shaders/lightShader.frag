#version 410 core
out vec4 fragColor;         //The final color

in vec3 normalTransposed;   //Normal of fragment, already transposed
in vec3 fragmentPosition;
in vec2 UV;

uniform sampler2D textureSampler;

uniform vec3 lightPosition = vec3(0.0,0.0,0.0);
uniform vec3 cameraPosition;

uniform float ambientStrength = 0.15f;
uniform vec3  ambientColor = vec3(0.8f, 0.8f, 0.15f);
uniform float lightStrength = 1.5f;
uniform vec3  lightColor = vec3(0.5f, 0.5f, 0.0f);
uniform float specularStrength = 0.7f;
uniform int   specularExponent = 32;
uniform float constant = 1.0f;
uniform float linear = 0.14f;
uniform float quadratic = 0.07f;

uniform vec3 objectColor = vec3(0.2, 0.5, 0.3); //grey

void main() {
    // Attenuation
    float distance = length(lightPosition - fragmentPosition);
    float attenuation = 1.0 / (constant + linear * distance + quadratic * (distance * distance));

    //ambient
    vec3 ambient = ambientStrength * ambientColor;

    //diffuse
    vec3 normalCorrected = normalize(normalTransposed);
    vec3 lightDirection = normalize(lightPosition - fragmentPosition);
    float angleFactor = max(dot(normalCorrected, lightDirection), 0.0);
    vec3 diffuse = angleFactor * objectColor * lightColor * lightStrength;

    //specular
    vec3 viewDirection = normalize(cameraPosition - fragmentPosition);
    float spec = 0.0;
    if (angleFactor > 0.0)     //calculations only needed if diffuse is above 0.0
    {
        vec3 reflectDirection = reflect(-lightDirection, normalCorrected);
        spec = pow(max(dot(viewDirection, reflectDirection), 0.0), specularExponent);
    }
    vec3 specular = spec * lightColor * specularStrength;

    vec3 result = (ambient + diffuse + specular) * attenuation;

    fragColor = vec4(result, 1.0) + texture(textureSampler, UV);


}
