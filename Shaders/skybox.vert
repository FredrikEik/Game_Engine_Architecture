layout (location = 0) in vec3 Position;
layout (location = 1) in vec4 colAttr;
layout (location = 2) in vec3 vertexUV;

uniform mat4 mMatrix;
uniform mat4 vMatrix;
uniform mat4 pMatrix;
out vec3 UV;

void main()
{

    UV = Position;
    gl_Position = pMatrix * vMatrix * mMatrix * vec4(Position, 1.0);
}
