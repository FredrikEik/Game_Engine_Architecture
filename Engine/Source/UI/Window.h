#pragma once
#include <string>

class Window
{
public:
	Window(std::string inWindowName, class ECSManager* inECS);
	virtual ~Window();
	
	virtual void begin(class Viewport* inViewport) { viewport = inViewport; }
	virtual void update() {};
	virtual void end() {};
protected:
	std::string windowName;
	class ECSManager* ECS;
	class Viewport* viewport{};

};