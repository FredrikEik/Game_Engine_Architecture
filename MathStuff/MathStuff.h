#ifndef MATHSTUFF_H
#define MATHSTUFF_H

#include <cmath>
#include "gsl/matrix4x4.h"


namespace tog
{

struct vec4
{
    float x, y, z, w;
    vec4(float inX, float inY, float inZ, float inW)
    {
        x = inX;
        y = inY;
        z = inZ;
        w = inW;
    }
};

vec4 Transform(gsl::Matrix4x4 matrix, vec4 vector);

float dotProduct(gsl::Vector3D a, gsl::Vector3D b);

float min(float x, float y);
float max(float x, float y);

float distanceVec3D(gsl::Vector3D a, gsl::Vector3D b);

}


#endif // MATHSTUFF_H
