#version 460 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform sampler2D u_tex_diffuse1;
uniform sampler2D u_tex_specular1;
uniform samplerCube u_depthMap;

uniform int bUsingTexture = 0;
uniform int bUsingMat = 0;


uniform vec3 u_lightPos;
uniform vec3 u_camPos;

uniform float u_far_plane;
uniform bool u_shadows;

uniform vec3 u_lightColor = vec3(0.3);


// array of offset direction for sampling
vec3 gridSamplingDisk[20] = vec3[]
(
   vec3(1, 1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1, 1,  1), 
   vec3(1, 1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1, 1, -1),
   vec3(1, 1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1, 1,  0),
   vec3(1, 0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1, 0, -1),
   vec3(0, 1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0, 1, -1)
);

float ShadowCalculation(vec3 fragPos)
{
    vec3 fragToLight = fragPos - u_lightPos;
    float currentDepth = length(fragToLight);

    float shadow = 0.0;
    float bias = 0.15;
    int samples = 25;
    float viewDistance = length(u_camPos - fragPos);
    float diskRadius = (1 + (viewDistance / u_far_plane)) / 25.0;
    for(int i = 0; i < samples; ++i)
    {
        float closestDepth = texture(u_depthMap, fragToLight + gridSamplingDisk[i] * diskRadius).r;
        closestDepth *= u_far_plane;   // undo mapping [0;1]
        if(currentDepth - bias > closestDepth)
            shadow += 1.0;
    }
    shadow /= float(samples);
        
    // display closestDepth as debug (to visualize depth cubemap)
    // FragColor = vec4(vec3(closestDepth / far_plane), 1.0);    
        
    return shadow;
}

void main()
{           

    vec4 color = vec4(0.3,0.6,0.9,1);

    color.rgb = bUsingTexture * texture(u_tex_diffuse1, TexCoords).rgb +
    bUsingMat * texture(u_tex_diffuse1, TexCoords).rgb +
    (1 - bUsingTexture - bUsingMat) * vec4(0.3,0.6,0.9,1).rgb;

    color.a = bUsingMat * texture(u_tex_specular1, TexCoords).r + (1-bUsingMat);
    vec3 normal = normalize(Normal);
    // ambient
    vec3 ambient = 0.3 * u_lightColor;
    // diffuse
    vec3 lightDir = normalize(u_lightPos - FragPos);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * u_lightColor;
    // specular
    vec3 viewDir = normalize(u_camPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = 0.0;
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    spec = pow(max(dot(normal, halfwayDir), 0.0), 64.0);
    vec3 specular = spec * u_lightColor;    
    // calculate shadow
    float shadow = u_shadows ? ShadowCalculation(FragPos) : 0.0;                      
    vec4 lighting = vec4(ambient + (1.0 - shadow) * (diffuse + specular), 1) * color;    
    
    FragColor = lighting;
}