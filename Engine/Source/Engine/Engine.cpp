#include "Engine.h"
#include <iostream>


#include <imgui.h>
#include "../imgui/Bindings/imgui_impl_glfw.h"
#include "../imgui/Bindings/imgui_impl_opengl3.h"


#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

#include "../Shader.h"
#include "../ECSManager.h"
#include "../DataStructures/SweepAndPrune.h"
#include "../UI/Viewport.h"

#include "../Assets/DefaultAssets.h"
#include "../Systems/TransformSystem.h"
#include "../Systems/CameraSystem.h"

#include "../Systems/SelectionSystem.h"
#include "../Systems/CollisionSystem.h"


#include "../Input/Input.h"
#include "../Components/Components.h"


#define ASSERT(x) if (!(x)) __debugbreak();
#ifdef _DEBUG
#define GLCall(x) GLClearError(); x; ASSERT(GLLogCall(__FUNCTION__, __FILE__, __LINE__))
#else
#define GLCall(x) x
#endif

Engine::Engine()
{
	ECS = new ECSManager();
	CollisionBroadphaseDatastructure = new SweepAndPrune(ECS);
	viewport = new Viewport(ECS);
}

Engine::~Engine()
{
	delete CollisionBroadphaseDatastructure;
	delete ECS;
	delete ourShader;
	delete viewport;
}

void Engine::start()
{
	init();
	loop();
	terminate();
}

void Engine::init()
{
	glfwInit();
	glfwWindowHint(GLFW_SAMPLES, 16);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(getWindowWidth(), getWindowHeight(), "Welcome to GameInJin", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);
	
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		exit(EXIT_FAILURE);
	}

	//Note that processed coordinates in OpenGL are between - 1 and 1
	//so we effectively map from the range(-1 to 1) to(0, 800) and (0, 600).
	glViewport(0, 0, getWindowWidth(), getWindowHeight());
	glfwSetFramebufferSizeCallback(window, Engine::framebuffer_size_callback);
	glfwSetCursorPosCallback(window, Engine::mouse_callback);
	glfwSetScrollCallback(window, Engine::scroll_callback);
	glfwSetMouseButtonCallback(window, Engine::mouseButton_callback);
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	glfwSetWindowSizeCallback(window, Engine::windowSize_callback);

	glfwSetCursorPos(window, getWindowWidth() * 0.5, getWindowHeight() * 0.5);


	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);
	glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE); 



	ourShader = new Shader("Shaders/BasicShader.vert", "Shaders/BasicShader.frag");
	selectionShader = new Shader("Shaders/SelectionShader.vert", "Shaders/SelectionShader.frag");
	outlineShader = new Shader("Shaders/OutlineShader.vert", "Shaders/OutlineShader.frag");


	editorCameraEntity = ECS->newEntity();
	ECS->addComponents<CameraComponent, TransformComponent>(editorCameraEntity);

	RTSSelectionEntity = ECS->newEntity();
	/// transform can be used to creat rts selection
	//mesh comp
	// opacity shader
	ECS->addComponents<TransformComponent, SelectionComponent>(RTSSelectionEntity);

	CameraSystem::setPerspective(editorCameraEntity, ECS, 
		glm::perspective(glm::radians(fov), 800.0f / 600.0f, 0.1f, 100.0f));

	// Setup Dear ImGui context
	//IMGUI_CHECKVERSION();
	//ImGui::CreateContext();
	//ImGuiIO& io = ImGui::GetIO();
	//// Setup Platform/Renderer bindings
	//ImGui_ImplGlfw_InitForOpenGL(window, true);
	//ImGui_ImplOpenGL3_Init("#version 460 core");
	//// Setup Dear ImGui style
	//ImGui::StyleColorsDark();
	viewport->begin(window);
}

