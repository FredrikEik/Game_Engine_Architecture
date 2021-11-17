#include "Engine.h"
#include <iostream>


//#include <imgui.h>
//#include "../imgui/Bindings/imgui_impl_glfw.h"
//#include "../imgui/Bindings/imgui_impl_opengl3.h"


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
#include "AL/al.h"

#include "../imgui/docking/imgui.h"
#include "../imgui/docking/imgui_impl_opengl3.h"
#include "../imgui/docking/imgui_impl_glfw.h"

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

void Engine::setIsPlaying(bool isPlaying)
{
	bIsPlaying = isPlaying;
	//std::cout << "bIsPlaying: " << bIsPlaying;
}

Engine* Engine::instance = nullptr;
float Engine::windowWidth = 800.f;
float Engine::windowHeight = 600.f;
float Engine::fov = 45.f;	

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
	glfwSwapInterval(1); // VSYNC turn off for more fps 
	
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
	phongShader = new Shader("Shaders/PhongShader.vert", "Shaders/PhongShader.frag");
	selectionShader = new Shader("Shaders/SelectionShader.vert", "Shaders/SelectionShader.frag");
	outlineShader = new Shader("Shaders/OutlineShader.vert", "Shaders/OutlineShader.frag");


	editorCameraEntity = ECS->newEntity();
	ECS->addComponents<CameraComponent, TransformComponent>(editorCameraEntity);

	RTSSelectionEntity = ECS->newEntity();
	/// transform can be used to creat rts selection
	//mesh comp
	// opacity shader
	ECS->addComponents<TransformComponent, SelectionComponent>(RTSSelectionEntity);

	//CameraSystem::setPerspective(editorCameraEntity, ECS, 
	//	glm::radians(fov), windowWidth / windowWidth, 0.1f, 100.0f));

	// Setup Dear ImGui context
	//IMGUI_CHECKVERSION();
	//ImGui::CreateContext();
	//ImGuiIO& io = ImGui::GetIO();
	//// Setup Platform/Renderer bindings
	//ImGui_ImplGlfw_InitForOpenGL(window, true);
	//ImGui_ImplOpenGL3_Init("#version 460 core");
	//// Setup Dear ImGui style
	//ImGui::StyleColorsDark();
	viewport->begin(window, ECS->getNumberOfEntities());
}

int EntityToTransform{}; // TODO: VERY TEMP, remove as soon as widgets are implemented
void Engine::loop()
{
	while (!glfwWindowShouldClose(window))
	{
		//// input
		processInput(window);

		// TODO: Make this not happen every frame
		CameraSystem::setPerspective(editorCameraEntity, ECS, fov, windowWidth / windowHeight, 0.1f, 100.0f);
		// can be used to calc deltatime
		float currentFrame = glfwGetTime();

		CollisionBroadphaseDatastructure->update();


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
				if (core::MAX_ENTITIES < pickedID)
					return;

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

		if (bIsPlaying)
		{
			//std::cout << "Game camera'\n";
			// TODO: Implement proper deltatime
			CameraSystem::updateGameCamera(editorCameraEntity, ECS, 0.016f);
		}
		else
		{
			//std::cout << "Editor camera'\n";
			//TODO: Draw a game camera here
			CameraSystem::updateEditorCamera(editorCameraEntity, ECS, 0.016f);
		}


		// RTS Selection render -- Translucent -- ingame only
		SelectionSystem::updateSelection(RTSSelectionEntity, editorCameraEntity, ECS, currentFrame);
		//SelectionSystem::drawSelectedArea(RTSSelectionEntity, ourShader, ECS);
		SelectionSystem::drawSelectedArea(RTSSelectionEntity, ourShader, ECS);



		glStencilFunc(GL_ALWAYS, 1, 0xFF); // all fragments should pass the stencil test
		glStencilMask(0xFF); // enable writing to the stencil buffer
	
			//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		//CameraSystem::draw(editorCameraEntity, ourShader, ECS);
		CameraSystem::draw(editorCameraEntity, phongShader, ECS);
		CameraSystem::setPhongUniforms(editorCameraEntity, phongShader, ECS);
		//MeshSystem::draw(ourShader, "u_model", ECS, editorCameraEntity);
		MeshSystem::draw(phongShader, "u_model", ECS, editorCameraEntity);


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
	windowWidth = x;
	windowHeight = y;
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
