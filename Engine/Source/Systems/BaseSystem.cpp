#include "BaseSystem.h"
#include "../Components/ComponentManager.h"
#include "../Components/Components.h"

void TestSystem::updateAll(ComponentManager<class testComponent>& manager)
{
	auto& array = manager.getComponentArray();
	for (auto& it : array)
	{
		it.pos[0] = 13;
		it.pos[1] = 14;
		it.pos[2] = 15;
	}
}
