#include "SweepAndPrune.h"
//#include "glm/glm.hpp"
#include "../ECSManager.h"
#include "../Components/ComponentManager.h"
#include <algorithm>
#include "../Systems/CollisionSystem.h"
#include <functional>

SweepAndPrune::SweepAndPrune(ECSManager* inECS)
	: m_data((*new std::vector<SweepElement>{})),
	m_collisionPairs((*new std::unordered_map<uint32, std::vector<uint32>>()))
{
	ECS = inECS;
}

SweepAndPrune::SweepAndPrune()
	: m_data((*new std::vector<SweepElement>{})),
	m_collisionPairs((*new std::unordered_map<uint32, std::vector<uint32>>()))
{
}

SweepAndPrune::~SweepAndPrune()
{
	delete &m_data;
	delete& m_collisionPairs;
}


void SweepAndPrune::update()
{
	if(updateData())
		sortAndSweep();
}

bool SweepAndPrune::getOverlappedEntities(uint32 entityID, std::vector<uint32>& OUTVector)
{
	if (m_collisionPairs.find(entityID) != m_collisionPairs.end())
	{
		auto& temp = m_collisionPairs.at(entityID);
		std::copy(temp.begin(), temp.end(), OUTVector.begin());
		return true;
	}

	return false;
}

const std::vector<uint32>& SweepAndPrune::getOverlappedEntities(uint32 entityID)
{
	std::cout << "Overlapped called\n";

	if (m_collisionPairs.find(entityID) != m_collisionPairs.end())
	{
		std::cout << "Overlapped entities\n";
		return m_collisionPairs.at(entityID);
	}
	return std::vector<uint32>();
}

void SweepAndPrune::sortAndSweep()
{
	// Sort
	uint8 tempSortAxis = sortAxis;
	auto compare = [tempSortAxis](const SweepElement& a, const SweepElement& b) -> bool
	{
		// Sorting based on either x or y axis.
		float minA = (a).min[tempSortAxis];
		float minB = (b).min[tempSortAxis];

		// if a should generate overlap while b shouldn't, a is pushed in front
		// Seems to work so far, needs more testing
		// If collision issues arise, remove everything after the OR
		return (minA > minB) || (a.bShouldGenerateOverlapEvents > b.bShouldGenerateOverlapEvents);
	};
	//std::sort(m_data.begin(), m_data.end(), compareAABB(*this));
	std::sort(m_data.begin(), m_data.end(), compare);

	m_collisionPairs.clear(); // clears the collision array every frame
	// Sweep

	glm::vec2 sum{};
	glm::vec2 sum2{};

	for (uint32 i{}; i < m_data.size(); ++i)
	{
		if (!m_data.at(i).bShouldGenerateOverlapEvents)
			continue;
		glm::vec2 center = 0.5f * (m_data.at(i).min + m_data.at(i).max);

		// To compute variance of centers
		sum += center;
		sum2 += center * center;

		int32 CollisionsTested{};
		for (uint32 j{ i + 1 }; j < m_data.size(); ++j)
		{

			// If the tested collision can collide on one axis collide with the current collision, test collision.
			// If not, break
			if ((m_data.at(j).max[sortAxis]) >=
				(m_data.at(i).min[sortAxis]) &&
				(m_data.at(i).max[sortAxis]) >=
				(m_data.at(j).min[sortAxis]))
				// Does the actual collision test.
				if (CollisionSystem::testCollision(m_data.at(i).entity, m_data.at(j).entity, ECS))
					insertCollisionPair(m_data.at(i).entity, m_data.at(j).entity);
			else
				break;

			++CollisionsTested;

		}
		//std::cout << "tested " << CollisionsTested << " out of " << m_data.size() << " collisions before breaking\n";
#ifdef DEBUG
#ifdef DEBUG_BROADPHASE

#endif // DEBUG_BROADPHASE


#endif // DEBUG

		//_sleep(250);
	}
	//std::cout << "Finished testing collisions\n";

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
		m_data.push_back(SweepElement(it.entityID, it.bShouldGenerateOverlapEvents));
		std::cout << "should generate: " << it.bShouldGenerateOverlapEvents << '\n';
	}
}


bool SweepAndPrune::updateData()
{
	ComponentManager<AxisAlignedBoxComponent>* AABBManager = ECS->getComponentManager<AxisAlignedBoxComponent>();
	ComponentManager<TransformComponent>* transformManager = ECS->getComponentManager<TransformComponent>();
	if (!AABBManager || !transformManager)
		return false;
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
	return true;
	//AxisAlignedBoxComponent* AABBComponent = AABBManager->getComponentChecked(entity);
	//TransformComponent* transformComponent = transformManager->getComponentChecked(entity);
}

void SweepAndPrune::insertCollisionPair(uint32 entityA, uint32 entityB)
{
	if (m_collisionPairs.find(entityA) == m_collisionPairs.end())
	{
		m_collisionPairs.insert(std::pair<uint32, std::vector<uint32>>(entityA, std::vector<uint32>()));
		//collisionPair = m_collisionPairs.find(entityA);
	}
	std::vector<uint32>& collisionPair = m_collisionPairs.at(entityA);
	//assert(collisionPair != m_collisionPairs.end());
	collisionPair.push_back(entityB);
}