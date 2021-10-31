#pragma once
#include "glm/glm.hpp"
#include "../CoreMinimal.h"

class Engine
{
public:
	Engine();
	~Engine();

	void start();

private:
	void init();
	void loop();
	void terminate();

	static void framebuffer_size_callback(struct GLFWwindow* window, int width, int height);
	void processInput(struct GLFWwindow* window);
	static void mouse_callback(struct GLFWwindow* window, double xpos, double ypos);
	static void scroll_callback(struct GLFWwindow* window, double xoffset, double yoffset);
	static void mouseButton_callback(GLFWwindow* window, int button, int action, int mods);


	// glGetError is very slow, thus should only be used in debug builds
	static void GLClearError();

	// glGetError is very slow, thus should only be used in debug builds
	static bool GLLogCall(const char* function, const char* file, int line);

	struct GLFWwindow* window{};

private:
	// TODO: Move this shit ASAP
	float deltaTime{ 0.0f }; // Time between current frame and last frame
	float lastFrame{ 0.0f }; // Time of last frame

	//fov
	float fov{ 45.0f };

	//TODO: Move these into appropriate classes. 
	class ECSManager* ECS{};
	class Shader* ourShader{};
	class Shader* selectionShader{};

	uint32 editorCameraEntity{};
	class SweepAndPrune* CollisionBroadphaseDatastructure{};
};