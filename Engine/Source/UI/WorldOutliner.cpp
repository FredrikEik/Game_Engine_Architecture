#include "WorldOutliner.h"
//#include <imgui.h>
//#include "../imgui/Bindings/imgui_impl_glfw.h"
//#include "../imgui/Bindings/imgui_impl_opengl3.h"
#include "../imgui/docking/imgui.h"
#include "../imgui/docking/imgui_impl_opengl3.h"
#include "../imgui/docking/imgui_impl_glfw.h"
#include "../ECSManager.h"
#include "Viewport.h"
WorldOutliner::WorldOutliner(std::string inWindowName, ECSManager* inECS)
	: Window(inWindowName, inECS)
{
}

void WorldOutliner::begin(class Viewport* inViewport, int32 inReservedEntities)
{
	//callBack = inCallBack;
	viewport = inViewport;
	reservedEntities = inReservedEntities;
	//context = ImGui::CreateContext();

	//ImGui::Begin(windowName.c_str());

	//ImGui::Begin("World Outliner");
}

void WorldOutliner::update()
{
	int32 numberOfEntities = ECS->getNumberOfEntities();
	int currentItem{};

	// TODO: Make it so that this does not break when an entity is removed
	ImGui::BeginChild("Entities");
	for (int32 i{ reservedEntities }; i < numberOfEntities; ++i)
	{
		//ImGui::Text("%04d: Some text", i);
		std::string str = "Entity ";
		str.append( std::to_string(i));
		if (ImGui::Button(str.c_str()))
			viewport->setCurrentEntity(i);
	}
	ImGui::EndChild();

}

void WorldOutliner::end()
{
	//ImGui::End();
	//ImGui::DestroyContext(context);
}
