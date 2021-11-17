#pragma once
#include "glm/glm.hpp"
#include "../CoreMinimal.h"

//#define DEBUG
//#define DEBUG_BROADPHASE

class Engine
{
public:
	// TODO: Remove or fix up the Get function
	static Engine& Get()
	{
		static Engine* engineInst = new Engine();
		return *engineInst;
	}
	static Engine* GetInstance()
	{
		if (!instance)
			instance = new Engine();
		return instance;
	}

	void start();
	~Engine();
	// TODO: Make this a callback func instead of public
	void setIsPlaying(bool isPlaying);
private:
	static Engine* instance;
	Engine();

	void init();
	void loop();
	void terminate();

	static void framebuffer_size_callback(struct GLFWwindow* window, int width, int height);
	void processInput(struct GLFWwindow* window);
	static void mouse_callback(struct GLFWwindow* window, double xpos, double ypos);
	static void scroll_callback(struct GLFWwindow* window, double xoffset, double yoffset);
	static void mouseButton_callback(GLFWwindow* window, int button, int action, int mods);
	static void windowSize_callback(GLFWwindow* window, int x, int y);

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
	static float fov;

	//TODO: Move these into appropriate classes. 
	class ECSManager* ECS{};
	class Shader* ourShader{};
	class Shader* phongShader{};
	class Shader* selectionShader{};
	class Shader* outlineShader{};

	uint32 editorCameraEntity{};

	uint32 RTSSelectionEntity{};


	static float windowWidth;
	static float windowHeight;

	class SweepAndPrune* CollisionBroadphaseDatastructure{};

	class Viewport* viewport;

	bool bIsPlaying{ false };
public:
	float getWindowWidth() const { return windowWidth; }
	void setWindowWidth(float val) { windowWidth = val; }
	float getWindowHeight() const { return windowHeight; }
	void setWindowHeight(float val) { windowHeight = val; }


};