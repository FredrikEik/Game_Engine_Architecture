#include "MathStuff/MathStuff.h"


tog::vec4 tog::Transform(gsl::Matrix4x4 matrix, vec4 vector)
{
    vec4 returnVector = vec4(0.f, 0.f, 0.f, 0.f);
    returnVector.x = (vector.x * matrix.getFloat(0)) + (vector.y * matrix.getFloat(1)) + (vector.z * matrix.getFloat(2)) + (vector.w * matrix.getFloat(3));
    returnVector.y = (vector.x * matrix.getFloat(4)) + (vector.y * matrix.getFloat(5)) + (vector.z * matrix.getFloat(6)) + (vector.w * matrix.getFloat(7));
    returnVector.z = (vector.x * matrix.getFloat(8)) + (vector.y * matrix.getFloat(9)) + (vector.z * matrix.getFloat(10)) + (vector.w * matrix.getFloat(11));
    returnVector.w = (vector.x * matrix.getFloat(12)) + (vector.y * matrix.getFloat(13)) + (vector.z * matrix.getFloat(14)) + (vector.w * matrix.getFloat(15));
    return returnVector;
}

float tog::dotProduct(gsl::Vector3D a, gsl::Vector3D b)
{
    float ans = (a.getX() * b.getX()) + (a.getY() * b.getY()) + (a.getZ() * b.getZ());
    return ans;
}

float tog::min(float x, float y)
{
    if (x < y)
        return x;
    else
        return y;
}

float tog::max(float x, float y)
{
    if (x > y)
        return x;
    else
        return y;
}

float tog::distanceVec3D(gsl::Vector3D a, gsl::Vector3D b)
{
    gsl::Vector3D ab = b - a;
    return ab.length();
}
