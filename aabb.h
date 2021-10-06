#ifndef AABB_H
#define AABB_H

#include "vertex.h"
#include "gameobject.h"
#include <vector>

class AABB
{
public:
    AABB(GameObject* object);
    void intersect();
    void createAABB();

private:
    std::vector<Vertex> m_vertices;
};

#endif // AABB_H
