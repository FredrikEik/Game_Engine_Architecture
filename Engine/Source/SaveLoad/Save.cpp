#include "Save.h"
#include "../JSON/json.hpp"
#include "../ECSManager.h"
#include "../Components/Components.h"
#include <fstream>
#include <filesystem>

using JSON = nlohmann::json;
std::string Save::defaultSaveLocation = "../saves";
std::string Save::defaultSaveName = "/entities.json";

void Save::saveEntities(const std::array<
	std::pair<bool, std::vector<std::pair<std::type_index, uint32>>>,
	core::MAX_ENTITIES>& entities, 
	uint32 startIndex, class ECSManager* ECS)
{
	JSON json;

	for (uint32 i{ startIndex }; i < entities.size(); ++i)
	{
		if (!entities[i].first) // Entity is not in use
			continue;
		JSON components;
		for (const auto& it : entities[i].second)
		{	
			// TODO: Add a flag for if the component is reusable to make sure loading just loads from path
			// This should be done in the component's json() function.
			addComponentToJson(it.first, i, components, ECS);
		}
		json.push_back({ { std::to_string(i), components } });
	}

	std::filesystem::create_directory(defaultSaveLocation);
	std::ofstream file(std::string(defaultSaveLocation + defaultSaveName));
	assert(file);
	file << std::setw(4) << json << std::endl;
	file.close();
}

void Save::addComponentToJson(std::type_index type, uint32 entityID, nlohmann::json& OUTjson, ECSManager* ECS)
{
	JSON json;
	if (type == std::type_index(typeid(TransformComponent)))
	{
		auto comp = ECS->getComponentManager<TransformComponent>()->getComponentChecked(entityID);
		assert(comp);
		json = comp->json();
	}
	else if (type == std::type_index(typeid(MeshComponent)))
	{
		auto comp = ECS->getComponentManager<MeshComponent>()->getComponentChecked(entityID);
		assert(comp);
		json = comp->json();
	}
	else if (type == std::type_index(typeid(TextureComponent)))
	{
		auto comp = ECS->getComponentManager<TextureComponent>()->getComponentChecked(entityID);
		assert(comp);
		json = comp->json();
	}
	else if (type == std::type_index(typeid(AxisAlignedBoxComponent)))
	{
		auto comp = ECS->getComponentManager<AxisAlignedBoxComponent>()->getComponentChecked(entityID);
		assert(comp);
		json = comp->json();
	}
	else if (type == std::type_index(typeid(CameraComponent)))
	{
		auto comp = ECS->getComponentManager<CameraComponent>()->getComponentChecked(entityID);
		assert(comp);
		json = comp->json();
	}
	else if (type == std::type_index(typeid(SphereComponent)))
	{
		auto comp = ECS->getComponentManager<SphereComponent>()->getComponentChecked(entityID);
		assert(comp);
		json = comp->json();
	}
	else if (type == std::type_index(typeid(SoundComponent)))
	{
		auto comp = ECS->getComponentManager<SoundComponent>()->getComponentChecked(entityID);
		assert(comp);
		json = comp->json();
	}


	if (json == JSON())
		return;
	OUTjson.push_back({ { type.name(), json} });
}
