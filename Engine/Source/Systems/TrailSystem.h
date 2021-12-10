#pragma once
#include "BaseSystem.h"
#include "glm/glm.hpp"
#include <vector>

class TrailSystem : public BaseSystem
{
public:
	static void generateBSplines(uint32 trailEntity,class ECSManager* ECS, 
		int orderOfCurve = 4, bool bDrawControlPoints = false, float resolution = 0.01);

	static void generateBSpline(class MeshComponent* trailMesh, 
		const std::vector<glm::vec3> & recordedPositions,
		class ECSManager* ECS, int orderOfCurve = 4, bool bDrawControlPoints = false, 
		float resolution = 0.01);

	static glm::vec4 getKnotCoefficient(int index, int orderOfCurve,
		float positionOnCurve, const std::vector<int>& controlPoints);

	static void recordPositions(class ECSManager* ECS, float currentTime);
	static void toggleRecording(uint32 entityID, class ECSManager* ECS);
	static void toggleRecordings(class ECSManager* ECS);

	static void setRecordInterval(uint32 entityID, class ECSManager* ECS, float recordInterval);
};

