#include "MathStuff/MathStuff.h"


float tog::dotProduct(gsl::Vector3D a, gsl::Vector3D b)
{
    float ans = (a.getX() * b.getX()) + (a.getY() * b.getY()) + (a.getZ() * b.getZ());
    return ans;
}
