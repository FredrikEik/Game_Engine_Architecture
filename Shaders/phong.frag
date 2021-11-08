#version 410 core

//in vec4 col;
in vec3 FragPosition;
in vec3 normalTransposed;
in vec2 UV;

out vec4 fragColor;

uniform vec3 lightPosition;  //Test with static light in position: x:0, y:20, z:0
uniform vec3 cameraPosition;

uniform vec3 objectColor = vec3(0.7, 0.7, 0.7); //gray
uniform vec3 lightColor = vec3(0.93, 0.87, 0.5);  //yellow/white
uniform float ambientStrength = 0.2;

uniform float lightStrength = 1;
uniform float specularStrength = 0.5;
uniform int specularExponent = 16;


void main()
{
/** Ambient light */

   vec3 ambient = ambientStrength * lightColor;

/** Diffuse light */

   vec3 normalized = normalize( normalTransposed );
   vec3 lightDirection = normalize( lightPosition - FragPosition );
   float angleFactor = max(dot(normalized, lightDirection), 0.0);
   vec3 diffuse = angleFactor * lightColor * lightStrength;

/** Specular light */
   float spec = 0.0;
   vec3 viewDirection = normalize(cameraPosition - FragPosition);
   if (angleFactor > 0.0)     //calculations only needed if diffuse is above 0.0
   {
     vec3 reflectionDirection = reflect(-lightDirection, normalized);
     spec = pow(max(dot(viewDirection, reflectionDirection), 0.0), specularExponent);
   }
   vec3 specular = spec * lightColor * specularStrength;

/** Output */

   vec3 result = vec3(ambient + diffuse + specular) * objectColor;
   fragColor = vec4(result, 1);
}
