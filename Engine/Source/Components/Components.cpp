#include "Components.h"
#include <iostream>
#include "../SaveLoad/Save.h"
#include "../Systems/ParticleSystem.h"
#include "../Systems/ScriptSystem.h"
#include "../Engine/Engine.h"
#include "../Systems/HudSystem.h"

JSON TransformComponent::json()
{
	JSON matrix;
	for (int i{}; i < 4; ++i)
	{
		JSON row = nlohmann::json::array();
		for (int j{}; j < 4; ++j)
		{
			row.push_back(transform[i][j]);
		}
		matrix.push_back(row);
	}
	return matrix;
}

void TransformComponent::jsonParse(const JSON& json)
{
	for (int i{}; i < 4; ++i)
	{
		for (int j{}; j < 4; ++j)
		{
			transform[i][j] = json[i][j];
		}
	}
}

JSON MeshComponent::json()
{
	// TODO: If there is no path, save vertices and indices as well OR save the mesh as a .obj
	JSON json({ 
		{"reusable", true},
		{"path", path},
		{"drawType", m_drawType},
		{"LODType", LODType},
		{"bIsTranslucent", bIsTranslucent},
		{"bDisregardedDuringFrustumCulling", bDisregardedDuringFrustumCulling},
		{"bShouldRender", bShouldRender}
	});

	return json;
}

void MeshComponent::jsonParse(const JSON& json)
{
	path = json["path"];
	m_drawType = json["drawType"];
	LODType = json["LODType"];
	bIsTranslucent = json["bIsTranslucent"];
	bDisregardedDuringFrustumCulling = json["bDisregardedDuringFrustumCulling"];
	bShouldRender = json["bShouldRender"];
}

JSON AxisAlignedBoxComponent::json()
{
	JSON json({
		{"min", JSON::array({minScaled.x, minScaled.y, minScaled.z})},
		{"max", JSON::array({maxScaled.x, maxScaled.y, maxScaled.z}) },
		{"center", JSON::array({center.x, center.y, center.z}) },
		{"bShouldGenerateOverlapEvents", bShouldGenerateOverlapEvents}
		});
	return json;
}

void AxisAlignedBoxComponent::jsonParse(const JSON& json)
{
	minScaled = glm::vec3(json["min"][0], json["min"][1], json["min"][2]);
	maxScaled = glm::vec3(json["max"][0], json["max"][1], json["max"][2]);
	center = glm::vec3(json["center"][0], json["center"][1], json["center"][2]);
	bShouldGenerateOverlapEvents = json["bShouldGenerateOverlapEvents"];
}

JSON TextureComponent::json()
{
	JSON json{
	{"reusable", true},
	{"path", path},
	{"wrapMode", wrapMode}
	};

	return json;
}

void TextureComponent::jsonParse(const JSON& json)
{
	wrapMode = json["wrapMode"];
	path = json["path"];
}

JSON ParticleComponent::json()
{
	JSON json{
	{"textureRows", textureRows},
	{"maxParticles", maxParticles},
	{"spawnRate", spawnRate},
	{"spawnFrequency", spawnFrequency},
	{"emitterTotalLifeTime", emitterTotalLifeTime},
	{"bLoops", bLoops},
	{"destroyOnLifetimeEnd", destroyOnLifetimeEnd},
	{"blendSFactor", blendSFactor},
	{"blendDFactor", blendDFactor},
	{"mesh", mesh.json()},
	{"texture", texture.json()},
	{"particleBlueprint", {
		{"particle", {
			{"startColor", Save::vec4JSON(particleBlueprint.particle.startColor)},
			{"endColor", Save::vec4JSON(particleBlueprint.particle.endColor)},
			{"position", Save::vec3JSON(particleBlueprint.particle.position)},
			{"velocity", Save::vec3JSON(particleBlueprint.particle.velocity)},
			{"acceleration", Save::vec3JSON(particleBlueprint.particle.acceleration)},
			{"currentLife", particleBlueprint.particle.currentLife},
			{"startSize", particleBlueprint.particle.startSize},
			{"endSize", particleBlueprint.particle.endSize}
			}
		},
		{"positionMinOffset", Save::vec3JSON(particleBlueprint.positionMinOffset)},
		{"positionMaxOffset", Save::vec3JSON(particleBlueprint.positionMaxOffset)},
		{"velocityMinOffset", Save::vec3JSON(particleBlueprint.velocityMinOffset)},
		{"velocityMaxOffset", Save::vec3JSON(particleBlueprint.velocityMaxOffset)},
		{"accelerationMinOffset", Save::vec3JSON(particleBlueprint.accelerationMinOffset)},
		{"accelerationMaxOffset", Save::vec3JSON(particleBlueprint.accelerationMaxOffset)},
		{"colorMinOffset", Save::vec4JSON(particleBlueprint.colorMinOffset)},
		{"colorMaxOffset", Save::vec4JSON(particleBlueprint.colorMaxOffset)},
		{"sizeMinOffset", particleBlueprint.sizeMinOffset},
		{"sizeMaxOffset", particleBlueprint.sizeMaxOffset},
		{"lifeMinOffset", particleBlueprint.lifeMinOffset},
		{"lifeMaxOffset", particleBlueprint.lifeMaxOffset}
	}
	}
	};
	return json;
}

