#include "TrailSystem.h"
#include "../ECSManager.h"
#include "MeshSystem.h"
#include <chrono>
void TrailSystem::generateBSplines(uint32 trailEntity, ECSManager* ECS, int orderOfCurve, bool bDrawControlPoints, 
	float stepDistance)
{
	const auto& trailEntities = ECS->getComponentManager<TrailComponent>()->getComponentArray();
	MeshComponent* mesh = ECS->getComponentManager<MeshComponent>()->getComponentChecked(trailEntity);
	assert(mesh);
	mesh->m_vertices.clear();
	mesh->m_indices.clear();
	for (auto& it : trailEntities)
	{
		for (auto& position : it.positions)
		{
			generateBSpline(mesh, position, ECS, orderOfCurve, bDrawControlPoints, stepDistance);
		}
	}
	mesh->m_drawType = GL_LINES;
	MeshSystem::initialize(*mesh);
	mesh->bDisregardedDuringFrustumCulling = true;
}

void TrailSystem::generateBSpline(MeshComponent* trailMesh, const std::vector<glm::vec3>& recordedPositions, 
	ECSManager* ECS, int orderOfCurve, bool bDrawControlPoints, float stepDistance)
{
#pragma region ConvenienceLambdas
	auto nearlyEqual = [](float a, float b) -> bool
	{
		return (std::fabs(a - b) < 1e-5); // check if correct
	};

	// Non uniform b-spline curves has a fixed interval where the value of t_0 and t_n+d repeats d times.
	// d = orderOfCurve while n = numberOfControlPoints
	auto generateKnotVector = [orderOfCurve]( int numberOfControlPoints) -> std::vector<int>
	{
		std::vector<int> knotVector;

		for (int i{}; i < orderOfCurve - 1; ++i)
			knotVector.push_back(0);

		for (int i{}; i < orderOfCurve + numberOfControlPoints - 2 * orderOfCurve + 2; ++i)
			knotVector.push_back(i);

		for (int i{}; i < orderOfCurve - 1; ++i)
			knotVector.push_back(knotVector.back());

		return knotVector;
	};
	//ConvenienceLambdas
#pragma endregion
	
#pragma region GenerateBSpline
	std::vector<int> knotVector = generateKnotVector(recordedPositions.size());
	
	int range[2] = { knotVector[0], knotVector.back() };
	std::vector<glm::vec3> positions;

	for (float positionOnCurve = range[0]; positionOnCurve < (float)range[1]-0.0001; positionOnCurve += stepDistance)
	{
		glm::vec4 u(positionOnCurve * positionOnCurve * positionOnCurve, positionOnCurve * positionOnCurve, positionOnCurve,1);
		glm::vec3 point{};

		int num = (int)(positionOnCurve - (float)range[0]);
		if (nearlyEqual(positionOnCurve, range[1]))
			--num;
		for (int i{ num }; i < num + orderOfCurve; ++i)
		{
			// De boor's recursive algorithm happens here
			glm::vec4 coefficient = getKnotCoefficient(i, orderOfCurve, positionOnCurve, knotVector);

			point += glm::dot(u, coefficient) * recordedPositions[i];
		}
		positions.push_back(point);
		trailMesh->m_vertices.push_back(Vertex(point, glm::vec3(0, 5, 0), glm::vec2()));
	}

	int index = trailMesh->m_indices.size() > 0 ? trailMesh->m_indices.back()+1 : 0;
	bool bHadIndexes = index > 0;

	for (int i{ index }; i < trailMesh->m_vertices.size()-2; ++i)
	{
		trailMesh->m_indices.push_back(i + bHadIndexes);
		trailMesh->m_indices.push_back(i + 1 + bHadIndexes);
	}
	//GenerateBSpline
#pragma endregion

#pragma region DrawControlPoints
	if (bDrawControlPoints)
	{
		index = trailMesh->m_indices.back()+2;
		for (int i{}; i < recordedPositions.size(); ++i)
		{
			trailMesh->m_vertices.push_back(Vertex(recordedPositions[i], glm::vec3(5, 10, 5), glm::vec2()));
			if (i < recordedPositions.size() - 1)
			{
				trailMesh->m_indices.push_back(index);
				++index;
				trailMesh->m_indices.push_back(index);
			}
		}
	}
	//DrawControlPoints
#pragma endregion
}

