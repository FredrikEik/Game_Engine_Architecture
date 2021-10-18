#include "collisionsystem.h"

CollisionSystem::CollisionSystem()
{

}

bool CollisionSystem::collisionTestPvsS()
{
    // s = c - p; if abs(s) <= r .. return true;
    return true;
}

bool CollisionSystem::collisionTestSvsS()
{
    CollisionShapes* temp1;
    CollisionShapes* temp2;
    CollisionShape* temp3;
    CollisionShape* temp4;
    gsl::Vector3D c1;
    gsl::Vector3D c2;
    float r1;
    float r2;
    // s = c1 - c2; if (abs(s)^2) <= (r1+r2)^2 .. return true;
    for(int i = 0; i < m_Collidables.size(); i++)
    {
        temp1 = m_Collidables.at(i);
        temp3 = temp1->getGeometricForm();
        c1 = temp3->getPoint().get_xyz();
        r1 = temp3->getRadius();
        for(int j = 0; j<m_Collidables.size(); j++)
        {
            temp2 = m_Collidables.at(j);
            temp4 = temp2->getGeometricForm();
            c2 = temp4->getPoint().get_xyz();
            r2 = temp4->getRadius();
            auto s1 = c1.operator-(c2);
            auto s2 = c2.operator-(c1);
            auto s1_len = s1.length();
            auto s2_len = s2.length();
            if (s1_len <= r1 || s2_len <= r2)
            {
                return true; // Collition
            }
            else return false; // No collition
        }
    }
    return false;
}
