#pragma once
#include "Window.h"
class ParticleWidget : public Window
{
public:
	ParticleWidget(std::string inWindowName, class ECSManager* inECS);

	virtual void begin(class Viewport* inViewport, int32 inReservedEntities) override;
	virtual void update(int32 entityID, bool& entitiesChanged);
	virtual void end() override;
};

