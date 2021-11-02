#pragma once
#include "../CoreMinimal.h"
#include <string>
class Viewport /*: public ImGui*/
{
public:
	Viewport(class ECSManager* InECS);
	~Viewport();
	void begin(struct GLFWwindow* window, int32 inReservedEntities);
	void render();
	void end();
	void setCurrentEntity(int32 entityID);
protected:
	void togglePlay();
protected:
	struct GLFWwindow* window;
	class ECSManager* ECS;
	class WorldOutliner* worldOutliner{};
	class Details* details{};
	int32 reservedEntities{};
	int selectedEntity{}; // TODO: VERY TEMP, remove as soon as widgets are implemented
	std::string playButtonText;
	bool bIsPlaying{ false };
};

