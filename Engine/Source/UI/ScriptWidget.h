#pragma once
#include "Window.h"

class ScriptWidget : public Window
{
public:
	ScriptWidget(std::string inWindowName, class ECSManager* inECS);

	virtual void begin(class Viewport* inViewport, int32 inReservedEntities) override;
	virtual void update(int32 entityID, bool& entitiesChanged);
	virtual void end() override;

	std::string drawScriptSelector(uint32 entity, std::string currentClass, bool& OUTnewClassSelected);

	const std::string unit{ "Unit" };
	const std::string gameMode{ "GameMode" };
	const std::string ball{ "Ball" };

};

