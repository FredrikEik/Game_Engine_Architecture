#include "ScriptWidget.h"
#include "../ECSManager.h"
#include "../imgui/docking/imgui.h"
#include "../imgui/docking/imgui_impl_opengl3.h"
#include "../imgui/docking/imgui_impl_glfw.h"
#include "../Systems/ScriptSystem.h"
ScriptWidget::ScriptWidget(std::string inWindowName, ECSManager* inECS)
	:Window(inWindowName, inECS)
{
}
void ScriptWidget::begin(Viewport* inViewport, int32 inReservedEntities)
{
	Window::begin(inViewport, inReservedEntities);

}

void ScriptWidget::update(int32 entityID, bool& entitiesChanged)
{
	ScriptComponent* component = ECS->getComponentManager<ScriptComponent>()->getComponentChecked(entityID);
	if (component)
	{
		bool newClassSelected = false;
		std::string selectedClass = drawScriptSelector(entityID, component->ScriptClassName, newClassSelected);

		if (newClassSelected)
		{
			component->bInitialized = false;
			ScriptSystem::InitScriptObject(component, selectedClass);
			std::cout << "INITING SCRIPT IN WIDGET\n";
		}
		
	}
	
}

void ScriptWidget::end()
{
	Window::end();
}

std::string ScriptWidget::drawScriptSelector(uint32 entity, std::string currentClass, bool& OUTnewClassSelected)
{
	const char* items[] = {
	unit.c_str(),
	gameMode.c_str(),
	ball.c_str()
	};
	static const char* currentScript = currentClass.c_str();
	static std::string script = currentScript;
	currentScript = script.c_str(); // Hack to avoid the pointer jumping around in memory

	ImGuiComboFlags flags = ImGuiComboFlags_NoArrowButton;
	OUTnewClassSelected = false;
	ImGuiStyle& style = ImGui::GetStyle();

	float w = ImGui::CalcItemWidth();
	float spacing = style.ItemInnerSpacing.x;
	float button_sz = ImGui::GetFrameHeight();
	ImGui::PushItemWidth(w - spacing * 2.0f - button_sz * 2.0f);
	if (ImGui::BeginCombo("##custom script combo", currentScript, ImGuiComboFlags_NoArrowButton))
	{
		//std::cout << currentScript << "\n";

		for (int n = 0; n < IM_ARRAYSIZE(items); n++)
		{
			bool is_selected = (currentScript == items[n]);
			if (ImGui::Selectable(items[n], is_selected))
			{
				script = items[n];
				OUTnewClassSelected = true;
			}
			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}
	return currentScript;
}
