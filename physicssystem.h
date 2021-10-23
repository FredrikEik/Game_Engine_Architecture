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
    void FindTriangle(TransformComponent* Transf, float collisionRadius);
    float distanceToPlane(TransformComponent *Transf);
    QVector3D Barysentric(QVector3D p1,QVector3D p2,QVector3D p3, QVector3D pos);


private:
    FloorData Data;
    bool once = true;
    MeshComponent* mSurfaceData = new MeshComponent();
    gsl::Vector3D MakeGSLvec3D(QVector3D vec);
    QVector3D MakeQvec3D(gsl::Vector3D vec);
};

#endif // PHYSICSSYSTEM_H
