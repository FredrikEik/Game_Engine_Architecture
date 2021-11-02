#pragma once
#include "Window.h"
#include "../CoreMinimal.h"

class TransformWidget : public Window
{
public:
	TransformWidget(std::string inWindowName, class ECSManager* inECS);

	virtual void begin(class Viewport* inViewport) ;
	virtual void update(int32 entityID);
	virtual void end() ;
};

