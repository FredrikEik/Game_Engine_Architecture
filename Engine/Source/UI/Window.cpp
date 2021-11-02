#include "Window.h"

Window::Window(std::string inWindowName, class ECSManager* inECS)
{
	windowName = inWindowName;
	ECS = inECS;
}

Window::~Window()
{
}
