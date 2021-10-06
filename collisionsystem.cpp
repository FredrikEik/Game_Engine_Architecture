#include "collisionsystem.h"

CollisionSystem::CollisionSystem()
{

}

bool CollisionSystem::collisionTestPvsS()
{
    // s = c - p; if abs(s) <= r .. return true;
}

bool CollisionSystem::collisionTestSvsS()
{
    // s = c1 - c2; if (abs(s)^2) <= (r1+r2)^2 .. return true;
}
