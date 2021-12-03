#include "Line.h"


Line::Line(float startX, float startY, float startZ, float endX, float endY, float endZ)
	: m_start_xyz{ startX, startY, startZ },
	m_end_xyz{ endX , endY, endZ }
{

}


Line::Line(const glm::vec3& startPos, const glm::vec3& endPos)
    : m_start_xyz{ startPos.x, startPos.y, startPos.z },
	m_end_xyz{ endPos.x, endPos.y, endPos.z }
{

}