#pragma once
#include "../CoreMinimal.h"

class BaseComponent
{
public:
	static uint32 componentID;
	static uint32 nextID();
	~BaseComponent(){}
};

template <typename T>
class Component : public BaseComponent
{
public:
	// Figure out if the template pattern is really necessary
	inline Component(uint32 entity) : entityID{ entity }, ID{ nextID() } {}
	//inline Component(const T& other) : ID{ other.ID }, entityID{other.entityID} {}
	~Component(){}
	uint32 ID;
	uint32 entityID;
	//T& operator= (const T& other) {}
};

class testComponent : public Component <testComponent>
{
public:
	testComponent(uint32 entity) : Component(entity) {}
	//testComponent(const testComponent& other) : Component(other) {}
	~testComponent(){}
	float pos[3]{};
};