glm::vec4 TrailSystem::getKnotCoefficient(int index, int orderOfCurve, float positionOnCurve, const std::vector<int>& knotVector)
{
	if (orderOfCurve == 1)
	{
		if ((positionOnCurve >= knotVector[index] && positionOnCurve < knotVector[index + 1]) || fabs(knotVector.back() - positionOnCurve) < 1e-8)
			return glm::vec4(0, 0, 0, 1);
		else
			return glm::vec4(0,0,0,0);
	}
	else
	{
		float length1 = knotVector[index + orderOfCurve - 1] - knotVector[index];
		float length2 = knotVector[index + orderOfCurve] - knotVector[index + 1];
		if (length1 == 0.f)
			length1 = 1.0;
		if (length2 == 0.f)
			length2 = 1.0;
		glm::vec4 tempCoefficient1, tempCoefficient2;

		tempCoefficient1.x = -getKnotCoefficient(index, orderOfCurve - 1, positionOnCurve, knotVector).x * knotVector[index] / length1 + 
			getKnotCoefficient(index, orderOfCurve - 1, positionOnCurve, knotVector).y / length1;
		tempCoefficient1.y = -getKnotCoefficient(index, orderOfCurve - 1, positionOnCurve, knotVector).y * knotVector[index] / length1 + 
			getKnotCoefficient(index, orderOfCurve - 1, positionOnCurve, knotVector).z / length1;
		tempCoefficient1.z = -getKnotCoefficient(index, orderOfCurve - 1, positionOnCurve, knotVector).z * knotVector[index] / length1 + 
			getKnotCoefficient(index, orderOfCurve - 1, positionOnCurve, knotVector).w / length1;
		tempCoefficient1.w = -getKnotCoefficient(index, orderOfCurve - 1, positionOnCurve, knotVector).w * knotVector[index] / length1;

		tempCoefficient2.x = getKnotCoefficient(index + 1, orderOfCurve - 1, positionOnCurve, knotVector).x * knotVector[index + orderOfCurve] / length2 - 
			getKnotCoefficient(index + 1, orderOfCurve - 1, positionOnCurve, knotVector).y / length2;
		tempCoefficient2.y = getKnotCoefficient(index + 1, orderOfCurve - 1, positionOnCurve, knotVector).y * knotVector[index + orderOfCurve] / length2 - 
			getKnotCoefficient(index + 1, orderOfCurve - 1, positionOnCurve, knotVector).z / length2;
		tempCoefficient2.z = getKnotCoefficient(index + 1, orderOfCurve - 1, positionOnCurve, knotVector).z * knotVector[index + orderOfCurve] / length2 - 
			getKnotCoefficient(index + 1, orderOfCurve - 1, positionOnCurve, knotVector).w / length2;
		tempCoefficient2.w = getKnotCoefficient(index + 1, orderOfCurve - 1, positionOnCurve, knotVector).w * knotVector[index + orderOfCurve] / length2;

		return tempCoefficient1 + tempCoefficient2;
	}
}

void TrailSystem::recordPositions(ECSManager* ECS, float currentTime)
{
	auto& trails = ECS->getComponentManager<TrailComponent>()->getComponentArray();
	auto transformManager = ECS->getComponentManager<TransformComponent>();

	for (auto& it : trails)
	{
		if ((currentTime - it.lastRecordedTime) > it.recordInterval && it.bRecording)
		{
			TransformComponent* transform = transformManager->getComponentChecked(it.entityID);
			assert(transform);
			it.positions.back().push_back(glm::vec3(transform->transform[3]));
			it.lastRecordedTime = currentTime;
		}
	}
}

void TrailSystem::toggleRecording(uint32 entityID, ECSManager* ECS)
{
	TrailComponent* trail = ECS->getComponentManager<TrailComponent>()->getComponentChecked(entityID);
	assert(trail);
	if (!trail->bRecording)
	{
		trail->bRecording = true;
		trail->lastRecordedTime = 0.f;
		trail->positions.push_back(std::vector<glm::vec3>());
	}
	else
	{
		trail->bRecording = false;
	}
}

void TrailSystem::toggleRecordings(ECSManager* ECS)
{
	auto& trails = ECS->getComponentManager<TrailComponent>()->getComponentArray();
	for (auto& it : trails)
		toggleRecording(it.entityID, ECS);
}

void TrailSystem::setRecordInterval(uint32 entityID, ECSManager* ECS, float recordInterval)
{
	TrailComponent* trail = ECS->getComponentManager<TrailComponent>()->getComponentChecked(entityID);
	assert(trail);
	trail->recordInterval = recordInterval;
}
