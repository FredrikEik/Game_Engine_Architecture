#include "collisionshapes.h"

CollisionShapes::CollisionShapes()
{

}

void CollisionShapes::createCollisionSphere(GameObject& obj)
{
    CollisionSphere tempShape;
    // Find the center point
    gsl::Vector3D tempVec;
    tempVec = createCentroid(obj);
    tempShape.m_cPoint.set_xyz(tempVec);
    // Find the radius from the center point

    // populate the shape into the vector
    m_cGeometricForm.push_back(tempShape);
}

gsl::Vector3D CollisionShapes::createCentroid(GameObject &obj)
{
    std::vector<Vertex> vert = obj.mMesh->mVertices;
    float x = 0, y = 0, z = 0;
    for(int i = 0; i < vert.size(); i++)
    {
        gsl::Vector3D temp = vert.at(i).get_xyz();
        x += temp.getX();
        y += temp.getY();
        z += temp.getZ();
    }
    float sx = 0, sy = 0, sz = 0;
    sx = x/vert.size();
    sy = y/vert.size();
    sz = z/vert.size();
    gsl::Vector3D tempVec;
    tempVec.setX(sx);
    tempVec.setY(sy);
    tempVec.setZ(sz);
    return tempVec;
}
