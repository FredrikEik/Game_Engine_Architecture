#pragma once
#include "CoreMinimal.h"
#include "glm/glm.hpp" // Every place that use vertex probably use glm anyways


class Vertex
{
public:
    Vertex(const float& x, const float& y, const float& z,
        const float& n1, const float& n2, const float& n3,
        const float& u = 0, const float& v = 0);
    Vertex::Vertex(const glm::vec3& position, const glm::vec3& normals, glm::vec2& UV);
private:
    float m_xyz[3];
    float m_normal[3];
    float m_uv[2];
};

inline Vertex::Vertex(const float& x, const float& y, const float& z,
    const float& n1, const float& n2, const float& n3,
    const float& u, const float& v)
    : m_xyz{ x, y, z }, m_normal{ n1, n2, n3 }, m_uv{ u, v }
{

}