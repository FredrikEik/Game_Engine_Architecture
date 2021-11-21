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
	std::pair<bool, std::vector<std::pair<size_t, uint32>>>,
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

std::string Save::getDefaultAbsolutePath()
{
	return std::string(defaultSaveLocation + defaultSaveName);
}

void Save::addComponentToJson(size_t type, uint32 entityID, nlohmann::json& OUTjson, ECSManager* ECS)
{
	JSON json;
	if (type == std::type_index(typeid(TransformComponent)).hash_code())
	{
		auto comp = ECS->getComponentManager<TransformComponent>()->getComponentChecked(entityID);
		assert(comp);
		json = comp->json();
	}
	else if (type == std::type_index(typeid(MeshComponent)).hash_code())
	{
		auto comp = ECS->getComponentManager<MeshComponent>()->getComponentChecked(entityID);
		assert(comp);
		json = comp->json();
	}
	else if (type == std::type_index(typeid(TextureComponent)).hash_code())
	{
		auto comp = ECS->getComponentManager<TextureComponent>()->getComponentChecked(entityID);
		assert(comp);
		json = comp->json();
	}
	else if (type == std::type_index(typeid(AxisAlignedBoxComponent)).hash_code())
	{
		auto comp = ECS->getComponentManager<AxisAlignedBoxComponent>()->getComponentChecked(entityID);
		assert(comp);
		json = comp->json();
	}
	else if (type == std::type_index(typeid(CameraComponent)).hash_code())
	{
		auto comp = ECS->getComponentManager<CameraComponent>()->getComponentChecked(entityID);
		assert(comp);
		json = comp->json();
	}
	else if (type == std::type_index(typeid(SphereComponent)).hash_code())
	{
		auto comp = ECS->getComponentManager<SphereComponent>()->getComponentChecked(entityID);
		assert(comp);
		json = comp->json();
	}
	else if (type == std::type_index(typeid(SoundComponent)).hash_code())
	{
		auto comp = ECS->getComponentManager<SoundComponent>()->getComponentChecked(entityID);
		assert(comp);
		json = comp->json();
	}


	if (json == JSON())
		return;
	OUTjson.push_back({ {type, json} });
}
