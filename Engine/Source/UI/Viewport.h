#pragma once
#include "../CoreMinimal.h"
class Viewport /*: public ImGui*/
{
public:
	Viewport(class ECSManager* InECS);
	~Viewport();
	void begin(struct GLFWwindow* window);
	void render();
	void end();
	void setCurrentEntity(int32 entityID);
protected:
	struct GLFWwindow* window;
	class ECSManager* ECS;
	class WorldOutliner* worldOutliner{};
	class Details* details{};
		
	int selectedEntity{}; // TODO: VERY TEMP, remove as soon as widgets are implemented

};

