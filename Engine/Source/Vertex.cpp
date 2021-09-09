#include "Vertex.h"

Vertex::Vertex(const glm::vec3& position, const glm::vec3& normals, glm::vec2& UV)
    : m_xyz{ position.x, position.y, position.z },
    m_normal{ normals.x, normals.y, normals.z },
    m_uv{ UV.x, UV.y }
{

}