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

#include "../Systems/ScriptSystem.h"
#include "../Systems/LightSystem.h"

#include "../Systems/ParticleSystem.h"
#include "../Systems/TerrainSystem.h"
#include "../Systems/PhysicsSystem.h"
#include "../Systems/HudSystem.h"


#include "../SaveLoad/Save.h"
#include "../SaveLoad/Load.h"

#include <thread>



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

	delete phongShader;
	delete selectionShader;
	delete outlineShader;
	delete particleShader;
	delete hudShader;
}

void Engine::setIsPlaying(bool isPlaying)
{
	MeshSystem::setHiddenInGame(gameCameraEntity, ECS, isPlaying);
	if (!isPlaying)
	{
		load(Save::getDefaultAbsolutePath());
		TransformSystem::setPosition(gameCameraEntity, glm::vec3(70, 0, 140), ECS);
		CameraSystem::updateGameCamera(gameCameraEntity, ECS, 0.016);
		ScriptSystem::uninitializeAllComponents(ECS);
		ECS->removeComponent<ScriptComponent>(gameStateEntity);
		ECS->addComponent<ScriptComponent>(gameStateEntity);
		ScriptSystem::InitScriptObject(ECS->getComponentManager<ScriptComponent>()->getComponentChecked(gameStateEntity), "GameMode");

	}
	else
	{
		//save();
		// First invoking on the game state because it created new entities.
		// This will cause the components to shift around in memory, so it cannot be done
		// while looping through all components
		ScriptSystem::Invoke(gameStateEntity, "BeginPlay", ECS);
		ScriptSystem::Invoke("BeginPlay", ECS);
	}
	bIsPlaying = isPlaying;
}

void Engine::save()
{
	Save::saveEntities(ECS->entities, reservedEntities, ECS); 
}

void Engine::load(const std::string& path)
{
	for (uint32 i{ reservedEntities }; i < core::MAX_ENTITIES; ++i)
	{
		ECS->destroyEntity(i);
	}
	Load::loadEntities(path, ECS);
}

uint32 Engine::createDefaultEntity_Internal(MonoString* path)
{
	//uint32 entity = Get().ECS->newEntity();
	//Get().pendingEntities.push_back(std::make_pair(entity, mono_string_to_utf8(path)));
	//return entity;
	return Load::loadEntity(Get().ECS->newEntity(), mono_string_to_utf8(path), Get().ECS);
}

Engine* Engine::instance = nullptr;
float Engine::windowWidth = 1400.f;
float Engine::windowHeight = 1000.f;
float Engine::fov = 45.f;	

//sa
void Engine::start()
{
	init();
	loop();

	terminate();
}

void Engine::initGLFW()
{
	glfwInit();
	glfwWindowHint(GLFW_SAMPLES, 4);
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
}

void Engine::initOpenGL()
{	
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);	
}

void Engine::compileShaders()
{
	ourShader = new Shader("Shaders/BasicShader.vert", "Shaders/BasicShader.frag");
	phongShader = new Shader("Shaders/PhongShader.vert", "Shaders/PhongShader.frag");
	selectionShader = new Shader("Shaders/SelectionShader.vert", "Shaders/SelectionShader.frag");
	outlineShader = new Shader("Shaders/OutlineShader.vert", "Shaders/OutlineShader.frag");
	particleShader = new Shader("Shaders/ParticleShader.vert", "Shaders/ParticleShader.frag");
	hudShader = new Shader("Shaders/HudShader.vert", "Shaders/HudShader.frag");

	GeometryPassShader = new Shader("Shaders/G_Buffer.vert", "Shaders/G_Buffer.frag");
	LightPassShader = new Shader("Shaders/DefS_LightsShadows.vert", "Shaders/DefS_LightsShadows.frag");

	ShadowShader = new Shader("Shaders/P_Shadows.vert", "Shaders/P_Shadows.frag");
	ShadowDepthShader = new Shader("Shaders/P_ShadowsDepth.vert", "Shaders/P_ShadowsDepth.frag", "Shaders/P_ShadowsDepth.geo");
	
	matShader = new Shader("Shaders/debugMatShader.vert", "Shaders/debugMatShader.frag");
}

