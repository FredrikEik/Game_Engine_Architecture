#ifndef PHYSICSSYSTEM_H
#define PHYSICSSYSTEM_H
#include "components.h"

/**
 * @brief The FloorData struct containing Data that is used to simulate physics.
 */
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
    /**
     * @brief PhysicsSystem Consturucter
     */
    PhysicsSystem();
    /**
     * @brief InitPhysicsSystem   - Here we set which surface we want to simulate physics.
     * @param surfaceData  - meshComponent of floor.
     * @param inVertexData  - Vertex data, this can be anything, in case you need to simulate lower poly floor.
     */
    void InitPhysicsSystem(MeshComponent* surfaceData, std::vector<Vertex> inVertexData);

    /**
     * @brief move  - Main physics function.
     * @param deltaTime  - Using delta time to calculate distances accuratley.
     * @param Transf - Transform component of the object that is colliding with surface.
     * @param radius  - Radius of the object thats simulated.
     */
    void move(float deltaTime, TransformComponent* Transf, float radius);



private:
    /**
     * @brief Data Struct member.
     */
    FloorData Data;
    /**
     * @brief once Used to remove extra bouncec - might be legacy.
     */
    bool once = true;
    /**
     * @brief onTriangle bool to check if you are on a triangle.
     */
    bool onTriangle = false;
    /**
     * @brief mSurfaceData Copy of the surface meshComponent.
     */
    MeshComponent* mSurfaceData = new MeshComponent();
    /**
     * @brief vertexData   copy of surface vertexData.
     */
    std::vector<Vertex> vertexData;

    /**
     * @brief FindTriangle Main barysentric function. here we get FloorData - if we find triangle.
     * @param Transf - Transformcomponent of object we want to check.
     */
    void FindTriangle(TransformComponent* Transf);

    /**
     * @brief CalcPlaneNormal Normal calculation.
     * @param p1 Vertex 1.
     * @param p2 Vertex 2.
     * @param p3 Vertex 3.
     * @return normalized normal vector.
     */
    QVector3D CalcPlaneNormal(QVector3D p1,QVector3D p2,QVector3D p3);

    /**
     * @brief Barysentric Barysentric calculation.
     * @param p1 Vertex 1.
     * @param p2 Vertex 2.
     * @param p3 Vertex 3.
     * @param pos position of physics object.
     * @return barysentric Vector , if numbers are positive and between 0 and 1, then we are on correct triangle.
     */
    QVector3D Barysentric(QVector3D p1,QVector3D p2,QVector3D p3, QVector3D pos);
    //!
    /**
     * @brief MakeGSLvec3D converter from QVector3D to gsl::Vector3D.
     */
    gsl::Vector3D MakeGSLvec3D(QVector3D vec);

    /** gsl::Vector3D misses some important functionality
     * @brief MakeQvec3D Converter from gsl::Vector3D to QVector3D.
     * @param vec
     * @return
     */
    QVector3D MakeQvec3D(gsl::Vector3D vec);

    /**
     * @brief MirrorVector mirror function.
     * @param Vector velocity vector of physics simulated object.
     * @param normal  is the surface normal.
     * @return vector mirrored around the surface normal.
     */
    QVector3D MirrorVector(QVector3D Vector, QVector3D normal);
    //! @brief DistanceTraveled
    //!@param Velocity
    //! @param Acceleration
    //! @param DT
    /**
     * @brief DistanceTraveled - physics calculation of distance based on time.
     * @param Velocity - velocity vector of object.
     * @param Acceleration - Acc Vector of object.
     * @param DT  - Delta time.
     * @return Distance traveleld - this gets added to translate function later. :D
     */
    QVector3D DistanceTraveled(QVector3D Velocity, QVector3D Acceleration, float DT);
};


#endif // PHYSICSSYSTEM_H
