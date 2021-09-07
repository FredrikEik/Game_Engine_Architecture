#ifndef COMPONENTS_H
#define COMPONENTS_H


#include "constants.h"
#include "matrix4x4.h"
#include "vertex.h"

//namespace ecs{

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
class materialId
{
    MAX_ENTITIES_TYPE mEntityID{MAX_ENTITIES};
    unsigned short mMaterialIndex{0};        //Material 0 = default material

};

class Collider : public Components
{
    gsl::Colliders mType{gsl::NOCOLLIDER};

};


#endif // COMPONENTS_H
