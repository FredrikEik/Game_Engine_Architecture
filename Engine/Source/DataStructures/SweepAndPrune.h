#pragma once
#include <vector>
//#include "../Components/Components.h"
#include "glm/glm.hpp"
#include "../CoreMinimal.h"
struct SweepElement
{
	SweepElement(uint32 entity)
		: entity{ entity }
	{}
	glm::vec2 min{};
	glm::vec2 max{};
	uint32 entity{core::MAX_ENTITIES};
};

class SweepAndPrune
{
public:
	SweepAndPrune(class ECSManager* inECS);
	~SweepAndPrune();

	void update();
private:
	// NEVER USE THIS, only there for functor compare AABB
	SweepAndPrune();

	void insert(uint32 entity);// TODO: REmove
	uint8 sortAxis{};
	std::vector<SweepElement> &m_data;
	class ECSManager* ECS{};
	// TODO: Only update entities that moved. Dirty bit maybe?
	bool updateData();
	void sortAndSweep();
	void updateSortAxis(const glm::vec2& sum, const glm::vec2& sum2);

	void clearAndFillData();
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