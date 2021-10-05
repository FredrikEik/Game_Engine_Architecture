#include <imgui.h>
#include "imgui/bindings/imgui_impl_glfw.h"
#include "imgui/bindings/imgui_impl_opengl3.h"


#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Components/Components.h"
#include "Components/ComponentManager.h"
#include "Factory.h"
#include "Systems/BaseSystem.h"
#include "ECSManager.h"
#include "Assets/DefaultAssets.h"
#include "Systems/TransformSystem.h"
#include <chrono>
#include <typeindex>
#include "Shader.h"
#include "DataStructures/TArray.h"

#include "Engine/Engine.h"


#define ASSERT(x) if (!(x)) __debugbreak();
#ifdef _DEBUG
#define GLCall(x) GLClearError(); x; ASSERT(GLLogCall(__FUNCTION__, __FILE__, __LINE__))
#else
#define GLCall(x) x
#endif

// glGetError is very slow, thus should only be used in debug builds
static void GLClearError()
{
	// not equal 0
	while (glGetError() != GL_NO_ERROR);
}

// glGetError is very slow, thus should only be used in debug builds
static bool GLLogCall(const char* function, const char* file, int line)
{
	while (GLenum error = glGetError())
	{
		std::cout << "[OpenGL Error] (0x" << std::hex << error << ")" << std::dec << function << " " << file << ", on line: " << line << '\n';
		return false;
	}
	return true;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);


glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 2.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

float yaw{ -90.0f };
float pitch{ 0.0f };

float deltaTime{ 0.0f }; // Time between current frame and last frame
float lastFrame{ 0.0f }; // Time of last frame

//mouse
float lastX{ 400 };
float lastY{ 300 };
bool firstMouse{ false };
bool shouldCaptureMouse{ false };

//fov
float fov{ 45.0f };


int main()
{
	Engine* engine = new Engine();
	engine->start();
	delete engine;

	return 0;
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "Welcome to GameInJin", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	//Note that processed coordinates in OpenGL are between - 1 and 1
	//so we effectively map from the range(-1 to 1) to(0, 800) and (0, 600).
	glViewport(0, 0, 800, 600);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);


	glm::mat4 projection;
	glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 cameraDirection = glm::normalize(cameraPos - cameraTarget);

	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));
	glm::vec3 cameraUp = glm::cross(cameraDirection, cameraRight);

	//look at matrix
	glm::mat4 view;
	view = glm::lookAt(cameraPos, cameraTarget, up);

	
	Shader* ourShader = new Shader("Shaders/BasicShader.vert", "Shaders/BasicShader.frag");
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	// Setup Platform/Renderer bindings
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 460 core");
	// Setup Dear ImGui style
	ImGui::StyleColorsDark();


	ECSManager* ECS = new ECSManager();


	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		// can be used to calc deltatime
		float currentFrame = glfwGetTime();

		projection = glm::perspective(glm::radians(fov), 800.0f / 600.0f, 0.1f, 100.0f);
		view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);


		//// input
		processInput(window);
		// feed inputs to dear imgui, start new frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();



		//// RENDER
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		// render your GUI

		ImGui::Begin("Demo window");
		if (ImGui::Button("Spawn cube"))
		{
			uint32 entity = ECS->newEntity();
			ECS->loadAsset(entity, DefaultAsset::CUBE);
			ECS->addComponent<TransformComponent>(entity);
			std::cout << "Adding entity "<<entity<<'\n';
		}
		
		//// TEMP UPDATE
		//ComponentManager<TransformComponent>* mng = ECS->getComponentManager<TransformComponent>();
		TransformSystem::moveAll(ECS->getComponentManager<TransformComponent>());


		if (ImGui::Button("Destroy entity 0"))
		{
			ECS->destroyEntity(0);
		}

		ImGui::End();

		MeshSystem::draw(ourShader, "u_model", ECS);
		//ourShader.setMat4("u_model", model);
		ourShader->setMat4("u_view", view);
		ourShader->setMat4("u_projection", projection);


		// Render dear imgui into screen
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


		glfwSwapBuffers(window);
	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	// delete all GLFW's resources that were allocated..
	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	
	const float cameraSpeed = 200000000.5f * deltaTime; // adjust accordingly
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
		shouldCaptureMouse = true;
		//std::cout << "Holding rmb\n";
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			cameraPos += cameraSpeed * cameraFront;
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			cameraPos -= cameraSpeed * cameraFront;
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
		if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		{
		}
		if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		{
		}
	}
	else
	{
		shouldCaptureMouse = false;
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (shouldCaptureMouse)
	{
		if (firstMouse)
		{
			lastX = xpos;
			lastY = ypos;
			firstMouse = false;
		}

		float xoffset = xpos - lastX;
		float yoffset = lastY - ypos; // reversed: y ranges bottom to top
		lastX = xpos;
		lastY = ypos;

		const float sensitivity = 0.1f;
		xoffset *= sensitivity;
		yoffset *= sensitivity;

		yaw += xoffset;
		pitch += yoffset;

		if (pitch > 89.0f)
			pitch = 89.0f;
		if (pitch < -89.0f)
			pitch = -89.0f;

		glm::vec3 direction;
		direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		direction.y = sin(glm::radians(pitch));
		direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
		cameraFront = glm::normalize(direction);
	}
	else
	{
		firstMouse = true;
	}
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	fov -= (float)yoffset;
	if (fov < 1.0f)
		fov = 1.0f;
	if (fov > 45.0f)
		fov = 45.0f;
}


