#version 410 core

out vec4 fragmentColor;     //output color
in vec3 normalTransposed;
in vec3 fragmentPosition;


uniform float ambientStrength = 0.1f;
uniform float lightStrength = 0.6f;
uniform float objectAlpha = 1.f;

uniform vec3 lightColor = vec3(0.8, 0.8, 0.3);  //Kinda yellow
uniform vec3 objectColor = vec3(0.2, 0.2, 0.2);  // grey
uniform vec3 cameraPosition = vec3(50.f, 5.f, 50.f);
uniform vec3 lightPosition = vec3(100.f, 50.f, 200.f);

uniform float specularStrength = 0.6;
uniform int specularExponent = 64;


void main() {
    // Ambient light
    vec3 ambient = ambientStrength * lightColor;

     //Diffuse lighting
    vec3 normalCorrected = normalize(normalTransposed);
    vec3 lightDirection = normalize(lightPosition-fragmentPosition);
    float angleFactor = max(dot(normalCorrected, lightDirection), 0.f);
    vec3 diffuse = angleFactor * objectColor * lightColor * lightStrength;

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
