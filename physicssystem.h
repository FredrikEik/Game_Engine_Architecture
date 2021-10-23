#ifndef PHYSICSSYSTEM_H
#define PHYSICSSYSTEM_H
#include "components.h"

struct FloorData
{
    gsl::Vector3D CurrentTriangle[3];
    gsl::Vector3D floorNormal;

};
class PhysicsSystem
{
public:
    PhysicsSystem();
    void InitPhysicsSystem(MeshComponent* surfaceData);
    void freeFall(float deltaTime, TransformComponent* Transf, float radius);
    void bounce_floor(float deltaTime, TransformComponent* Transf, float radius);
    void FindTriangle(TransformComponent* Transf);
    float distanceToPlane(TransformComponent *Transf);

private:
    FloorData Data;
    bool once = true;
    MeshComponent* mSurfaceData = new MeshComponent();
};

#endif // PHYSICSSYSTEM_H
