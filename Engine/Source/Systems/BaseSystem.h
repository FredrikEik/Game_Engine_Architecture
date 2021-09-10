#pragma once
#include "../CoreMinimal.h"
//#include "../Components/ComponentManager.h"
class BaseSystem
{
	//BaseSystem();
};

class testComponent;
template<class T>
class ComponentManager;

class TestSystem final : public BaseSystem
{
public:
	TestSystem(){}

	void updateAll(class ComponentManager<class testComponent>& manager);
};    