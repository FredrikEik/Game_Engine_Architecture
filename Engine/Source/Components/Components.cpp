#include "Components.h"
#include <iostream>
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

	std::cout << json;
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
}
