#include "TextureWidget.h"
#include "../imgui/docking/imgui.h"
#include "../imgui/docking/imgui_impl_opengl3.h"
#include "../imgui/docking/imgui_impl_glfw.h"
#include "../ECSManager.h"
#include "../FileSystemHelpers.h"
TextureWidget::TextureWidget(std::string inWindowName, ECSManager* inECS)
	:Window(inWindowName, inECS)
{
}

void TextureWidget::begin(Viewport* inViewport, int32 inReservedEntities)
{
	Window::begin(inViewport, inReservedEntities);
}

void TextureWidget::update(int32 entityID, bool& entitiesChanged)
{
	if (entityID >= reservedEntities)
	{
		entitiesChanged = false;
		TextureComponent* component = ECS->getComponentManager<TextureComponent>()->getComponentChecked(entityID);
		assert(component);
		ImGui::Text("Texture");
		if (ImGui::Button("Load Texture"))
		{
			std::string path;
			if (FileSystemHelpers::getPathFromFileExplorer(path))
			{
				ECS->removeComponent<TextureComponent>(entityID);
				ECS->loadAsset(entityID, path);
				entitiesChanged = true;
			}
		}
	}
}

void TextureWidget::end()
{
	Window::end();
}

