#pragma once
#include "../CoreMinimal.h"

class BaseSystem
{
	
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