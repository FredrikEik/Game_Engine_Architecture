#ifndef COLLISIONSYSTEM_H
#define COLLISIONSYSTEM_H
#include <vector>
class Components;
class GameObject;

class CollisionSystem
{
public:
    CollisionSystem();

    static bool isColliding(std::vector<GameObject*> objects);
};

#endif // COLLISIONSYSTEM_H
