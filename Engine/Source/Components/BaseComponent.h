#pragma once
#include "../CoreMinimal.h"


class Component 
{
public:
	inline Component(uint32 entity, uint32 componentID) : entityID{ entity }, ID{ componentID } {}
	~Component(){}
	uint32 ID;
	uint32 entityID;

};

class testComponent : public Component 
{
public:
	testComponent(uint32 entity, uint32 componentID) : Component(entity, componentID) {}
	//testComponent(const testComponent& other) : Component(other) {}
	~testComponent(){}
	float pos[3]{};
};