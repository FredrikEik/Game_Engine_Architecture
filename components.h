#ifndef COMPONENTS_H
#define COMPONENTS_H

#include "vector3d.h"
#include "matrix4x4.h"


class Components
{
public:
    Components();
    ~Components();
};

struct TransformComponent
{
    gsl::Matrix4x4 transform;
};

struct meshComponent
{
    unsigned int mVAO; // vertex array
    unsigned int mVBO; // vertex buffer
    unsigned int mIBO; // index buffer
};

struct textureComponent
{

};

#endif // COMPONENTS_H
