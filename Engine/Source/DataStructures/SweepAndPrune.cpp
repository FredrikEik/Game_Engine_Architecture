#include "SweepAndPrune.h"
//#include "glm/glm.hpp"
#include "../ECSManager.h"
#include "../Components/ComponentManager.h"
#include <algorithm>
#include "../Systems/CollisionSystem.h"
#include <functional>

SweepAndPrune::SweepAndPrune(ECSManager* inECS)
	: m_data((*new std::vector<SweepElement>{}))
{
	ECS = inECS;
}

SweepAndPrune::SweepAndPrune()
	: m_data((*new std::vector<SweepElement>{}))
{
}
SweepAndPrune::~SweepAndPrune()
{
	delete &m_data;
}


//void SweepAndPrune::insert(uint32 entity)
//{
//	//m_data->push_back(entity);
//	// Making sure the entities have collision
//	ComponentManager<AxisAlignedBoxComponent>* AABBManager = ECS->getComponentManager<AxisAlignedBoxComponent>();
//	ComponentManager<TransformComponent>* transformManager = ECS->getComponentManager<TransformComponent>();
//
//	AxisAlignedBoxComponent* AABBComponent = AABBManager->getComponentChecked(entity);
//	TransformComponent* transformComponent = transformManager->getComponentChecked(entity);
//	assert(AABBComponent);
//	assert(transformComponent);
//
//	// Inserting the AABBs in absolute world positions.
//	//m_data.push_back(SweepElement(
//	//	entity));
//
//}
//
//void SweepAndPrune::update()
//{
//	updateData();
//	sortAndSweep();
//}
//
void SweepAndPrune::sortAndSweep()
{

	// Sort
	uint8 tempSortAxis = sortAxis;
	auto compare = [tempSortAxis](const SweepElement& a, const SweepElement& b) -> bool
	{
		// Sorting based on either x or y axis.
		float minA = (a).min[tempSortAxis];
		float minB = (b).min[tempSortAxis];

		return minA > minB;
	};
	//std::sort(m_data.begin(), m_data.end(), compareAABB(*this));
	std::sort(m_data.begin(), m_data.end(), compare);

	// Sweep
	glm::vec2 sum{};
	glm::vec2 sum2{};

	for (uint32 i{}; i < m_data.size(); ++i)
	{
		glm::vec2 center = 0.5f * (m_data.at(i).min + m_data.at(i).max);

		// To compute variance of centers
		sum += center;
		sum2 += center * center;

		for (uint32 j{ i + 1 }; j < m_data.size(); ++j)
		{
			// If the tested collision cannot collide with the current, break
			if (m_data.at(j).min[sortAxis] > m_data.at(i).max[sortAxis])
				break;
			// Does the actual collision test.
			CollisionSystem::testCollision(m_data.at(i).entity, m_data.at(j).entity, ECS);
		}
	}
	updateSortAxis(sum, sum2);
}

void SweepAndPrune::updateSortAxis(const glm::vec2& sum, const glm::vec2& sum2)
{
	glm::vec2 variance{sum2 - sum * sum / (float)m_data.size()};

	// Set the sort axis to be the axis with most variance
	sortAxis = 0;
	if (variance.y > variance.x)
		sortAxis = 1;
}

void SweepAndPrune::clearAndFillData()
{
	// Clear data
	m_data.clear();

	// Fill data
	ComponentManager<AxisAlignedBoxComponent>* AABBManager = ECS->getComponentManager<AxisAlignedBoxComponent>();
	const auto& componentArray = AABBManager->getComponentArray();
	m_data.reserve(componentArray.size());

	for (const auto& it : componentArray)
	{
		m_data.push_back(SweepElement(it.entityID));
	}
}


void SweepAndPrune::updateData()
{
	ComponentManager<AxisAlignedBoxComponent>* AABBManager = ECS->getComponentManager<AxisAlignedBoxComponent>();
	ComponentManager<TransformComponent>* transformManager = ECS->getComponentManager<TransformComponent>();

	// TODO: There is an edgecase where this doesn't work. If a AABB component is removed, 
	// then a new one added in the same frame, this breaks. 
	// For now it is unlikely, so for performance reasons we do not care about this edgecase
	// due to the engine being made for units that are indirectly controlled and thus 
	// precise collision is not *that* important.
	if (AABBManager->getComponentArray().size() != m_data.size())
		clearAndFillData();

	for (auto& it : m_data)
	{
		TransformComponent transform{ transformManager->getComponent(it.entity) };
		AxisAlignedBoxComponent AABB{ AABBManager->getComponent(it.entity) };

		it.min = glm::vec2(AABB.minScaled.x + transform.transform[3].x,
			AABB.minScaled.z + transform.transform[3].z);
		it.max = glm::vec2(AABB.maxScaled.x + transform.transform[3].x,
			AABB.maxScaled.z + transform.transform[3].z);
	}

	//AxisAlignedBoxComponent* AABBComponent = AABBManager->getComponentChecked(entity);
	//TransformComponent* transformComponent = transformManager->getComponentChecked(entity);
}

