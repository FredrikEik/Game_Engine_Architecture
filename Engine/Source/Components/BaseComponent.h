#pragma once
#include "../CoreMinimal.h"

class BaseComponent
{
protected:
	static uint32 nextID();
};

template <typename T>
class Component : public BaseComponent
{
public:
	// Figure out if the template pattern is really necessary
	inline Component(uint32 entity) : entityID{ entity }, ID{ nextID() } {}
	const uint32 ID;
	const uint32 entityID;
protected:
};

class positionComponent : public Component <positionComponent>
{
public:
	positionComponent(uint32 entity) : Component(entity) {}
	float pos[3]{};
};