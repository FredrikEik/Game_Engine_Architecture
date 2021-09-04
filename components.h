#ifndef COMPONENTS_H
#define COMPONENTS_H

#include "constants.h"
#include "matrix4x4.h"
#include "vertex.h"


class TransformComponents
{
public:
    TransformComponents();
    gsl:Matrix4x4 mMatrix;
};

class MeshComponent
{
public:
    std::vector<Vertex> mVertices;
    std::vector<
};

#endif // COMPONENTS_H
