#version 410 core
out vec4 fragmentColor;     //The final color

in vec3 normalTransposed;   //Normal of fragment, already transposed
in vec3 fragmentPosition;
in vec2 UV;                 //for textures


uniform vec3 lightPosition = vec3(5, 2, -2);
uniform vec3 cameraPosition = vec3(5, 1, 2);
uniform vec3 lightColor = vec3(0.7, 0.7, 0.5); //(0.8, 0.8, 0.3);  //yellowish
uniform vec3 objectColor = vec3(1, 1, 1); //white

uniform vec3 lightDirection = vec3(0, 1, 0);    //For directional lights

uniform vec3 ambientColor = vec3(0.3, 0.3, 0.3);
uniform float ambientStrengt = 0.1;
uniform float lightStrengt = 1;
uniform float specularStrength = 1.5;
uniform int specularExponent = 32;
uniform sampler2D textureSampler;

void main() {
    //ambient
    vec3 ambient = ambientStrengt * ambientColor;

    //diffuse
    vec3 normalCorrected = normalize(normalTransposed);
    vec3 fragmentLightDirection = normalize(lightPosition - fragmentPosition);
//    vec3 fragmentLightDirection = normalize(lightPosition - vec3(0, 0, 0));  //Quick hack directional light
    float diff = max(dot(normalCorrected, fragmentLightDirection), 0.0);
    vec3 diffuse = diff * lightColor * lightStrengt;

    //specular
    vec3 viewDirection = normalize(cameraPosition - fragmentPosition);
    float spec = 0.0;
    if (diff > 0.0)     //calculations only needed if diffuse is above 0.0
    {
        vec3 reflectDirection = reflect(-fragmentLightDirection, normalCorrected);
        spec = pow(max(dot(viewDirection, reflectDirection), 0.0), specularExponent);
    }
    vec3 specular = spec * lightColor * specularStrength;

    vec3 lights = ambient + diffuse + specular;
    vec4 result = vec4(lights, 1) * texture(textureSampler, UV) * vec4(objectColor, 1);
    fragmentColor = vec4(result);
}

//Using calculations in world space,
//https://learnopengl.com/Lighting/Basic-Lighting
//but could just as easy be done in camera space
//http://www.opengl-tutorial.org/beginners-tutorials/tutorial-8-basic-shading/
