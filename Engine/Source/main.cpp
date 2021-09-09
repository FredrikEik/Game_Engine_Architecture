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
#include <chrono>
#include <typeindex>
#include "Shader.h"





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

//template <typename... Ts>
//void swallow(Ts&&...) {
//}
//
//template <typename T>
//int create(int i) {
//	std::cout << typeid(T).name()<<" id: " << i << '\n';
//	return 0;
//}
//
//template<typename... Ts>
//void createObject(int i) {
//	swallow(create<Ts>(i)...);
//}
int main()
{

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



	Shader ourShader = Shader("basicShader.vert", "basicShader.frag");
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	// Setup Platform/Renderer bindings
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 460 core");
	// Setup Dear ImGui style
	ImGui::StyleColorsDark();

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		// can be used to calc deltatime
		float currentFrame = glfwGetTime();




		//// input
		processInput(window);
		// feed inputs to dear imgui, start new frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();


		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		// possible bits we can set are:
			// -GL_COLOR_BUFFER_BIT
			// -GL_DEPTH_BUFFER_BIT
			// -GL_STENCIL_BUFFER_BIT
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//WIREFRAME mode
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		// render your GUI
		ImGui::Begin("Demo window");
		ImGui::Button("Hello!");
		ImGui::End();

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


}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
}


