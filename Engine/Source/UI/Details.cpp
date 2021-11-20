#include "Details.h"
#include "../ECSManager.h"
//#include <imgui.h>
//#include "../imgui/Bindings/imgui_impl_glfw.h"
//#include "../imgui/Bindings/imgui_impl_opengl3.h"
#include "../imgui/docking/imgui.h"
#include "../imgui/docking/imgui_impl_opengl3.h"
#include "../imgui/docking/imgui_impl_glfw.h"
#include "TransformWidget.h"
#include "nfd.h"
#include "../Systems/CollisionSystem.h"

Details::Details(std::string inWindowName, ECSManager* inECS)
	:Window(inWindowName, inECS)
{
}

void Details::begin(Viewport* inViewport, int32 inReservedEntities)
{
	Window::begin(inViewport, inReservedEntities);
}

void Details::update(int32 inEntityID)
{
	if (inEntityID < reservedEntities/* || !ECS->entityExists(inEntityID)*/)
		return;

	//while (!ECS->entityExists(inEntityID))
	//{
	//	inEntityID++;
	//	if (inEntityID > core::MAX_ENTITIES)
	//		inEntityID = 0;
	//}

	entityID = inEntityID;
	entity = ECS->getEntity(entityID);
	ImGui::BeginChild("Entities");
	for (const auto &it : entity)
	{
		if (it.first == std::type_index(typeid(TransformComponent)))
		{
			TransformWidget* transformWidget = new TransformWidget(windowName, ECS);
			transformWidget->begin(viewport, reservedEntities);
			transformWidget->update(entityID);
			transformWidget->end();
			delete transformWidget;
		}
	}
	//ImGui::BeginChild("AddComponent");
	//static bool popup{true};
	//ImGui::BeginPopupModal("Add component", &popup);
	//ImGui::Button("TransformWidget");
	//ImGui::EndPopup();
	//ImGui::EndChild();
	
	drawAddComponent();

	ImGui::EndChild();

}

void Details::end()
{
}

void Details::drawAddComponent()
{
	const char* items[] = { transformComponent.c_str(), boxColliderComponent.c_str(), meshComponent.c_str() };
	static const char* current_item = "Select component";
	ImGuiComboFlags flags = ImGuiComboFlags_NoArrowButton;

	ImGuiStyle& style = ImGui::GetStyle();
	float w = ImGui::CalcItemWidth();
	float spacing = style.ItemInnerSpacing.x;
	float button_sz = ImGui::GetFrameHeight();
	ImGui::PushItemWidth(w - spacing * 2.0f - button_sz * 2.0f);
	if (ImGui::BeginCombo("##custom combo", current_item, ImGuiComboFlags_NoArrowButton))
	{
		for (int n = 0; n < IM_ARRAYSIZE(items); n++)
		{
			bool is_selected = (current_item == items[n]);
			if (ImGui::Selectable(items[n], is_selected))
				current_item = items[n];
			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}
	ImGui::PopItemWidth();
	ImGui::SameLine(0, spacing);
	if (ImGui::Button("Add"))
	{
		addComponent(current_item);
		// do stuff
	}
	//if (ImGui::ArrowButton("##r", ImGuiDir_Left))
	//{
	//}
	//ImGui::SameLine(0, spacing);
	//if (ImGui::ArrowButton("##r", ImGuiDir_Right))
	//{
	//}
	//ImGui::SameLine(0, style.ItemInnerSpacing.x);
	//ImGui::Text("Custom Combo");
}

void Details::addComponent(std::string componentToAdd)
{
	bool hasComponent{ false };
	if (componentToAdd == transformComponent)
	{
		for (const auto& it : entity)
		{
			if (it.first == std::type_index(typeid(TransformComponent)))
				hasComponent = true;
		}
		if (!hasComponent)
			ECS->addComponent<TransformComponent>(entityID);
	}

	if (componentToAdd == boxColliderComponent)
	{
		for (const auto& it : entity)
		{
			if (it.first == std::type_index(typeid(AxisAlignedBoxComponent)))
				hasComponent = true;
		}
		if (!hasComponent)
			ECS->addComponent<AxisAlignedBoxComponent>(entityID);
	}

	if (componentToAdd == meshComponent)
	{
		for (const auto& it : entity)
		{
			if (it.first == std::type_index(typeid(MeshComponent)))
				hasComponent = true;
		}
		if (!hasComponent)
		{
			nfdchar_t* outPath = NULL;
			nfdresult_t result = NFD_OpenDialog(NULL, NULL, &outPath);

			if (result == NFD_OKAY) {
				//puts("Success!");
				//puts(outPath);
				//Engine::Get().load(outPath);
				ECS->loadAsset(entityID, outPath);
				ECS->addComponent<AxisAlignedBoxComponent>(entityID);
				CollisionSystem::construct(entityID, ECS);
				//MeshComponent*  ECS->getComponentManager<MeshComponent>()->getComponentChecked();
				free(outPath);
				//selectedEntity = -1;
				//std::cout << outPath;
			}
			else if (result == NFD_CANCEL) {
				puts("User pressed cancel.");
			}
			else {
				printf("Error: %s\n", NFD_GetError());
			}
		}
	}
}
