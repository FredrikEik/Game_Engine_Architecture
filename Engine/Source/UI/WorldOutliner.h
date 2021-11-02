#include "Window.h"
#include "../CoreMinimal.h"

class Viewport;
class WorldOutliner : public Window
{
public:
	WorldOutliner(std::string inWindowName, class ECSManager* inECS);

	virtual void begin(class Viewport* inViewport, int32 inReservedEntities);
	virtual void update() override;
	virtual void end() override;

protected:
	class ImGuiContext* context{};

};