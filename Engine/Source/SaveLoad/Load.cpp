#include "Load.h"
#include <fstream>
#include "../Components/Components.h"
#include <iostream>
#include <typeindex>
#include "../ECSManager.h"
void Load::loadEntities(const std::filesystem::path& filePath, ECSManager* ECS)
{
	std::ifstream file(filePath);
	assert(file);

	JSON json;
	file >> json;

	// Even though multiple loops are used, most of them won't run many times.
	// It is just an easy way to iterate through the json file in a general way. 
	// TODO: Should be refactored
	for (const auto& outer : json.items())
	{
		uint32 entityID = ECS->newEntity();
		//std::cout << entity << "\n\n\n\n";
		for (const auto& entity : outer.value().items())
		{
			for (const auto& components : entity.value().items())
			{
				for (auto component : components.value().items())
				{
					Component* comp = createComponent(component.key(), component.value(), entityID, ECS);
					//std::cout <<entityID << " " << component.key() << "\n\n\n";
					if (comp)
						comp->jsonParse(component.value());
				}
			}
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
