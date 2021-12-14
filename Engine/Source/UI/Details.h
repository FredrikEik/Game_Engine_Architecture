#pragma once
#include "Window.h"
#include "../CoreMinimal.h"
#include <vector>
#include <typeindex>
class Details : public Window
{
public:
	Details(std::string inWindowName, class ECSManager* inECS);

	virtual void begin(class Viewport* inViewport, int32 inReservedEntities) override;
	virtual void update(int32 inEntityID);
	virtual void end() override;
protected:
	void drawAddComponent();
	void addComponent(std::string componentToAdd);
	void removeComponent(std::string componentToAdd);

	bool hasComponent(std::type_index type);

	void addTransformComponent();
	void addMeshComponent();
	void addAABBComponent();
	void addTextureComponent();
	void addParticleComponent();
	void addScriptComponent();

	std::vector<std::pair<std::type_index, uint32>> entity;
	int32 entityID{};
	const std::string transformComponent{ "Transform" };
	const std::string boxColliderComponent{ "Box Collider" };
	const std::string meshComponent{ "Mesh" };
	const std::string textureComponent{ "Texture" };
	const std::string particleComponent{ "Particle" };
	const std::string scriptComponent{ "Script" };
};