void Engine::initEtities()
{
	editorCameraEntity = ECS->newEntity();
	ECS->addComponents<CameraComponent, TransformComponent>(editorCameraEntity);
	gameCameraEntity = ECS->newEntity();
	ECS->addComponents<CameraComponent, TransformComponent>(gameCameraEntity);
	CameraSystem::setPerspective(gameCameraEntity, ECS, fov, windowWidth / windowHeight, 0.1f, 30.0f);
	CameraSystem::updateGameCamera(gameCameraEntity, ECS, 0.016f);
	CameraSystem::createFrustumMesh(gameCameraEntity, ECS);
	TransformSystem::move(gameCameraEntity, glm::vec3(70, 0, 140), ECS);
	TransformSystem::move(editorCameraEntity, glm::vec3(40, 15, 145), ECS);

	SystemEntity = ECS->newEntity();
	ECS->addComponents<TransformComponent, SelectionComponent, GBufferComponent, ShadowBufferComponent>(SystemEntity);
	auto gBufferComp = ECS->getComponentManager<GBufferComponent>()->getComponentChecked(SystemEntity);
	//LightSystem::InitGBuffer(gBufferComp);
	auto sBufferComp = ECS->getComponentManager<ShadowBufferComponent>()->getComponentChecked(SystemEntity);
	LightSystem::InitSBuffer(sBufferComp);

	terrainEntity = ECS->newEntity();
	ECS->addComponents<TransformComponent, MeshComponent, testComponent>(terrainEntity);
	TerrainSystem::generateRegularGrid(terrainEntity, ECS);
	ECS->loadAsset(terrainEntity, "Assets/grass.png");


	unitEntity = ECS->newEntity();
	ECS->addComponents<TransformComponent, ScriptComponent, MeshComponent, AxisAlignedBoxComponent>(unitEntity);
	ECS->loadAsset(unitEntity, "Assets/suzanne.obj");
	ScriptSystem::InitScriptObject(ECS->getComponentManager<ScriptComponent>()->getComponentChecked(unitEntity));
	ScriptSystem::Invoke("BeginPlay", ECS);

	

	dogEntity = ECS->newEntity();
	ECS->addComponents<TransformComponent, MeshComponent, MaterialComponent>(dogEntity);
	ECS->loadAsset(dogEntity, "Assets/Dogling.obj");
	//TransformSystem::setScale(dogEntity, glm::vec3(10, 10, 10), ECS);
	MeshSystem::setConsideredForFrustumCulling(dogEntity, ECS, false);
	//MeshSystem::setIsDeferredDraw(dogEntity, ECS, true);
	auto gunMaterial = ECS->getComponentManager<MaterialComponent>()->getComponentChecked(dogEntity);
	std::map<std::string, std::string> materialMap;
	materialMap.insert(std::make_pair("u_tex_diffuse1", "Assets/Dogling_D.png"));
	materialMap.insert(std::make_pair("u_tex_specular1", "Assets/Dogling_S.png"));
	TextureSystem::loadMaterial(gunMaterial, materialMap);
	ECS->addComponent<HudComponent>(gameCameraEntity);
	HudSystem::init(gameCameraEntity, ECS, "Assets/ballChase.png");
	HudSystem::setPosition(gameCameraEntity, glm::vec2(-0.65, 0.65), ECS);
	HudSystem::setScale(gameCameraEntity, glm::vec2(0.5, 0.5), ECS);
	//ScriptTest
	//Init - binds all internal functions - Important first step


	int i = 1;
	for (auto& light : lightEnitites)
	{
		light = ECS->newEntity();
		ECS->addComponents<TransformComponent, MeshComponent, LightComponent>(light);
		ECS->loadAsset(light, DefaultAsset::CUBE);
		MeshSystem::setConsideredForFrustumCulling(light, ECS, false);
		
		TransformSystem::move(light, glm::vec3(i, 1, i), ECS);
		LightSystem::SetLightValues(light, ECS, glm::vec3(1 / i, 0.8 / i, 0.6 / i), i, i % 5);
		i++;
	}

}

void Engine::init()
{	
	initGLFW();
	initOpenGL();
	compileShaders();
	ScriptSystem::Init();
	initEtities();

	CollisionSystem::setShouldGenerateOverlapEvents(unitEntity, ECS, false);

	gameStateEntity = ECS->newEntity();
	ECS->addComponent<ScriptComponent>(gameStateEntity);
	ScriptSystem::InitScriptObject(ECS->getComponentManager<ScriptComponent>()->getComponentChecked(gameStateEntity), "GameMode");


	reservedEntities = ECS->getNumberOfEntities();

	viewport->begin(window, reservedEntities);
	load(Save::getDefaultAbsolutePath());
}

