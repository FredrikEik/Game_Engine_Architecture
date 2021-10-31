#include "Engine.h"
#include <iostream>


#include <imgui.h>
#include "../imgui/bindings/imgui_impl_glfw.h"
#include "../imgui/bindings/imgui_impl_opengl3.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

#include "../Shader.h"
#include "../ECSManager.h"
#include "../DataStructures/SweepAndPrune.h"

#include "../Assets/DefaultAssets.h"
#include "../Systems/TransformSystem.h"
#include "../Systems/CameraSystem.h"
#include "../Systems/CollisionSystem.h"

#include "../Input/Input.h"

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
}

Engine::~Engine()
{
	delete CollisionBroadphaseDatastructure;
	delete ECS;
	delete ourShader;
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

	window = glfwCreateWindow(800, 600, "Welcome to GameInJin", NULL, NULL);
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
	glViewport(0, 0, 800, 600);
	glfwSetFramebufferSizeCallback(window, Engine::framebuffer_size_callback);
	glfwSetCursorPosCallback(window, Engine::mouse_callback);
	glfwSetScrollCallback(window, Engine::scroll_callback);
	glfwSetMouseButtonCallback(window, Engine::mouseButton_callback);
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);


	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);


	ourShader = new Shader("Shaders/BasicShader.vert", "Shaders/BasicShader.frag");
	selectionShader = new Shader("Shaders/SelectionShader.vert", "Shaders/SelectionShader.frag");

	editorCameraEntity = ECS->newEntity();
	ECS->addComponents<CameraComponent, TransformComponent>(editorCameraEntity);
	CameraSystem::setPerspective(editorCameraEntity, ECS, 
		glm::perspective(glm::radians(fov), 800.0f / 600.0f, 0.1f, 100.0f));

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	// Setup Platform/Renderer bindings
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 460 core");
	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
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
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		// render your GUI
		ImGui::Begin("Demo window");
		
		if (ImGui::Button("Spawn cube"))
		{
			uint32 entity = ECS->newEntity();
			ECS->loadAsset(entity, DefaultAsset::CUBE);
			ECS->addComponent<TransformComponent>(entity);
			ECS->addComponent<AxisAlignedBoxComponent>(entity);
			CollisionSystem::construct(entity, ECS);
			std::cout << "Adding entity " << entity << '\n';
	
		}
		//// TEMP UPDATE
		//ComponentManager<TransformComponent>* mng = ECS->getComponentManager<TransformComponent>();
		//TransformSystem::moveAll(ECS->getComponentManager<TransformComponent>());

		if (ImGui::Button("Destroy entity 1"))
		{
			ECS->destroyEntity(1);
		}
		ImGui::End();

		ImGui::Begin("TransformWidget");

		ImGui::InputInt("Entity", &EntityToTransform, 1, 10);
		glm::vec3 position = glm::vec3(ECS->getComponentManager<TransformComponent>()->getComponent(EntityToTransform).transform[3]);
		float test[3]{ position.x, position.y, position.z };
		//ImGui::InputFloat3("Position", test);
		ImGui::DragFloat3("Position", test, 0.1f, -10000.f, 10000.f);
		//std::cout << "Setting position x: " << test[0] << " y: " << test[1] << " z: " << test[2]<<'\n';

		TransformSystem::setPosition(EntityToTransform, glm::vec3(test[0], test[1], test[2]), ECS);



		ImGui::End();
		CollisionBroadphaseDatastructure->update();


		CameraSystem::updateEditorCamera(editorCameraEntity, ECS, 0.016f);

		//// RENDER
		// Selection Render
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT))
		{

			glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			CameraSystem::draw(editorCameraEntity, selectionShader, ECS);
			MeshSystem::drawSelectable(selectionShader, "u_model", ECS);


			glFlush();
			glFinish();

			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

			double xpos, ypos;
			glfwGetCursorPos(window, &xpos, &ypos);
			unsigned char data[4];
			glReadPixels(xpos,ypos, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, data);

			// Convert the color back to an integer ID
			int pickedID =
				data[0] +
				data[1] * 256 +
				data[2] * 256 * 256;

			if (pickedID == 0x00ffffff)
			{ // Full white, must be the background !
				//std::cout << "background" << '\n';
			}
			else
			{
				std::ostringstream oss;
				oss << "mesh " << pickedID;
				std::cout << oss.str() << '\n';
			}

			glfwSwapBuffers(window);
		}
		else
		{
			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			CameraSystem::draw(editorCameraEntity, ourShader, ECS);
			MeshSystem::draw(ourShader, "u_model", ECS);
		}
		



		// Render dear imgui into screen
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		
		
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}

void Engine::terminate()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
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
