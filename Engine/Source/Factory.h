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

	// This is 2-3 times slower than creating components directly through the manager when creating many components.
	// If you need to create a lot of components, consider doing it through getManager directly.
	// When creating a few components, use this.
	template <typename T>
	uint32 createComponent(uint32 entityID);

	// This is 10 times slower than removing directly through the manager when removing many components.
	// If you need to delete a lot of components, consider doing it through getManager directly.
	// For one component, this is faster
	template <typename T>
	void removeComponent(uint32 entityID);

	template <typename T>
	ComponentManager<T>& getManager();

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
inline void Factory::removeComponent(uint32 entityID)
{
	std::type_index classType = std::type_index(typeid(T));
	assert(componentManagers.find(classType) != componentManagers.end());

	(*(ComponentManager<T>*)componentManagers.at(classType)).removeComponent(entityID);
}

template<typename T>
inline ComponentManager<T>& Factory::getManager()
{
	return (*(ComponentManager<T>*)componentManagers.at(std::type_index(typeid(T))));
}
