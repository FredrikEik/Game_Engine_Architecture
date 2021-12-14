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
#include "../Systems/ParticleSystem.h"
#include "../Systems/TerrainSystem.h"

#include "../SaveLoad/Save.h"
#include "../SaveLoad/Load.h"


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
	MeshSystem::setHiddenInGame(gameCameraEntity, ECS, isPlaying);
	if (!isPlaying)
	{
		load(Save::getDefaultAbsolutePath());
		TransformSystem::setPosition(gameCameraEntity, glm::vec3(), ECS);
		CameraSystem::updateGameCamera(gameCameraEntity, ECS, 0.016);
		ScriptSystem::Invoke("BeginPlay", ECS);
	}
	else
	{
		save();
	}
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
	//return 0;
	return Load::loadEntity(mono_string_to_utf8(path), Get().ECS);
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
	particleShader = new Shader("Shaders/ParticleShader.vert", "Shaders/ParticleShader.frag");


	editorCameraEntity = ECS->newEntity();
	ECS->addComponents<CameraComponent, TransformComponent>(editorCameraEntity);	
	gameCameraEntity = ECS->newEntity();
	ECS->addComponents<CameraComponent, TransformComponent>(gameCameraEntity);
	CameraSystem::setPerspective(gameCameraEntity, ECS, fov, windowWidth / windowHeight, 0.1f, 30.0f);
	CameraSystem::updateGameCamera(gameCameraEntity, ECS, 0.016f);
	CameraSystem::createFrustumMesh(gameCameraEntity, ECS);
	TransformSystem::setHeight(editorCameraEntity, 5, ECS);

	RTSSelectionEntity = ECS->newEntity();
	/// transform can be used to creat rts selection
	//mesh comp
	// opacity shader
	ECS->addComponents<TransformComponent, SelectionComponent>(RTSSelectionEntity);


	terrainEntity = ECS->newEntity();
	//ECS->loadAsset(terrainEntity, "Assets/plane.obj");
	ECS->addComponents<TransformComponent, MeshComponent>(terrainEntity);
	TerrainSystem::generateRegularGrid(terrainEntity, ECS);
	ECS->loadAsset(terrainEntity, "Assets/grass.png");
	//MeshComponent* meshComp = ECS->getComponentManager<MeshComponent>()->getComponentChecked(terrainEntity);
	//meshComp->bDisregardedDuringFrustumCulling = true;
	//TransformSystem::setScale(terrainEntity, glm::vec3(100, 1, 100), ECS);
	//TransformSystem::setPosition(terrainEntity, glm::vec3(0, -1.1, 0), ECS);
	//ECS->addComponent<AxisAlignedBoxComponent>(entity);


	//viewport->begin(window, ECS->getNumberOfEntities());


	//ScriptTest
	//Init - binds all internal functions - Important first step
	ScriptSystem::Init();

	unitEntity = ECS->newEntity();
	ECS->addComponents<TransformComponent, ScriptComponent, MeshComponent, AxisAlignedBoxComponent>(unitEntity);
	ECS->loadAsset(unitEntity, "Assets/suzanne.obj");
	ScriptSystem::InitScriptObject(ECS->getComponentManager<ScriptComponent>()->getComponentChecked(unitEntity));
	CollisionSystem::construct(unitEntity, ECS, false);
	CollisionSystem::setShouldGenerateOverlapEvents(unitEntity, ECS, false);
	//MeshSystem::setHiddenInGame(unitEntity, ECS, true);

	uint32 gameStateEntity = ECS->newEntity();
	ECS->addComponent<ScriptComponent>(gameStateEntity);
	//ScriptSystem::setScriptClassName(gameStateEntity, "GameMode", ECS);
	ScriptSystem::InitScriptObject(ECS->getComponentManager<ScriptComponent>()->getComponentChecked(gameStateEntity), "GameMode");


	reservedEntities = ECS->getNumberOfEntities();

	viewport->begin(window, reservedEntities);
	//Save::saveEntities(ECS->entities, reservedEntities, ECS); // MOVE TO UI
	//Load::loadEntities("../saves/entities.json", ECS);
	load(Save::getDefaultAbsolutePath());


	//ParticleComponent::ParticleBlueprint particleBlueprint;
	//particleBlueprint.particle.currentLife = 0.5f;
	//particleBlueprint.particle.acceleration = glm::vec3(0, 0, 0);
	//particleBlueprint.particle.velocity = glm::vec3(0, 0.f, 0);
	//particleBlueprint.particle.startColor = glm::vec4(1, 1, 1,1);
	//particleBlueprint.particle.position = glm::vec3(1, 5, 1);
	//particleBlueprint.particle.startSize = 30;

	//////TransformSystem::setPosition(unitEntity, glm::vec3(0, 15, 0), ECS);
	//uint32 particleEntity = ECS->newEntity();
	//ECS->addComponents<TransformComponent, ParticleComponent>(particleEntity);
	////ECS->addComponent<ParticleComponent>(unitEntity);
	//ParticleSystem::init(particleEntity, 500000, 2000, particleBlueprint, ECS);
}

