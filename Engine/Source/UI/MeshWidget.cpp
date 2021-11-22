#include "MeshWidget.h"
#include "../imgui/docking/imgui.h"
#include "../imgui/docking/imgui_impl_opengl3.h"
#include "../imgui/docking/imgui_impl_glfw.h"
#include "../ECSManager.h"
#include "../FileSystemHelpers.h"
MeshWidget::MeshWidget(std::string inWindowName, ECSManager* inECS)
	:Window(inWindowName, inECS)
{

}

void MeshWidget::begin(Viewport* inViewport, int32 inReservedEntities)
{
	Window::begin(inViewport, inReservedEntities);
}

void MeshWidget::update(int32 entityID, bool& entitiesChanged)
{
	if (entityID >= reservedEntities)
	{
		entitiesChanged = false;
		MeshComponent* mesh = ECS->getComponentManager<MeshComponent>()->getComponentChecked(entityID);
		assert(mesh);
		ImGui::Text("Mesh");
		ImGui::Checkbox("Should Render", &mesh->bShouldRender);
		ImGui::Checkbox("Ignored during frustum culling", &mesh->bDisregardedDuringFrustumCulling);
		if (ImGui::Button("Load Mesh"))
		{
			std::string path;
			if (FileSystemHelpers::getPathFromFileExplorer(path))
			{
				bool bShouldRender = mesh->bShouldRender;
				bool bDisregardedDuringFrustumCulling = mesh->bDisregardedDuringFrustumCulling;
				ECS->removeComponent<MeshComponent>(entityID);
				ECS->loadAsset(entityID, path);
				mesh = ECS->getComponentManager<MeshComponent>()->getComponentChecked(entityID);
				mesh->bDisregardedDuringFrustumCulling = bDisregardedDuringFrustumCulling;
				mesh->bShouldRender = bShouldRender;
				entitiesChanged = true;
			}
		}
	}
}

void MeshWidget::end()
{
	Window::end();
}
