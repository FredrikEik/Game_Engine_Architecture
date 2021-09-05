#include "BaseComponent.h"

uint32 BaseComponent::componentID = 0;

uint32 BaseComponent::nextID()
{
	return componentID++;
}