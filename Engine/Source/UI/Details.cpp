#include "Details.h"
#include "../ECSManager.h"
#include "../imgui/docking/imgui.h"
#include "../imgui/docking/imgui_impl_opengl3.h"
#include "../imgui/docking/imgui_impl_glfw.h"
#include "TransformWidget.h"
#include "MeshWidget.h"
#include "TextureWidget.h"
#include "BoxColliderWidget.h"
#include "ParticleWidget.h"
#include "../Systems/ParticleSystem.h"
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
	ImGui::BeginChild("Entities");
	entity = ECS->getEntity(entityID);
	for (const auto &it : entity)
	{
		// Creating new widgets every frame for now. If performance becomes an issue, refactor.
		std::type_index index = it.first;
		if (index == std::type_index(typeid(TransformComponent)))
		{
			TransformWidget* transformWidget = new TransformWidget(windowName, ECS);
			transformWidget->begin(viewport, reservedEntities);
			transformWidget->update(entityID);
			transformWidget->end();
			delete transformWidget;
		}
		else if (index == TYPE(typeid(MeshComponent)))
		{
			bool bEntityChanged{};
			MeshWidget* meshWidget = new MeshWidget(windowName, ECS);
			ImGui::NewLine();
			meshWidget->begin(viewport, reservedEntities);
			meshWidget->update(entityID, bEntityChanged);
			meshWidget->end();
			delete meshWidget;
			if (bEntityChanged)
			{
				entity = ECS->getEntity(entityID);
				break;
			}
		}
		else if (index == TYPE(typeid(AxisAlignedBoxComponent)))
		{
			bool bEntityChanged{};
			BoxColliderWidget* boxColliderWidget = new BoxColliderWidget(windowName, ECS);
			ImGui::NewLine();
			boxColliderWidget->begin(viewport, reservedEntities);
			boxColliderWidget->update(entityID, bEntityChanged);
			boxColliderWidget->end();
			delete boxColliderWidget;
			if (bEntityChanged)
			{
				entity = ECS->getEntity(entityID);
				break;
			}
		}
		else if (index == TYPE(typeid(TextureComponent)))
		{
			bool bEntityChanged{};
			TextureWidget* textureWidget = new TextureWidget(windowName, ECS);
			ImGui::NewLine();
			textureWidget->begin(viewport, reservedEntities);
			textureWidget->update(entityID, bEntityChanged);
			textureWidget->end();
			delete textureWidget;
			if (bEntityChanged)
			{
				entity = ECS->getEntity(entityID);
				break;
			}
		}
		else if (index == TYPE(typeid(ParticleComponent)))
		{
			bool bEntityChanged{};
			ParticleWidget* particleWidget = new ParticleWidget(windowName, ECS);
			ImGui::NewLine();
			particleWidget->begin(viewport, reservedEntities);
			particleWidget->update(entityID, bEntityChanged);
			particleWidget->end();
			delete particleWidget;
			if (bEntityChanged)
			{
				entity = ECS->getEntity(entityID);
				break;
			}
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
		textureComponent.c_str(),
		particleComponent.c_str()
	};
	static const char* currentItem = "Select component";
	ImGuiComboFlags flags = ImGuiComboFlags_NoArrowButton;

	ImGuiStyle& style = ImGui::GetStyle();
	float w = ImGui::CalcItemWidth();
	float spacing = style.ItemInnerSpacing.x;
	float button_sz = ImGui::GetFrameHeight();
	ImGui::PushItemWidth(w - spacing * 2.0f - button_sz * 2.0f);
	if (ImGui::BeginCombo("##custom combo", currentItem, ImGuiComboFlags_NoArrowButton))
	{
		for (int n = 0; n < IM_ARRAYSIZE(items); n++)
		{
			bool is_selected = (currentItem == items[n]);
			if (ImGui::Selectable(items[n], is_selected))
				currentItem = items[n];
			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}
	ImGui::PopItemWidth();
	ImGui::SameLine(0, spacing);
	if (ImGui::Button("Add"))
	{
		addComponent(currentItem);
	}
	if (ImGui::Button("Remove"))
	{
		removeComponent(currentItem);
	}
}

void Details::addComponent(std::string componentToAdd)
{
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
	else if (componentToAdd == particleComponent)
		addParticleComponent();
}

void Details::removeComponent(std::string componentToAdd)
{
	if (componentToAdd == transformComponent && hasComponent(TYPE(typeid(TransformComponent))))
	{
		ECS->removeComponent<TransformComponent>(entityID);
	}
	else if (componentToAdd == boxColliderComponent && hasComponent(TYPE(typeid(AxisAlignedBoxComponent))))
	{
		ECS->removeComponent<AxisAlignedBoxComponent>(entityID);

	}
	else if (componentToAdd == meshComponent && hasComponent(TYPE(typeid(MeshComponent))))
	{
		ECS->removeComponent<MeshComponent>(entityID);

	}
	else if (componentToAdd == textureComponent && hasComponent(TYPE(typeid(TextureComponent))))
		ECS->removeComponent<TextureComponent>(entityID);

}

bool Details::hasComponent(std::type_index type)
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
	if(!hasComponent(TYPE(typeid(TransformComponent))))
		ECS->addComponent<TransformComponent>(entityID);
}

void Details::addMeshComponent()
{
	if(hasComponent(TYPE(typeid(MeshComponent))))
		return;
	std::string path;
	if (FileSystemHelpers::getPathFromFileExplorer(path))
	{
		if (!hasComponent(TYPE(typeid(TransformComponent))))
			addTransformComponent();

		ECS->loadAsset(entityID, path);

		if (!hasComponent(TYPE(typeid(AxisAlignedBoxComponent))))
		{
			MeshSystem::setConsideredForFrustumCulling(entityID, ECS, false);
		}
	}
}

void Details::addAABBComponent()
{
	if (!hasComponent(TYPE(typeid(AxisAlignedBoxComponent))))
		ECS->addComponent<AxisAlignedBoxComponent>(entityID);
}

void Details::addTextureComponent()
{
	if (hasComponent(TYPE(typeid(TextureComponent))))
		return;
	std::string path;
	if (FileSystemHelpers::getPathFromFileExplorer(path))
	{
		ECS->loadAsset(entityID, path);
	}
}

void Details::addParticleComponent()
{
	if (hasComponent(TYPE(typeid(ParticleComponent))))
		return;

	if (!hasComponent(TYPE(typeid(TransformComponent))))
		ECS->addComponent<TransformComponent>(entityID);

	ECS->addComponent<ParticleComponent>(entityID);
	ParticleSystem::initMesh(ECS->getComponentManager<ParticleComponent>()->getComponentChecked(entityID), entityID);
}
