#include "TransformWidget.h"
#include <imgui.h>
#include "../imgui/Bindings/imgui_impl_glfw.h"
#include "../imgui/Bindings/imgui_impl_opengl3.h"
#include "../ECSManager.h"
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtx/transform.hpp>
TransformWidget::TransformWidget(std::string inWindowName, ECSManager* inECS)
	:Window(inWindowName, inECS)
{
}

void TransformWidget::begin(Viewport* inViewport, int32 inReservedEntities)
{
	Window::begin(inViewport, inReservedEntities);
}

void TransformWidget::update(int32 entityID)
{
	//ImGui::Begin("TransformWidget");
	//ImGui::BeginChild("TransformWidget");

	if (entityID >= reservedEntities)
	{
		glm::mat4x4& transform = ECS->getComponentManager<TransformComponent>()->getComponent(entityID).transform;
		ImGui::DragFloat3("Position", &transform[3].x, 0.1f, -10000.f, 10000.f);
		float scale[3] = { transform[0].x, transform[1].y, transform[2].z };
		ImGui::DragFloat3("Scale", &scale[0], 0.1f);
		transform[0].x = scale[0];
		transform[1].y = scale[1];
		transform[2].z = scale[2];
	}

	//ImGui::EndChild();
	//ImGui::End();
}

void TransformWidget::end()
{
}