//int EntityToTransform{}; // TODO: VERY TEMP, remove as soon as widgets are implemented
void Engine::loop()
{

	while (!glfwWindowShouldClose(window) || bIsPlaying)
	{
		//// input
		processInput(window);

		cameraEntity = bIsPlaying ? gameCameraEntity : editorCameraEntity;

		// TODO: Make this not happen every frame
		CameraSystem::setPerspective(cameraEntity, ECS, fov, windowWidth / windowHeight, 0.1f, 1000.0f);
		// can be used to calc deltatime
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;


		CollisionBroadphaseDatastructure->update();


		//// RENDER
		// Selection Render
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

			//CameraSystem::updateGameCamera(editorCameraEntity, ECS, 0.016f);

			CameraSystem::updateGameCamera(cameraEntity, ECS, deltaTime);

			//temp placement -- calls update on scripts
			ScriptSystem::Invoke("Update", ECS);
		}
		else
		{
			//std::cout << "Editor camera'\n";
			//TODO: Draw a game camera here
			CameraSystem::updateEditorCamera(cameraEntity, ECS, deltaTime);
		}

		glStencilFunc(GL_ALWAYS, 1, 0xFF); // all fragments should pass the stencil test
		glStencilMask(0xFF); // enable writing to the stencil buffer
	
			//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		//CameraSystem::draw(editorCameraEntity, ourShader, ECS);
		//phongShader->setVec3("lightPosition", glm::vec3(2, lightPos, 2));
		//lightPos += 0.01f;
		CameraSystem::draw(cameraEntity, phongShader, ECS);
		CameraSystem::setPhongUniforms(cameraEntity, phongShader, ECS);
		//MeshSystem::draw(ourShader, "u_model", ECS, editorCameraEntity);
		MeshSystem::draw(phongShader, "u_model", ECS, cameraEntity);


		glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
		glStencilMask(0x00); // disable writing to the stencil buffer
		glDisable(GL_DEPTH_TEST);
		CameraSystem::draw(cameraEntity, outlineShader, ECS);
		MeshSystem::drawOutline(outlineShader, "u_model", ECS);
		
	
		glStencilMask(0xFF);
		glStencilFunc(GL_ALWAYS, 1, 0xFF);
		glEnable(GL_DEPTH_TEST);

		if (bIsPlaying)
		{
			// RTS Selection render -- Translucent -- ingame only
			CameraSystem::draw(cameraEntity, ourShader, ECS);

			// Only checks collision for one frame, so we delete it after
			if (ECS->getComponentManager<AxisAlignedBoxComponent>()->getComponentChecked(RTSSelectionEntity))
			{
				SelectionSystem::setHitEntities(RTSSelectionEntity, 
						CollisionBroadphaseDatastructure->getOverlappedEntities(RTSSelectionEntity), ECS);
				ECS->removeComponent<AxisAlignedBoxComponent>(RTSSelectionEntity);
			}

			SelectionSystem::updateSelection(RTSSelectionEntity, cameraEntity, ECS, currentFrame);
			//SelectionSystem::drawSelectedArea(RTSSelectionEntity, ourShader, ECS);
			SelectionSystem::drawSelectedArea(RTSSelectionEntity, ourShader, ECS);
		}
		CameraSystem::draw(cameraEntity, particleShader, ECS);

		ParticleSystem::update(cameraEntity, particleShader, ECS, deltaTime);

		//// Render dear imgui into screen
		//ImGui::Render();
		//ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		viewport->render();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	std::cout << "\nMain loop ended\n";
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

float Engine::getDeltaTime_Internal()
{
	return Get().deltaTime;
}
