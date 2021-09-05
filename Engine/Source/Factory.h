#pragma once
#include "CoreMinimal.h"
#include "Components/ComponentManager.h"
#include "Components/BaseComponent.h"
#include <unordered_map>
#include <typeindex>

class Factory
{
public:
	//friend class ComponentManager;
	
	Factory(): componentManagers{(*new std::unordered_map<std::type_index, ComponentManager<Component>*>)}{}
	~Factory();

	template <typename T>
	uint32 createComponent(uint32 entityID);

	template <typename T>
	ComponentManager<T>& getManager(std::type_index classType);

private:
	std::unordered_map<std::type_index, ComponentManager<Component>*>& componentManagers;
};

Factory::~Factory()
{
	delete& componentManagers;
}

// This halves the speed of creating components. 
// It is plenty fast enough for creating a few components
// TODO: find a way to use the direct route in situations where performance is critical, IF NEEDED.
template<typename T>
inline uint32 Factory::createComponent(uint32 entityID)
{
	std::type_index classType = std::type_index(typeid(T));
	if (componentManagers.find(classType) == componentManagers.end())
	{
		ComponentManager<T> *manager = new ComponentManager<T>();
		uint32 componentID = manager->createComponent(entityID);
		componentManagers.insert(std::pair<std::type_index, ComponentManager<Component>*>
			(classType, (ComponentManager<Component>*)manager));
		return componentID;
	}
	ComponentManager<T>* manager = (ComponentManager<T>*)componentManagers.at(classType);
	return manager->createComponent(entityID);
}

template<typename T>
inline ComponentManager<T>& Factory::getManager(std::type_index classType)
{
	return (*(ComponentManager<T>*)componentManagers.at(classType));
}
