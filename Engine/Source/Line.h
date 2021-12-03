#pragma once
#include "CoreMinimal.h"
#include "glm/glm.hpp" // Every place that use vertex probably use glm anyways


class Line
{
public:
    Line(float startX, float startY, float startZ,
        float endX, float endY, float endZ);
    Line(const glm::vec3& startPos, const glm::vec3& endPos);

    float m_start_xyz[3];
    float m_end_xyz[3];
};
