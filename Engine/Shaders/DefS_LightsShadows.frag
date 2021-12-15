#version 460 core
out vec4 FragColor;     

in vec2 TexCoords;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;

struct Light {
    vec3 Position;
    vec3 Color;
    
    float Linear;
    float Quadratic;
};

const int MAX_NUM_LIGHTS = 100;
uniform int u_NumLights;
uniform Light u_Lights[MAX_NUM_LIGHTS];
uniform vec3 u_CameraPos;

void main() {

  // retrieve data from gbuffer
    vec3 FragPos = texture(gPosition, TexCoords).rgb;
    vec3 Normal = texture(gNormal, TexCoords).rgb;
    vec3 Diffuse = texture(gAlbedoSpec, TexCoords).rgb;
    float Specular = texture(gAlbedoSpec, TexCoords).a;

     // blinn-phong
    vec3 lighting  = Diffuse * 0.1; // hard-coded ambient component
    vec3 viewDir  = normalize(u_CameraPos - FragPos);
    for(int i = 0; i < u_NumLights; ++i)
    {
        // calculate distance between light source and current fragment
         // diffuse
        vec3 lightDir = normalize(u_Lights[i].Position - FragPos);
        vec3 diffuse = max(dot(Normal, lightDir), 0.0) * Diffuse * u_Lights[i].Color;
        // specular
        vec3 halfwayDir = normalize(lightDir + viewDir);  
        float spec = pow(max(dot(Normal, halfwayDir), 0.0), 16.0);
        vec3 specular = u_Lights[i].Color * spec * Specular;
        // attenuation
        float dist = length(u_Lights[i].Position - FragPos);
        float attenuation = 1.0 / (1.0 + u_Lights[i].Linear * dist + u_Lights[i].Quadratic * dist * dist);
        diffuse *= attenuation;
        specular *= attenuation;
        lighting += diffuse + specular;        
    }
    FragColor = vec4(lighting, 1.0);
}