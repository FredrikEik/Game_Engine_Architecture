#pragma once
#include <vector>
//#include "../Components/Components.h"
#include "glm/glm.hpp"
#include "../CoreMinimal.h"
#include <unordered_map>
struct SweepElement
{
	SweepElement(uint32 entity, bool generatesOverlap)
		: entity{ entity }, bShouldGenerateOverlapEvents{generatesOverlap}
	{}
	glm::vec2 min{};
	glm::vec2 max{};
	
	uint32 entity{core::MAX_ENTITIES};
	bool bShouldGenerateOverlapEvents;
};

class SweepAndPrune
{
public:
	SweepAndPrune(class ECSManager* inECS);
	~SweepAndPrune();

	void update();
	bool getOverlappedEntities(uint32 entityID, std::vector<uint32>& OUTVector);
	const std::vector<uint32>& getOverlappedEntities(uint32 entityID);

private:
	// NEVER USE THIS, only there for functor compare AABB
	SweepAndPrune();

	void insertCollisionPair(uint32 entityA, uint32 entityB);// TODO: REmove
	uint8 sortAxis{};
	std::vector<SweepElement> &m_data;
	class ECSManager* ECS{};
	// TODO: Only update entities that moved. Dirty bit maybe?
	bool updateData();
	void sortAndSweep();
	void updateSortAxis(const glm::vec2& sum, const glm::vec2& sum2);

	void clearAndFillData();

	std::unordered_map<uint32, std::vector<uint32>>& m_collisionPairs;

	//struct compareAABB
	//{
	//	compareAABB(const SweepAndPrune& inClassState)
	//		: classState{ inClassState } {}

	//	const SweepAndPrune& classState;
	//	bool operator()(const void* a, const void* b) 
	//	{
	//		// Sorting based on either x or y axis.
	//		float minA = ((SweepElement*)a)->min[classState.sortAxis];
	//		float minB = ((SweepElement*)b)->min[classState.sortAxis];

	//		return minA > minB;
	//	}
	//};
};