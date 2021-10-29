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

    vec4 (gsl::Matrix4x4 matrix, vec4 vector)
    {
        x = y = z = w = 0;
        for (int i = 0; i < 4; i++ ) {
            x += matrix.getFloat(i);
        }
        for (int i = 0; i < 4; i++ ) {
            y += matrix.getFloat(i+4);
        }
        for (int i = 0; i < 4; i++ ) {
            z += matrix.getFloat(i+8);
        }
        for (int i = 0; i < 4; i++ ) {
            w += matrix.getFloat(i+12);
        }

    }
};

float dotProduct(gsl::Vector3D a, gsl::Vector3D b);

}


#endif // MATHSTUFF_H
