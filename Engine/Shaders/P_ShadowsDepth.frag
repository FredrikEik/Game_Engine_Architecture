#version 460 core
in vec4 FragPos;

uniform vec3 u_lightPos;
uniform float u_far_plane;

void main()
{
    float lightDistance = length(FragPos.xyz - u_lightPos);
    
    // map to [0;1] range by dividing by far_plane
    lightDistance = lightDistance / u_far_plane;
    
    // write this as modified depth
    gl_FragDepth = lightDistance;
}