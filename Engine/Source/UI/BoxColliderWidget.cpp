#include "BoxColliderWidget.h"
#include "../imgui/docking/imgui.h"
#include "../imgui/docking/imgui_impl_opengl3.h"
#include "../imgui/docking/imgui_impl_glfw.h"
#include "../ECSManager.h"
#include "../FileSystemHelpers.h"
#include "../Systems/CollisionSystem.h"
BoxColliderWidget::BoxColliderWidget(std::string inWindowName, ECSManager* inECS)
	:Window(inWindowName, inECS)
{
}

void BoxColliderWidget::begin(Viewport* inViewport, int32 inReservedEntities)
{
	Window::begin(inViewport, inReservedEntities);
}

void BoxColliderWidget::update(int32 entityID, bool& entitiesChanged)
{
	if (entityID >= reservedEntities)
	{
		entitiesChanged = false;
		AxisAlignedBoxComponent* component = 
					ECS->getComponentManager<AxisAlignedBoxComponent>()->getComponentChecked(entityID);
		assert(component);
		ImGui::Text("Box Collider");
		ImGui::DragFloat3("Min", &component->minScaled[0], 0.1f);
		ImGui::DragFloat3("Max", &component->maxScaled[0], 0.1f);
		ImGui::Checkbox("Generate Overlap Events", &component->bShouldGenerateOverlapEvents);
		if (ImGui::Button("Fit To Mesh"))
		{
			CollisionSystem::construct(entityID, ECS, component->bShouldGenerateOverlapEvents);
		}

		//if (ImGui::Button("Load Texture"))
		//{
		//	std::string path;
		//	if (FileSystemHelpers::getPathFromFileExplorer(path))
		//	{
		//		ECS->removeComponent<TextureComponent>(entityID);
		//		ECS->loadAsset(entityID, path);
		//		entitiesChanged = true;
		//	}
		//}
	}
}

void BoxColliderWidget::end()
{
	Window::end();
}
