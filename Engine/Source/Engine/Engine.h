#pragma once
#include "glm/glm.hpp"
#include "../CoreMinimal.h"
#include <iostream>
//#define DEBUG
//#define DEBUG_BROADPHASE
#include "mono/jit/jit.h"
//#include <mono/metadata/assembly.h>
#include <vector>
class Engine
{
public:
	static Engine& Get()
	{
		static Engine* engineInst = new Engine();
		return *engineInst;
	}

	void start();
	~Engine();
	// TODO: Make this a callback func instead of public
	void setIsPlaying(bool isPlaying);
	void save();
	void load(const std::string& path);

	static uint32 createDefaultEntity_Internal(MonoString* path);
private:
	static Engine* instance;
	Engine();

	void init();
	void initGLFW();
	void initOpenGL();
	void compileShaders();
	void initEtities();
	void loop();
	void terminate();
	void editorSelection();

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
	class Shader* particleShader{};
	class Shader* hudShader{};

	class Shader* GeometryPassShader{};
	class Shader* LightPassShader{};

	class Shader* ShadowShader{};
	class Shader* ShadowDepthShader{};

	class Shader* matShader{};

	uint32 editorCameraEntity{};
	uint32 gameCameraEntity{};
	uint32 cameraEntity{};
	uint32 SystemEntity{};
	uint32 terrainEntity{};
	uint32 unitEntity{};
	//uint32 cameraEntity{};
	uint32 gameStateEntity{};

	uint32 dogEntity{};
	uint32 lightEnitites[25]{};


	uint32 reservedEntities{};

	static float windowWidth;
	static float windowHeight;

	class SweepAndPrune* CollisionBroadphaseDatastructure{};

	class Viewport* viewport;

	bool bIsPlaying{ false };

	std::vector<std::pair<uint32, std::string>> pendingEntities;
public:
	ECSManager* getECSManager() const { return ECS; }
	float getWindowWidth() const { return windowWidth; }
	void setWindowWidth(float val) { windowWidth = val; }
	float getWindowHeight() const { return windowHeight; }
	void setWindowHeight(float val) { windowHeight = val; }
	uint32 getCameraEntity() const { return cameraEntity; }
	bool getIsPlaying() const { return bIsPlaying; }
	SweepAndPrune* getCollisionDatastructure() { return CollisionBroadphaseDatastructure; }

	static float getDeltaTime_Internal();
};