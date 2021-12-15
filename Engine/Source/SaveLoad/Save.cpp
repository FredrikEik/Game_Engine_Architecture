#include "Save.h"
#include "../JSON/json.hpp"
#include "../ECSManager.h"
#include "../Components/Components.h"
#include <fstream>
#include <filesystem>
#include "../FileSystemHelpers.h"

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

void Save::saveEntityPrefab(uint32 entityID, ECSManager* ECS)
{
	std::string path;
	if (!FileSystemHelpers::getSavePathFromFileExplorer(path))
		return;

	JSON json;
	auto entity = ECS->getEntity(entityID);

	JSON components;
	for (const auto& it : entity)
	{
		// TODO: Add a flag for if the component is reusable to make sure loading just loads from path
		// This should be done in the component's json() function.
		addComponentToJson(it.first, entityID, components, ECS);
	}
	json.push_back({ { std::to_string(entityID), components } });
	
	std::ofstream file(path);
	assert(file);
	file << std::setw(4) << json << std::endl;
	file.close();
}

void Save::saveComponent(uint32 entityID, std::type_index type, class ECSManager* ECS)
{
	std::string path;
	if (!FileSystemHelpers::getSavePathFromFileExplorer(path))
		return;
	JSON component;
	auto entity = ECS->getEntity(entityID);
	addComponentToJson(type, entityID, component, ECS);
	std::ofstream file(path);
	assert(file);
	file << std::setw(4) << component << std::endl;
	file.close();
}

std::string Save::getDefaultAbsolutePath()
{
	return std::string(defaultSaveLocation + defaultSaveName);
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
	else if (type == std::type_index(typeid(ParticleComponent)))
	{
		auto comp = ECS->getComponentManager<ParticleComponent>()->getComponentChecked(entityID);
		assert(comp);
		json = comp->json();
	}
	else if (type == std::type_index(typeid(ScriptComponent)))
	{
		auto comp = ECS->getComponentManager<ScriptComponent>()->getComponentChecked(entityID);
		assert(comp);
		json = comp->json();
	}
	else if (type == std::type_index(typeid(PhysicsComponent)))
	{
		auto comp = ECS->getComponentManager<PhysicsComponent>()->getComponentChecked(entityID);
		assert(comp);
		json = comp->json();
	}	
	else if (type == std::type_index(typeid(HudComponent)))
	{
		auto comp = ECS->getComponentManager<HudComponent>()->getComponentChecked(entityID);
		assert(comp);
		json = comp->json();
	}
	if (json == JSON())
		return;
	OUTjson.push_back({ { type.name(), json} });
}

nlohmann::json Save::vec3JSON(const glm::vec3& v)
{
	return JSON::array({ v.x, v.y, v.z });
}

nlohmann::json Save::vec4JSON(const glm::vec4& v)
{
	return JSON::array({ v.x, v.y, v.z, v.w });
}
