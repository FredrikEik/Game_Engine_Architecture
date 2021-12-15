#ifndef COLLISIONSYSTEM_H
#define COLLISIONSYSTEM_H
#include "components.h"

/**
 * @brief The collisionSystem class
 */
class collisionSystem
{
public:
    /**
     * @brief collisionSystem - constructer
     */
    collisionSystem();

    /**
     * @brief isColliding - Checks if two spheres-colliders are colliding with eachother
     * @param Mesh1 - object 1 mesh component
     * @param transform1 - object 1 transform component
     * @param Mesh2 - object 2 mesh component
     * @param transform2 - object 2 transform component
     * @return If true, then they are overlapping and colliding, false == no collision
     */
    bool isColliding(MeshComponent *Mesh1,TransformComponent* transform1, MeshComponent* Mesh2, TransformComponent* transform2);
};

#endif // COLLISIONSYSTEM_H
