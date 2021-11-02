#pragma once
#include <string>
#include "../CoreMinimal.h"
class Window
{
public:
	Window(std::string inWindowName, class ECSManager* inECS);
	virtual ~Window();
	
	virtual void begin(class Viewport* inViewport, int32 inReservedEntities) {
		viewport = inViewport;
		reservedEntities = inReservedEntities;
	}
	virtual void update() {};
	virtual void end() {};
protected:
	std::string windowName;
	class ECSManager* ECS;
	class Viewport* viewport{};
	int32 reservedEntities{};
};