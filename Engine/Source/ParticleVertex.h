#pragma once
#include "CoreMinimal.h"
//#include "glm/glm.hpp" // Every place that use vertex probably use glm anyways

class ParticleVertex 
{
public:
    ParticleVertex(const float& x, const float& y, const float& z,
        const float& n1, const float& n2, const float& n3,
        const float& u = 0, const float& v = 0);
    ParticleVertex(const glm::vec3& vertexPosition, const glm::vec3& normals, glm::vec2& UV);
//private:
    glm::vec3 getPosition() const 
        { return glm::vec3(m_particleCenter[0], m_particleCenter[1], m_particleCenter[2]); }

    float m_vertexXYZ[3];
    float m_particleCenter[3];
    float m_particleColor[3];
    float m_uv[2];
};