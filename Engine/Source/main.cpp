#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Components/BaseComponent.h"
#include "Components/ComponentManager.h"
#include "Factory.h"
#include "Systems/BaseSystem.h"
#include <chrono>
#include <typeindex>

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


int main()
{
	uint32 elements{ 5000000 };
	//ComponentManager<testComponent>& testManager = (*new ComponentManager<testComponent>());
	//for (uint16 j = 0; j < 1; ++j)
	//{
	//	auto start = std::chrono::system_clock::now();
	//	for (uint32 i{}; i < elements; ++i)
	//	{
	//		testManager.createComponent(i);
	//	}
	//	testManager.cleanUp();

	//	auto doneCreating = std::chrono::system_clock::now();
	//	//std::cout << test.entityID << test.ID<<'\n';
	//	for (uint32 i{}; i < elements; ++i)
	//	{
	//		auto comp = testManager.getComponent(i);
	//		comp.pos[1] = 8;
	//	}
	//	auto doneLoopingOneByOne = std::chrono::system_clock::now();

	//	for (auto it : testManager.getComponentArray())
	//		it.pos[0] = 10;
	//	auto doneIteratingArray = std::chrono::system_clock::now();

	//	for (uint32 i{}; i < elements; ++i)
	//	{
	//		testManager.removeComponent(i);
	//	}
	//	auto done = std::chrono::system_clock::now();

	//	std::cout << "Time to create "<<elements<<" components: " << std::chrono::duration_cast<std::chrono::nanoseconds>(doneCreating - start).count() / 1000000000.f << " seconds\n";
	//	std::cout << "Time to loop over " << elements << " components by entityID: " << std::chrono::duration_cast<std::chrono::nanoseconds>(doneLoopingOneByOne - doneCreating).count() / 1000000000.f << " seconds\n";
	//	std::cout << "Time to change a value in all " << elements << " components: " << std::chrono::duration_cast<std::chrono::nanoseconds>(doneIteratingArray - doneLoopingOneByOne).count() / 1000000000.f << " seconds\n";
	//	std::cout << "Time to remove all " << elements << " components: " << std::chrono::duration_cast<std::chrono::nanoseconds>(done - doneIteratingArray).count() / 1000000000.f << " seconds\n";
	//	std::cout << "Total time: " << std::chrono::duration_cast<std::chrono::nanoseconds>(done - start).count() / 1000000000.f << " seconds\n";
	//	//std::cout << "The manager is using " << sizeof() << " bytes\n\n";
	//}
	//delete &testManager;
	//std::cout << "Waiting";

	auto start = std::chrono::system_clock::now();

	Factory &factory = (*new Factory);
	for (uint32 i{}; i < elements; ++i)
	{
		factory.createComponent<testComponent>(i);
	}
	auto doneCreating = std::chrono::system_clock::now();


	TestSystem* system = new TestSystem();
	system->updateAll(factory.getManager<testComponent>());
	auto doneUpdating = std::chrono::system_clock::now();
	std::cout << "Updated variable: " << factory.getManager<testComponent>().getComponent(100).pos[0] << '\n';
	delete system;

	for (uint32 i{}; i < elements; ++i)
	{
		factory.removeComponent<testComponent>(i);
	}
	auto done = std::chrono::system_clock::now();

	std::cout << "Time to create " << elements << " components with factory: " << std::chrono::duration_cast<std::chrono::microseconds>(doneCreating - start).count() / 1000000.f << " seconds\n";
	std::cout << "Time to update " << elements << " components with factory: " << std::chrono::duration_cast<std::chrono::microseconds>(doneUpdating - doneCreating).count() / 1000000.f << " seconds\n";
	std::cout << "Time to remove " << elements << " components with factory: " << std::chrono::duration_cast<std::chrono::microseconds>(done - doneUpdating).count() / 1000000.f << " seconds\n";
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



	while (!glfwWindowShouldClose(window))
	{
		// can be used to calc deltatime
		float currentFrame = glfwGetTime();

		//// input
		processInput(window);



		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		// possible bits we can set are:
			// -GL_COLOR_BUFFER_BIT
			// -GL_DEPTH_BUFFER_BIT
			// -GL_STENCIL_BUFFER_BIT
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//WIREFRAME mode
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);


		glfwPollEvents();
		glfwSwapBuffers(window);
	}


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


