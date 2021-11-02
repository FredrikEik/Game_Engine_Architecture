#include "Window.h"
#include "../CoreMinimal.h"

class Viewport;
class WorldOutliner : public Window
{
public:
	WorldOutliner(std::string inWindowName, class ECSManager* inECS);

	virtual void begin(void (Viewport::*inCallBack)(int32), class Viewport* inViewport);
	virtual void update() override;
	virtual void end() override;

protected:
	class ImGuiContext* context{};
	void(Viewport::*callBack)(int32) {};
};