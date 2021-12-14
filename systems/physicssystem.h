#ifndef PHYSICSSYSTEM_H
#define PHYSICSSYSTEM_H
#include "components.h"
//! Struct containing Data that is used to simulate physics
struct FloorData
{

    gsl::Vector3D CurrentTriangle[3]; //! Contains 3 vertexes that make up the current triangle, used in normals
    gsl::Vector3D floorNormal; //!Calculated normal vector
    gsl::Vector3D heightOfFloor; //! height of floor for the current triangle

};
class PhysicsSystem
{
public:
    //! empty constructer
    PhysicsSystem();
    //!
    //! \brief InitPhysicsSystem  - Here we set which surface we want to simulate physics
    //! \param surfaceData - meshComponent of floor
    //! \param inVertexData - Vertex data, this can be anything, in case you need to simulate lower poly floor
    //!
    void InitPhysicsSystem(MeshComponent* surfaceData, std::vector<Vertex> inVertexData);
    //!
    //! \brief move - Main physics function
    //! \param deltaTime - Using delta time to calculate distances accuratley
    //! \param Transf - Transform component of the object that is colliding with surface
    //! \param radius - Radius of the object thats simulated
    //!
    void move(float deltaTime, TransformComponent* Transf, float radius);



private:
    //! Struct member
    FloorData Data;
    //! Used to remove extra bouncec - might be legacy
    bool once = true;
    //! bool to check if you are on a triangle
    bool onTriangle = false;
    //! Copy of the surface meshComponent
    MeshComponent* mSurfaceData = new MeshComponent();
    //! copy of surface vertexData
    std::vector<Vertex> vertexData;
    //! Main barysentric function
    //! here we get FloorData - if we find triangle
    void FindTriangle(TransformComponent* Transf);
    //! Normal calculation
    //! @return normalized normal vector
    QVector3D CalcPlaneNormal(QVector3D p1,QVector3D p2,QVector3D p3);
    //! Barysentric calculation
    //! @return barysentric Vector , if numbers are positive and between 0 and 1, then we are on correct triangle
    QVector3D Barysentric(QVector3D p1,QVector3D p2,QVector3D p3, QVector3D pos);
    //! converter from QVector3D to gsl::Vector3D
    gsl::Vector3D MakeGSLvec3D(QVector3D vec);
    //! Converter from gsl::Vector3D to QVector3D
    //! gsl::Vector3D misses some important functionality
    QVector3D MakeQvec3D(gsl::Vector3D vec);
    //! mirror function
    //! @param normal is the surface normal
    //! @param Vector is velocity vector of physics simulated object
    //! @return Velocity vector mirrored around the surface normal
    QVector3D MirrorVector(QVector3D Vector, QVector3D normal);
    //! @brief DistanceTraveled - physics calculation of distance based on time
    //!@param Velocity - velocity vector of object
    //! @param Acceleration - Acc Vector of object
    //! @param DT - Delta time
    QVector3D DistanceTraveled(QVector3D Velocity, QVector3D Acceleration, float DT);
};


#endif // PHYSICSSYSTEM_H
