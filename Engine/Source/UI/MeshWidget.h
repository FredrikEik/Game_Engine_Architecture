#pragma once
#include "Window.h"
class MeshWidget : public Window
{
public:
	MeshWidget(std::string inWindowName, class ECSManager* inECS);

	virtual void begin(class Viewport* inViewport, int32 inReservedEntities) override;
	virtual void update(int32 entityID);
	virtual void end() override;
};

