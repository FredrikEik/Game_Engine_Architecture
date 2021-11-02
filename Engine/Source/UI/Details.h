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
	std::vector<std::pair<std::type_index, uint32>> entity;
	int32 entityID{};
	const std::string transformComponent{ "Transform" };
	const std::string boxColliderComponent{ "Box Collider" };
	const std::string meshComponent{ "Mesh" };
};

