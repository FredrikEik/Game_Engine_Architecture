#version 460 core

// Ouput data
out vec4 color;

// Values that stay constant for the whole mesh.
uniform vec4 u_pickingColor;

void main(){

    color = u_pickingColor;

}