void Engine::loop()
{

	while (!glfwWindowShouldClose(window) || bIsPlaying)
	{
		//// input
		processInput(window);

		cameraEntity = bIsPlaying ? gameCameraEntity : editorCameraEntity;

		
		CameraSystem::setPerspective(cameraEntity, ECS, fov, windowWidth / windowHeight, 0.1f, 1000.0f);
		// can be used to calc deltatime
		float currentFrame = glfwGetTime();
		if (!bIsPlaying)
		{
			deltaTime = currentFrame - lastFrame;
			lastFrame = currentFrame;
		}
		else
		{

			std::this_thread::sleep_for(std::chrono::duration<float>(0.016f - (currentFrame - lastFrame)));
			deltaTime = currentFrame - lastFrame;
			lastFrame = currentFrame;
			//PhysicsSystem::update(terrainEntity, ECS, 0.016); // without deltatime for debugging
			PhysicsSystem::update(terrainEntity, ECS, deltaTime);
			//TrailSystem::recordPositions(ECS, currentFrame);
			//TrailSystem::generateBSplines(trailEntity, ECS);
		}

		CollisionBroadphaseDatastructure->update();



		if(!bIsPlaying)
			editorSelection();

		if (bIsPlaying)
		{
			

			CameraSystem::updateGameCamera(cameraEntity, ECS, deltaTime);
			ScriptSystem::Invoke("Update", ECS);
		}
		else
		{

			CameraSystem::updateEditorCamera(cameraEntity, ECS, deltaTime);
		}
		// contains camera draw and mesh system draw.
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		glStencilMask(0x00);
		LightSystem::DrawShadows(ShadowShader, ShadowDepthShader, "u_model", ECS, SystemEntity, cameraEntity, deltaTime);

		//LightSystem::DefferedRendering(GeometryPassShader, LightPassShader, "u_model", ECS, SystemEntity, cameraEntity);


	
		glDepthFunc(GL_LESS);
		glEnable(GL_STENCIL_TEST);
		glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	

		glStencilFunc(GL_ALWAYS, 1, 0xFF); // all fragments should pass the stencil test
		glStencilMask(0xFF);
		CameraSystem::draw(cameraEntity, ShadowShader, ECS);
		MeshSystem::draw(ShadowShader, "u_model", ECS, cameraEntity);
		// enable writing to the stencil buffer


		glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
		glStencilMask(0x00); // disable writing to the stencil buffer
		glDisable(GL_DEPTH_TEST);
		CameraSystem::draw(cameraEntity, outlineShader, ECS);
		MeshSystem::drawOutline(outlineShader, "u_model", ECS);
		
	
		glStencilMask(0xFF);
		glStencilFunc(GL_ALWAYS, 1, 0xFF);
		glEnable(GL_DEPTH_TEST);

		glDisable(GL_STENCIL_TEST);
		

		if (bIsPlaying)
		{
			// RTS Selection render -- Translucent -- ingame only
			CameraSystem::draw(cameraEntity, ourShader, ECS);

			// Only checks collision for one frame, so we delete it after
			//if (ECS->getComponentManager<AxisAlignedBoxComponent>() && ECS->getComponentManager<AxisAlignedBoxComponent>()->getComponentChecked(RTSSelectionEntity))
			if (ECS->getComponentManager<AxisAlignedBoxComponent>()->getComponentChecked(SystemEntity))
			{
				SelectionSystem::setHitEntities(SystemEntity,
						CollisionBroadphaseDatastructure->getOverlappedEntities(SystemEntity), ECS);
				ECS->removeComponent<AxisAlignedBoxComponent>(SystemEntity);
			}

			SelectionSystem::updateSelection(SystemEntity, cameraEntity, ECS, currentFrame);
			//SelectionSystem::drawSelectedArea(RTSSelectionEntity, ourShader, ECS);
			SelectionSystem::drawSelectedArea(SystemEntity, ourShader, ECS);
		}

	

		CameraSystem::draw(cameraEntity, particleShader, ECS);

		ParticleSystem::update(cameraEntity, particleShader, ECS, deltaTime);
		HudSystem::render(ECS, hudShader);
		//// Render dear imgui into screen		
		viewport->render();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	std::cout << "\nMain loop ended\n";
}


void Engine::editorSelection()
{
	if (Input::getInstance()->getMouseKeyState(KEY_LMB).bPressed)
	{
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		CameraSystem::draw(cameraEntity, selectionShader, ECS);
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
		auto& selectionComp = ECS->getComponentManager<SelectionComponent>()->getComponent(SystemEntity);
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
		}
	}
}

void Engine::terminate()
{
	viewport->end();
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

float Engine::getDeltaTime_Internal()
{
	return Get().deltaTime;
}
