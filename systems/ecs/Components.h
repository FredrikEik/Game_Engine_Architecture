#ifndef COMPONENTS_H
#define COMPONENTS_H


#include "constants.h"
#include "matrix4x4.h"
#include "vertex.h"

class Components{
public:
    Components() : mComponentType{gsl::NONE}{};
    gsl::Components mComponentType;

};

class TransformComponent : public Components{

public:

    TransformComponent() {mComponentType = gsl::TRANSFORM;};
    gsl::Matrix4x4 mMatrix;
};

class MeshComponent : public Components{

public:

    MeshComponent(){mComponentType = gsl::MESH;};

    std::vector<Vertex> mVertices;
    std::vector<GLuint> mIndices;

    GLuint mVAO{0};
    GLuint mVBO{0};
    GLuint mEAB{0}; //holds the indices (Element Array Buffer - EAB)

};

class MaterialCOmponent : public Components{

    MaterialCOmponent(){mComponentType = gsl::MATERIAL;};
};


#endif // COMPONENTS_H
