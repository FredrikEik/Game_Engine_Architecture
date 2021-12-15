#include "Load.h"
#include <fstream>
#include "../Components/Components.h"
#include <iostream>
#include <typeindex>
#include "../ECSManager.h"

void Load::loadEntities(const std::filesystem::path& filePath, ECSManager* ECS)
{
	std::ifstream file(filePath);
	if (!file)
		return;

	JSON json;
	file >> json;

	// Even though multiple loops are used, most of them won't run many times.
	// It is just an easy way to iterate through the json file in a general way. 
	// TODO: Should be refactored
	for (const auto& outer : json.items())
	{
		uint32 entityID = ECS->newEntity();
		
		for (const auto& entity : outer.value().items())
		{
			for (const auto& components : entity.value().items())
			{
				for (auto component : components.value().items())
				{
					Component* comp = createComponent(component.key(), component.value(), entityID, ECS);
					
					if (comp)
						comp->jsonParse(component.value());
				}
			}
		}
	}
	file.close();
}

uint32 Load::loadEntity(uint32 entityID, const std::filesystem::path& filePath, ECSManager* ECS)
{
	std::ifstream file(filePath);
	
	if (!file)
		return entityID;

	JSON json;
	file >> json;


	for (const auto& outer : json.items())
	{

		for (const auto& entity : outer.value().items())
		{
			for (const auto& components : entity.value().items())
			{
				for (auto component : components.value().items())
				{
					Component* comp = createComponent(component.key(), component.value(), entityID, ECS);
					
					if (comp)
						comp->jsonParse(component.value());
				}
			}
		}
		break; // Only iterate one entity. The loop is just to get the iterator
	}
	file.close();

	return entityID;
}

void Load::loadComponent(const std::filesystem::path& filePath, Component* component)
{
	std::ifstream file(filePath);
	if (!file)
		return;

	JSON json;
	file >> json;

	for (const auto& entity : json)
	{

		for (const auto& it : entity.items())
		{

			component->jsonParse(it.value());
		}
			
	
	}

	

	file.close();

}

Component* Load::createComponent(std::string componentName, JSON jsonValue, uint32 entityID, ECSManager* ECS)
{
	Component* component{};
	if (componentName == std::type_index(typeid(TransformComponent)).name())
	{
		ECS->addComponent<TransformComponent>(entityID);
		component = ECS->getComponentManager<TransformComponent>()->getComponentChecked(entityID);
	}
	else if (componentName == std::type_index(typeid(AxisAlignedBoxComponent)).name())
	{
		ECS->addComponent<AxisAlignedBoxComponent>(entityID);
		component = ECS->getComponentManager<AxisAlignedBoxComponent>()->getComponentChecked(entityID);
	}
	else if (componentName == std::type_index(typeid(PhysicsComponent)).name())
	{
		ECS->addComponent<PhysicsComponent>(entityID);
		component = ECS->getComponentManager<PhysicsComponent>()->getComponentChecked(entityID);
	}	
	else if (componentName == std::type_index(typeid(HudComponent)).name())
	{
		ECS->addComponent<HudComponent>(entityID);
		component = ECS->getComponentManager<HudComponent>()->getComponentChecked(entityID);
	}
	else if (componentName == std::type_index(typeid(ParticleComponent)).name())
	{
		auto manager = ECS->getComponentManager<ParticleComponent>();
		if (!manager || !manager->getComponentChecked(entityID))
			ECS->addComponent<ParticleComponent>(entityID);
		component = ECS->getComponentManager<ParticleComponent>()->getComponentChecked(entityID);
	}
	else if (componentName == std::type_index(typeid(ScriptComponent)).name())
	{
		auto manager = ECS->getComponentManager<ScriptComponent>();
		if(!manager || !manager->getComponentChecked(entityID))
			ECS->addComponent<ScriptComponent>(entityID);
		component = ECS->getComponentManager<ScriptComponent>()->getComponentChecked(entityID);
	}
	else if (componentName == std::type_index(typeid(MeshComponent)).name())
	{
		if (jsonValue["reusable"])
		{
			std::string path = jsonValue["path"];
			if (path == "")
				return nullptr;
			ECS->loadAsset(entityID, path);
		}
		component = ECS->getComponentManager<MeshComponent>()->getComponentChecked(entityID);
	}	
	else if (componentName == std::type_index(typeid(TextureComponent)).name())
	{
		if (jsonValue["reusable"])
		{
			std::string path = jsonValue["path"];
			if (path == "")
				return nullptr;
			ECS->loadAsset(entityID, path);
		}
		component = ECS->getComponentManager<TextureComponent>()->getComponentChecked(entityID);
	}

	return component;
}
