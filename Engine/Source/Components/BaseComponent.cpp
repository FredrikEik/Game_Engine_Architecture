#include "BaseComponent.h"

static uint32 componentID = 0;

uint32 BaseComponent::nextID()
{
	return componentID++;
}