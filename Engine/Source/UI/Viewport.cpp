#include "Viewport.h"
//#include <imgui.h>
//#include "../imgui/Bindings/imgui_impl_glfw.h"
//#include "../imgui/Bindings/imgui_impl_opengl3.h"

#include "../imgui/docking/imgui.h"
#include "../imgui/docking/imgui_impl_opengl3.h"
#include "../imgui/docking/imgui_impl_glfw.h"

#include "../ECSManager.h"
#include "../Assets/DefaultAssets.h"
#include "../Systems/TransformSystem.h"
#include "../Systems/CameraSystem.h"
#include "../Systems/CollisionSystem.h"
#include "../Systems/SphereSystem.h"
#include "../Engine/Engine.h"

#include "WorldOutliner.h"
#include "Details.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
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
	//ImGuiIO& io = ImGui::GetIO();
	// Setup Platform/Renderer bindings

	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows

	//io.ConfigViewportsNoAutoMerge = true;
	//io.ConfigViewportsNoTaskBarIcon = true;

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();

	// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 460 core");
	// Setup Dear ImGui style
	//ImGui::StyleColorsDark();
	reservedEntities = inReservedEntities;
	//static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_PassthruCentralNode;
	//ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;













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
	if (!bIsPlaying)
	{
		if (ImGui::Button("New Entity"))
		{
			uint32 entity = ECS->newEntity();
			ECS->loadAsset(entity, "Assets/suzanne.obj");
			MeshComponent* meshComp = ECS->getComponentManager<MeshComponent>()->getComponentChecked(entity);
			if(!MeshSystem::loadMeshLOD("Assets/suzanne_L01.obj", *meshComp, LODMeshType::LOD1))
			{ }
			if(!MeshSystem::loadMeshLOD("Assets/suzanne_L02.obj", *meshComp, LODMeshType::LOD2))
			{ }
			ECS->addComponent<TransformComponent>(entity);
			ECS->addComponent<AxisAlignedBoxComponent>(entity);
			ECS->addComponent<SphereComponent>(entity);
			CollisionSystem::construct(entity, ECS);
			SphereSystem::construct(entity, ECS);
			
			std::cout << "Adding entity " << entity << '\n';

		}

		if(selectedEntity >= reservedEntities && ImGui::Button("Destroy selected entity"))

		{
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
	}
	else
	{
		ImGui::End();
		ImGui::EndGroup();
	}


	ImGui::Render();
	int display_w, display_h;
	glfwGetFramebufferSize(window, &display_w, &display_h);
	glViewport(0, 0, display_w, display_h);
	//glClearColor(clear_color.x* clear_color.w, clear_color.y* clear_color.w, clear_color.z* clear_color.w, clear_color.w);
	//glClear(GL_COLOR_BUFFER_BIT);
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	// Update and Render additional Platform Windows
	// (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
	//  For this specific demo app we could also call glfwMakeContextCurrent(window) directly)
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		GLFWwindow* backup_current_context = glfwGetCurrentContext();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		glfwMakeContextCurrent(backup_current_context);
	}



	//ImGui::Render();
	//ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	//worldOutliner->end();
	//details->end();
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
	//Engine::Get().setIsPlaying(bIsPlaying);

	Engine::GetInstance()->setIsPlaying(bIsPlaying);
}
