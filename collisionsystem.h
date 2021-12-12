#ifndef COLLISIONSYSTEM_H
#define COLLISIONSYSTEM_H
#include <vector>
#include "vertex.h"


class Components;
class GameObject;

class CollisionSystem
{
public:
    CollisionSystem();

    static bool isColliding(GameObject * player, std::vector<GameObject*> objects, QVector3D playerNextPos);
};

#endif // COLLISIONSYSTEM_H
