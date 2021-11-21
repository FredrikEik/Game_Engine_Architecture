#include "Details.h"
#include "../ECSManager.h"
//#include <imgui.h>
//#include "../imgui/Bindings/imgui_impl_glfw.h"
//#include "../imgui/Bindings/imgui_impl_opengl3.h"
#include "../imgui/docking/imgui.h"
#include "../imgui/docking/imgui_impl_opengl3.h"
#include "../imgui/docking/imgui_impl_glfw.h"
#include "TransformWidget.h"
#include "MeshWidget.h"
#include "nfd.h"
#include "../Systems/CollisionSystem.h"
#include "../FileSystemHelpers.h"

using TYPE = std::type_index;

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
		// Creating new widgets every frame for now. If performance becomes an issue, refactor.
		if (it.first == std::type_index(typeid(TransformComponent)).hash_code())
		{
			TransformWidget* transformWidget = new TransformWidget(windowName, ECS);
			transformWidget->begin(viewport, reservedEntities);
			transformWidget->update(entityID);
			transformWidget->end();
			delete transformWidget;
		}
		else if (it.first == TYPE(typeid(MeshComponent)).hash_code())
		{
			ImGui::NewLine();

			MeshWidget* meshWidget = new MeshWidget(windowName, ECS);
			meshWidget->begin(viewport, reservedEntities);
			meshWidget->update(entityID);
			meshWidget->end();
			delete meshWidget;
		}
	}
	//ImGui::BeginChild("AddComponent");
	//static bool popup{true};
	//ImGui::BeginPopupModal("Add component", &popup);
	//ImGui::Button("TransformWidget");
	//ImGui::EndPopup();
	//ImGui::EndChild();
	ImGui::NewLine();

	drawAddComponent();

	ImGui::EndChild();

}

void Details::end()
{
}

void Details::drawAddComponent()
{
	const char* items[] = { 
		transformComponent.c_str(), 
		boxColliderComponent.c_str(), 
		meshComponent.c_str(),
		textureComponent.c_str()
	};
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
		addTransformComponent();
	}
	else if (componentToAdd == boxColliderComponent)
	{
		addAABBComponent();
	}
	else if (componentToAdd == meshComponent)
	{
		addMeshComponent();
	}
	else if (componentToAdd == textureComponent)
		addTextureComponent();
}

bool Details::hasComponent(size_t type)
{
	for (const auto& it : entity)
	{
		if (it.first == type)
			return true;
	}

	return false;
}

void Details::addTransformComponent()
{
	if(!hasComponent(TYPE(typeid(TransformComponent)).hash_code()))
		ECS->addComponent<TransformComponent>(entityID);
}

void Details::addMeshComponent()
{
	if(hasComponent(TYPE(typeid(MeshComponent)).hash_code()))
		return;
	std::string path;
	if (FileSystemHelpers::getPathFromFileExplorer(path))
	{
		if (!hasComponent(TYPE(typeid(TransformComponent)).hash_code()))
			addTransformComponent();

		ECS->loadAsset(entityID, path);

		if (!hasComponent(TYPE(typeid(AxisAlignedBoxComponent)).hash_code()))
		{
			MeshSystem::setConsideredForFrustumCulling(entityID, ECS, false);
		}
	}
}

void Details::addAABBComponent()
{
	if (!hasComponent(TYPE(typeid(AxisAlignedBoxComponent)).hash_code()))
		ECS->addComponent<AxisAlignedBoxComponent>(entityID);
}

void Details::addTextureComponent()
{
	if (hasComponent(TYPE(typeid(TextureComponent)).hash_code()))
		return;
	std::string path;
	if (FileSystemHelpers::getPathFromFileExplorer(path))
	{
		ECS->loadAsset(entityID, path);
	}
}
