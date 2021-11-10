#version 410 core
out vec4 fragColor;         //The final color

in vec3 normalTransposed;   //Normal of fragment, already transposed
in vec3 fragmentPosition;
in vec3 vertexColors;
in vec2 UV;       //for textures

uniform bool usingTextures = false;
uniform sampler2D uTexture;

uniform vec3 lightPosition;
uniform vec3 cameraPosition;

uniform float ambientStrength = 0.5;
uniform float lightStrength = 0.9;
uniform vec3 lightColor = vec3(1, 1, 1);  //yellowish
uniform float specularStrength = 0.9;
uniform int specularExponent = 32;

uniform vec3 objectColor = vec3(1.3, 0.7, 0.3); //grey

void main() {

    //ambient
    float ambient = ambientStrength * lightStrength;

    //diffuse
    vec3 normalCorrected = normalize(normalTransposed);
    vec3 lightDirection = normalize(lightPosition - fragmentPosition);
    float angleFactor = max(dot(normalCorrected, lightDirection), 0.0);
    vec3 diffuse = angleFactor * vertexColors * lightColor * lightStrength;

    //specular
    vec3 viewDirection = normalize(lightPosition - fragmentPosition);
    float spec = 0.0;
    if (angleFactor > 0.0)     //calculations only needed if diffuse is above 0.0
    {
        vec3 reflectDirection = reflect(-lightDirection, normalCorrected);
        spec = pow(max(dot(viewDirection, reflectDirection), 0.0), specularExponent);
    }
    vec3 specular = spec * lightColor * specularStrength;

    //vec3 result = (ambient + diffuse + specular); //diffuse // + ;ambient


    vec3 result = (ambient + diffuse + specular ); //diffuse // + ;ambient

    if(usingTextures == true){
        fragColor = texture2D(uTexture, UV) * vec4(result, 1.0);
        return;
    }

    fragColor = vec4(result, 1.0);
}

//Using calculations in world space,
//https://learnopengl.com/Lighting/Basic-Lighting
//but could just as easy be done in camera space
//http://www.opengl-tutorial.org/beginners-tutorials/tutorial-8-basic-shading/
