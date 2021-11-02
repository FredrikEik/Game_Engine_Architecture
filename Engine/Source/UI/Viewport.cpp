#include "Viewport.h"
#include <imgui.h>
#include "../imgui/Bindings/imgui_impl_glfw.h"
#include "../imgui/Bindings/imgui_impl_opengl3.h"
#include "../ECSManager.h"
#include "../Assets/DefaultAssets.h"
#include "../Systems/TransformSystem.h"
#include "../Systems/CameraSystem.h"
#include "../Systems/CollisionSystem.h"
#include "../Engine/Engine.h"

#include "WorldOutliner.h"
#include "Details.h"

Viewport::Viewport(ECSManager* InECS)
{
	ECS = InECS;
	worldOutliner = new WorldOutliner("World Outliner", InECS);
	details = new Details("Details", InECS);
	playButtonText = "Play";

}

Viewport::~Viewport()
{
	delete worldOutliner;
	delete details;
}

void Viewport::begin(struct GLFWwindow* inWindow, int32 inReservedEntities)
{
	window = inWindow;
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	// Setup Platform/Renderer bindings
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 460 core");
	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	reservedEntities = inReservedEntities;
	worldOutliner->begin(this, reservedEntities);
	details->begin(this, reservedEntities);
}

void Viewport::render()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	ImGui::BeginGroup();

	ImGui::Begin("Demo window");
	if (ImGui::Button(playButtonText.c_str()))
	{
		togglePlay();
	}

	if (ImGui::Button("New Entity"))
	{
		uint32 entity = ECS->newEntity();
		//ECS->loadAsset(entity, DefaultAsset::CUBE);
		//ECS->addComponent<TransformComponent>(entity);
		//ECS->addComponent<AxisAlignedBoxComponent>(entity);
		//CollisionSystem::construct(entity, ECS);
		//std::cout << "Adding entity " << entity << '\n';

	}

	if (ImGui::Button("Destroy selected entity"))
	{
		if(selectedEntity >= reservedEntities)
			ECS->destroyEntity(selectedEntity);
	}
	ImGui::End();

	//ImGui::Begin("TransformWidget");
	////ImGui::BeginChild("TransformWidget");

	//if(!selectedEntity == 0)
	//	ImGui::DragFloat3("Position", &ECS->getComponentManager<TransformComponent>()->getComponent(selectedEntity).transform[3].x, 0.1f, -10000.f, 10000.f);

	////ImGui::EndChild();
	//ImGui::End();
	ImGui::EndGroup();

	ImGui::BeginGroup();
	ImGui::Begin("WorldOutliner");
	worldOutliner->update();
	ImGui::End();
	ImGui::Begin("Details Panel");
	if (selectedEntity >= reservedEntities)
	details->update(selectedEntity);
	ImGui::End();
	ImGui::EndGroup();
	// Render dear imgui into screen
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	worldOutliner->end();
	details->end();
}

void Viewport::end()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void Viewport::setCurrentEntity(int32 entityID)
{
	//std::cout << "Setting entity " << entityID << "\n";
	selectedEntity = entityID;
}

void Viewport::togglePlay()
{
	bIsPlaying = !bIsPlaying;
	playButtonText = bIsPlaying ? std::string("Stop") : std::string("Play");
	
}
