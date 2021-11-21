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

	bool hasComponent(size_t type);

	void addTransformComponent();
	void addMeshComponent();
	void addAABBComponent();
	void addTextureComponent();

	std::vector<std::pair<size_t, uint32>> entity;
	int32 entityID{};
	const std::string transformComponent{ "Transform" };
	const std::string boxColliderComponent{ "Box Collider" };
	const std::string meshComponent{ "Mesh" };
	const std::string textureComponent{ "Texture" };
};

