#ifndef COMPONENTS_H
#define COMPONENTS_H

#include "vector3d.h"
#include "matrix4x4.h"


class Components
{
public:
    Components();
};

//class TransformComponent : public Components
struct TransformComponent
{

    gsl::Matrix4x4 mMatrix();


/**public:
    TransformComponent();
    ~TransformComponent();

    float GetX() {return x;}
    float GetY() {return y;}
    float GetZ() {return z;}

    void SetX(float nx) {x = nx;}
    void SetY(float ny) {y = ny;}
    void SetZ(float nz) {z = nz;}

    float GetPitch() {return pitch;}
    float GetYaw() {return yaw;}
    float GetRoll() {return roll;}

    void SetPitch(float nPitch) {pitch = nPitch;}
    void SetYaw(float nYaw) {yaw = nYaw;}
    void SetRoll(float nRoll) {roll = nRoll;}*/
};

//class MeshComponent : public Components
struct MeshComponent
{
    unsigned int mVAO; //vertex array
    unsigned int mVBO; //vertex buffer
    unsigned int mIBO; //Index buffer

    //MeshComponent();
    //~MeshComponent();
};

#endif // COMPONENTS_H
