#include "collisionshapes.h"

CollisionShapes::CollisionShapes()
{

}

void CollisionShapes::createCollisionSphere(GameObject& obj)
{
    CollisionSphere* tempShape = new CollisionSphere;
    // Find the center point
    gsl::Vector3D tempVec;
    tempVec = createCentroid(obj);
    tempShape->m_cPoint.set_xyz(tempVec);
    // Find the radius from the center point
    float r = findRadian(obj, tempVec);
    tempShape->setRadius(r);
    // populate the shape into the vector
    m_cGeometricForm = tempShape;
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

//Find the radian from centroid to furthest coordinate
float CollisionShapes::findRadian(GameObject &obj, gsl::Vector3D centroid)
{
    std::vector<Vertex> points = obj.mMesh->mVertices;
    float r = 0;
    for(int i = 0; i<points.size(); i++)
    {
         gsl::Vector3D point = points.at(i).get_xyz();
         float n_r = sqrt(pow(centroid.getX() - point.getX(), 2)
                  + pow(centroid.getY() - point.getY(), 2)
                  + pow(centroid.getZ() - point.getZ(), 2));

         if(n_r > r) r = n_r;
    }
    return r;
}

// Dont really need it? Maybe for transforms?
CollisionSphere* CollisionShapes::createSphere(float fl_r, gsl::Vector3D vec_centroid)
{
    return nullptr;
}

CollisionShape* CollisionShapes::getGeometricForm()
{
    return m_cGeometricForm;
}



