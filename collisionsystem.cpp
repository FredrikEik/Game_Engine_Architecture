#include "collisionsystem.h"

CollisionSystem::CollisionSystem()
{

}

bool CollisionSystem::AABBtoAABB(const AABB &tBox1, const AABB &tBox2)
{
    return(tBox1.m_max.x > tBox2.m_min.x &&
    tBox1.m_min.x < tBox2.m_max.x &&
    tBox1.m_max.y > tBox2.m_min.y &&
    tBox1.m_min.y < tBox2.m_max.y &&
    tBox1.m_max.z > tBox2.m_min.z &&
           tBox1.m_min.z < tBox2.m_max.z);
}