int EntityToTransform{}; // TODO: VERY TEMP, remove as soon as widgets are implemented
void Engine::loop()
{
	while (!glfwWindowShouldClose(window))
	{

		//// input
		processInput(window);


		// can be used to calc deltatime
		float currentFrame = glfwGetTime();

		// feed inputs to dear imgui, start new frame
		//ImGui_ImplOpenGL3_NewFrame();
		//ImGui_ImplGlfw_NewFrame();
		//ImGui::NewFrame();
		////ImGui::SetWindowSize(ImVec2(400, 400));
		////ImGui::SetNextWindowPos(ImVec2(150, 150));
		//// render your GUI
		////ImGui::DockSpace(0, ImVec2(800.0f, 600.f));
		////ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
		////auto Context = ImGui::CreateContext();
		//
		//ImGui::BeginGroup();
		////ImGui::BeginChild("Demo window");
		//ImGui::Begin("Demo window");
		//if (ImGui::Button("Spawn cube"))
		//{
		//	uint32 entity = ECS->newEntity();
		//	ECS->loadAsset(entity, DefaultAsset::CUBE);
		//	ECS->addComponent<TransformComponent>(entity);
		//	ECS->addComponent<AxisAlignedBoxComponent>(entity);
		//	CollisionSystem::construct(entity, ECS);
		//	std::cout << "Adding entity " << entity << '\n';
	
		//}
		////// TEMP UPDATE
		////ComponentManager<TransformComponent>* mng = ECS->getComponentManager<TransformComponent>();
		////TransformSystem::moveAll(ECS->getComponentManager<TransformComponent>());

		//if (ImGui::Button("Destroy entity 1"))
		//{
		//	ECS->destroyEntity(1);
		//}
		////ImGui::EndChild();
		//ImGui::End();

		//ImGui::Begin("TransformWidget");
		////ImGui::BeginChild("TransformWidget");
		//
		//ImGui::InputInt("Entity", &EntityToTransform, 1, 10);
		//glm::vec3 position = glm::vec3(ECS->getComponentManager<TransformComponent>()->getComponent(EntityToTransform).transform[3]);
		//float test[3]{ position.x, position.y, position.z };
		////ImGui::InputFloat3("Position", test);
		//ImGui::DragFloat3("Position", &ECS->getComponentManager<TransformComponent>()->getComponent(EntityToTransform).transform[3].x, 0.1f, -10000.f, 10000.f);
		////std::cout << "Setting position x: " << test[0] << " y: " << test[1] << " z: " << test[2]<<'\n';

		////TransformSystem::setPosition(EntityToTransform, glm::vec3(test[0], test[1], test[2]), ECS);

		//

		////ImGui::EndChild();
		//ImGui::End();
		//ImGui::EndGroup();
		//ImGui::DestroyContext(Context);
		//{
		//	static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_PassthruCentralNode;

		//	// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		//	// because it would be confusing to have two docking targets within each others.
		//	ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;

		//	ImGuiViewport* viewport = ImGui::GetMainViewport();
		//	ImGui::SetNextWindowPos(viewport->Pos);
		//	ImGui::SetNextWindowSize(viewport->Size);
		//	//ImGui::SetNextWindowViewport(viewport->ID);
		//	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		//	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		//	window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		//	window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;


		//	// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background and handle the pass-thru hole, so we ask Begin() to not render a background.
		//	if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
		//		window_flags |= ImGuiWindowFlags_NoBackground;

		//	// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
		//	// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive, 
		//	// all active windows docked into it will lose their parent and become undocked.
		//	// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise 
		//	// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
		//	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		//	ImGui::Begin("DockSpace", nullptr, window_flags);
		//	ImGui::PopStyleVar();
		//	ImGui::PopStyleVar(2);

		//	// DockSpace
		//	ImGuiIO& io = ImGui::GetIO();
		//	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		//	{
		//		ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
		//		//ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);

		//		static auto first_time = true;
		//		if (first_time)
		//		{
		//			first_time = false;

		//			//ImGui::DockBuilderRemoveNode(dockspace_id); // clear any previous layout
		//			//ImGui::DockBuilderAddNode(dockspace_id, dockspace_flags | ImGuiDockNodeFlags_DockSpace);
		//			//ImGui::DockBuilderSetNodeSize(dockspace_id, viewport->Size);

		//			//// split the dockspace into 2 nodes -- DockBuilderSplitNode takes in the following args in the following order
		//			////   window ID to split, direction, fraction (between 0 and 1), the final two setting let's us choose which id we want (which ever one we DON'T set as NULL, will be returned by the function)
		//			////                                                              out_id_at_dir is the id of the node in the direction we specified earlier, out_id_at_opposite_dir is in the opposite direction
		//			//auto dock_id_left = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Left, 0.2f, nullptr, &dockspace_id);
		//			//auto dock_id_down = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Down, 0.25f, nullptr, &dockspace_id);

		//			//// we now dock our windows into the docking node we made above
		//			//ImGui::DockBuilderDockWindow("Down", dock_id_down);
		//			//ImGui::DockBuilderDockWindow("Left", dock_id_left);
		//			//ImGui::DockBuilderFinish(dockspace_id);
		//		}
		//	}

		//	ImGui::End();

		//	ImGui::Begin("Left");
		//	ImGui::Text("Hello, left!");
		//	ImGui::End();

		//	ImGui::Begin("Down");
		//	ImGui::Text("Hello, down!");
		//	ImGui::End();
		//}

// <<<<<<< HEAD
// =======
// 		ImGui::Begin("TransformWidget");

// 		ImGui::InputInt("Entity", &EntityToTransform, 1, 10);
// 		glm::vec3 position = glm::vec3(ECS->getComponentManager<TransformComponent>()->getComponent(EntityToTransform).transform[3]);
// 		float test[3]{ position.x, position.y, position.z };
// 		//ImGui::InputFloat3("Position", test);
// 		ImGui::DragFloat3("Position", test, 0.1f, -10000.f, 10000.f);
// 		//std::cout << "Setting position x: " << test[0] << " y: " << test[1] << " z: " << test[2]<<'\n';

// 		TransformSystem::setPosition(EntityToTransform, glm::vec3(test[0], test[1], test[2]), ECS);




// 		ImGui::End();
// >>>>>>> Isak_og_Johannes
		CollisionBroadphaseDatastructure->update();


		CameraSystem::updateEditorCamera(editorCameraEntity, ECS, 0.016f);


		//// RENDER
		// Selection Render
		if (Input::getInstance()->getMouseKeyState(KEY_LMB).bPressed)
		{
			glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			CameraSystem::draw(editorCameraEntity, selectionShader, ECS);
			MeshSystem::drawSelectableEditor(selectionShader, "u_model", ECS);


			glFlush();
			glFinish();

			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

			MousePosition mPos = Input::getInstance()->getMousePosition();
			unsigned char data[4];

			glReadPixels(mPos.x, getWindowHeight() - mPos.y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, data);


			// Convert the color back to an integer ID
			int pickedID =
				data[0] +
				data[1] * 256 +
				data[2] * 256 * 256;
			auto& selectionComp = ECS->getComponentManager<SelectionComponent>()->getComponent(RTSSelectionEntity);
			selectionComp.hitEntities.clear();

			if (pickedID == 0x00ffffff)
			{ // Full white, must be the background !

			}
			else
			{

				selectionComp.hitEntities.push_back(pickedID);

				std::ostringstream oss;
				oss << "mesh " << pickedID;
				std::cout << oss.str() << '\n';

				// use single color shader


				// do logic for highlighting object here.
				// flip a bool maybe in meshcomp for being selected, or make a new selectable component
			}
		}


	



		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		CameraSystem::updateEditorCamera(editorCameraEntity, ECS, 0.016f);


		// RTS Selection render -- Translucent -- ingame only
		SelectionSystem::updateSelection(RTSSelectionEntity, editorCameraEntity, ECS, currentFrame);
		SelectionSystem::drawSelectedArea(RTSSelectionEntity, ourShader, ECS);



		glStencilFunc(GL_ALWAYS, 1, 0xFF); // all fragments should pass the stencil test
		glStencilMask(0xFF); // enable writing to the stencil buffer
	
			//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		CameraSystem::draw(editorCameraEntity, ourShader, ECS);
		MeshSystem::draw(ourShader, "u_model", ECS);


		glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
		glStencilMask(0x00); // disable writing to the stencil buffer
		glDisable(GL_DEPTH_TEST);
		CameraSystem::draw(editorCameraEntity, outlineShader, ECS);
		MeshSystem::drawOutline(outlineShader, "u_model", ECS);
		
	
		glStencilMask(0xFF);
		glStencilFunc(GL_ALWAYS, 1, 0xFF);
		glEnable(GL_DEPTH_TEST);


		//// Render dear imgui into screen
		//ImGui::Render();
		//ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		
		viewport->render();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}

void Engine::terminate()
{
	//ImGui_ImplOpenGL3_Shutdown();
	//ImGui_ImplGlfw_Shutdown();
	//ImGui::DestroyContext();
	viewport->end();
	// delete all GLFW's resources that were allocated..
	glfwTerminate();
}

void Engine::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void Engine::processInput(GLFWwindow* window)
{
	Input::getInstance()->updateKeyState(window);
}

void Engine::mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	Input::getInstance()->setMousePosition(window, xpos, ypos);
}

void Engine::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	Input::getInstance()->setScrollState(xoffset, yoffset);
	//fov -= (float)yoffset;
	//if (fov < 1.0f)
	//	fov = 1.0f;
	//if (fov > 45.0f)
	//	fov = 45.0f;
}

void Engine::mouseButton_callback(GLFWwindow* window, int button, int action, int mods)
{

}

void Engine::windowSize_callback(GLFWwindow* window, int x, int y)
{
}

void Engine::GLClearError()
{
	// not equal 0
	while (glGetError() != GL_NO_ERROR);
}

bool Engine::GLLogCall(const char* function, const char* file, int line)
{
	while (GLenum error = glGetError())
	{
		std::cout << "[OpenGL Error] (0x" << std::hex << error << ")" << std::dec << function << " " << file << ", on line: " << line << '\n';
		return false;
	}
	return true;
}