void ParticleComponent::jsonParse(const JSON& json)
{
	auto vec3 = [](const JSON& json)
	{return glm::vec3(json[0], json[1], json[2]); };	
	auto vec4 = [](const JSON& json)
	{return glm::vec4(json[0], json[1], json[2], json[3]); };

	textureRows = json["textureRows"];
	maxParticles = json["maxParticles"];
	spawnRate = json["spawnRate"];
	spawnFrequency = json["spawnFrequency"];
	emitterTotalLifeTime = json["emitterTotalLifeTime"];
	bLoops = json["bLoops"];
	destroyOnLifetimeEnd = json["destroyOnLifetimeEnd"];
	blendSFactor = json["blendSFactor"];
	blendDFactor = json["blendDFactor"];
	mesh.jsonParse(json["mesh"]);
	texture.jsonParse(json["texture"]);

	const JSON& bp = json["particleBlueprint"];
	const JSON& bpParticle = bp["particle"];
	particleBlueprint.particle.startColor = vec4(bpParticle["startColor"]);
	particleBlueprint.particle.endColor = vec4(bpParticle["endColor"]);
	particleBlueprint.particle.position = vec3(bpParticle["position"]);
	particleBlueprint.particle.velocity = vec3(bpParticle["velocity"]);
	particleBlueprint.particle.acceleration = vec3(bpParticle["acceleration"]);
	particleBlueprint.particle.currentLife = bpParticle["currentLife"];
	particleBlueprint.particle.startSize = bpParticle["startSize"];
	particleBlueprint.particle.endSize = bpParticle["endSize"];

	particleBlueprint.positionMinOffset = vec3(bp["positionMinOffset"]);
	particleBlueprint.positionMaxOffset = vec3(bp["positionMaxOffset"]);
	particleBlueprint.velocityMinOffset = vec3(bp["velocityMinOffset"]);
	particleBlueprint.velocityMaxOffset = vec3(bp["velocityMaxOffset"]);
	particleBlueprint.accelerationMinOffset = vec3(bp["accelerationMinOffset"]);
	particleBlueprint.accelerationMaxOffset = vec3(bp["accelerationMaxOffset"]);
	particleBlueprint.colorMinOffset = vec4(bp["colorMinOffset"]);
	particleBlueprint.colorMaxOffset = vec4(bp["colorMaxOffset"]);
	particleBlueprint.sizeMinOffset = bp["sizeMinOffset"];
	particleBlueprint.sizeMaxOffset = bp["sizeMaxOffset"];
	particleBlueprint.lifeMinOffset = bp["lifeMinOffset"];
	particleBlueprint.lifeMaxOffset = bp["lifeMaxOffset"];

	ParticleSystem::initMesh(this, maxParticles);
	ParticleSystem::initTexture(this, texture.path, textureRows);
}

JSON ScriptComponent::json()
{
	return JSON{
	{"ScriptClassName", ScriptClassName}
	};
}

void ScriptComponent::jsonParse(const JSON& json)
{
	ScriptClassName = json["ScriptClassName"];
	ScriptSystem::InitScriptObject(this, ScriptClassName);

	if(Engine::Get().getIsPlaying())
		ScriptSystem::Invoke(entityID, "BeginPlay", Engine::Get().getECSManager());
}

JSON PhysicsComponent::json()
{
	return {
		{"mass", mass},
		{"restitution", restitution},
	};
}

void PhysicsComponent::jsonParse(const JSON& json)
{
	mass = json["mass"];
	restitution = json["restitution"];
}

JSON HudComponent::json()
{
	return JSON{ 
		{"center", Save::vec4JSON(center)},
		{"texture", texture.json()},
		{"mesh", mesh.json()}
	};
}

void HudComponent::jsonParse(const JSON& json)
{
	auto vec4 = [](const JSON& json)
	{return glm::vec4(json[0], json[1], json[2], json[3]); };

	center = vec4(json["center"]);
	mesh.jsonParse(json["mesh"]);
	texture.jsonParse(json["texture"]);

	HudSystem::init(entityID, Engine::Get().getECSManager(), texture.path);
}
