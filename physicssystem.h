#ifndef PHYSICSSYSTEM_H
#define PHYSICSSYSTEM_H
#include "components.h"

struct FloorData
{
    gsl::Vector3D CurrentTriangle[3]; // can delete it
    gsl::Vector3D floorNormal;
    gsl::Vector3D heightOfFloor;

};
class PhysicsSystem
{
public:
    PhysicsSystem();
    void InitPhysicsSystem(MeshComponent* surfaceData);
    void move(float deltaTime, TransformComponent* Transf, float radius);



private:
    FloorData Data;
    bool once = true;
    bool onTriangle = false;
    MeshComponent* mSurfaceData = new MeshComponent();
    gsl::Vector3D MakeGSLvec3D(QVector3D vec);
    void FindTriangle(TransformComponent* Transf);
    QVector3D CalcPlaneNormal(QVector3D p1,QVector3D p2,QVector3D p3);
    QVector3D Barysentric(QVector3D p1,QVector3D p2,QVector3D p3, QVector3D pos);
    QVector3D MakeQvec3D(gsl::Vector3D vec);
    QVector3D MirrorVector(QVector3D Vector, QVector3D normal);
    QVector3D DistanceTraveled(QVector3D Velocity, QVector3D Acceleration, float DT);
};

#endif // PHYSICSSYSTEM_H
