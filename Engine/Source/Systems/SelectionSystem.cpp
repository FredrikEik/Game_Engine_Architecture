#include "SelectionSystem.h"
#include "../Input/Input.h"
#include "../ECSManager.h"
#include "../Shader.h"
#include "../Engine/Engine.h"
#include "glm/glm.hpp"
#include "../Components/Components.h"
#include "MeshSystem.h"
#include <glm/gtc/matrix_transform.hpp> 



void SelectionSystem::init(uint32 entity, ECSManager* ECS)
{
	ECS->addComponent<MeshComponent>(entity);
	MeshComponent* meshComponent{ ECS->getComponentManager<MeshComponent>()->getComponentChecked(entity) };
	meshComponent->bIsTranslucent = true;
	meshComponent->bDisregardedDuringFrustumCulling = true;
}

void SelectionSystem::updateSelection(uint32 entity, uint32 cameraEntity, class ECSManager* ECS, float deltaTime)
{
	SelectionComponent* selectionComponent{ ECS->getComponentManager<SelectionComponent>()->getComponentChecked(entity) };
	CameraComponent* cameraComponent{ ECS->getComponentManager<CameraComponent>()->getComponentChecked(cameraEntity) };
	TransformComponent* cameraTransformComp{ ECS->getComponentManager<TransformComponent>()->getComponentChecked(cameraEntity) };
	assert(selectionComponent);
	assert(cameraComponent);
	assert(cameraTransformComp);

	MousePosition mPos = Input::getInstance()->getMousePosition();

	float x = (2.0f * mPos.x) / Engine::Get().getWindowWidth() - 1.0f;
	float y = 1.0f - (2.0f * mPos.y) / Engine::Get().getWindowHeight();
	float z = 1.0f;
	glm::vec3 ray_nds = glm::vec3(x, y, z);

	
	glm::vec4 ray_clip = glm::vec4(ray_nds.x, ray_nds.y, -1.0, 1.0);

	glm::vec4 ray_eye = glm::inverse(cameraComponent->m_projectionMatrix) * ray_clip;
	ray_eye = glm::vec4(ray_eye.x, ray_eye.y, -1.0, 0.0);
	glm::vec3 ray_wor = glm::vec3(glm::inverse(cameraComponent->m_viewMatrix) * ray_eye);
	ray_wor = glm::normalize(ray_wor);


	glm::vec3 originOfCam = cameraTransformComp->transform[3];
	glm::vec3 normalOfPlane = glm::vec3(0.f,1.f,0.f);
	float distanceOffset = -1.f;

	float distanceFromRayOriginToPlane = - (glm::dot(originOfCam, normalOfPlane) + distanceOffset) / glm::dot(ray_wor, normalOfPlane);

	//initial loc av når du trykker
	if (distanceFromRayOriginToPlane <= 0)
		return;

	if (Input::getInstance()->getMouseKeyState(KEY_LMB).bPressed)
		selectionComponent->initialHitPos = originOfCam + ray_wor * distanceFromRayOriginToPlane;

	if (Input::getInstance()->getMouseKeyState(KEY_LMB).bHeld)
		selectionComponent->currentHitPos = originOfCam + ray_wor * distanceFromRayOriginToPlane;

	if (Input::getInstance()->getMouseKeyState(KEY_LMB).bReleased)
	{
		glm::vec3 originPos =  selectionComponent->initialHitPos;
		glm::vec3 endPos = selectionComponent->currentHitPos;

		float minX = originPos.x < endPos.x ? originPos.x : endPos.x;
		float minZ = originPos.z < endPos.z ? originPos.z : endPos.z;
		float maxX = originPos.x >= endPos.x ? originPos.x : endPos.x;
		float maxZ = originPos.z >= endPos.z ? originPos.z : endPos.z;

		ECS->addComponent<AxisAlignedBoxComponent>(entity);
		AxisAlignedBoxComponent *AABB = ECS->getComponentManager<AxisAlignedBoxComponent>()->getComponentChecked(entity);
		assert(AABB);
		AABB->bShouldGenerateOverlapEvents = true;
		AABB->minScaled = glm::vec3(minX, -2, minZ);
		AABB->maxScaled = glm::vec3(maxX, 4, maxZ);
		AABB->center = glm::vec3((maxX + minX) / 2.f,
			0, (maxZ + minZ) / 2.f);

	}
}

void SelectionSystem::drawSelectedArea(uint32 entity, Shader* shader, ECSManager* ECS)
{
	init(entity, ECS);

	if (Input::getInstance()->getMouseKeyState(KEY_LMB).bHeld)
	{
		SelectionComponent* selectionComponent{ ECS->getComponentManager<SelectionComponent>()->getComponentChecked(entity) };
		MeshComponent* meshComponent{ ECS->getComponentManager<MeshComponent>()->getComponentChecked(entity) };
		TransformComponent* transformComponent{ ECS->getComponentManager<TransformComponent>()->getComponentChecked(entity) };
		assert(selectionComponent);
		assert(meshComponent);
		assert(transformComponent);

		std::string uniform = "u_model";

		MeshSystem::drawRTSSelection(shader, selectionComponent->initialHitPos, selectionComponent->currentHitPos, uniform, *meshComponent, *transformComponent);
	}
	
	ECS->removeComponent<MeshComponent>(entity);
}

void SelectionSystem::setHitEntities(uint32 entity, const std::vector<uint32> hitEntities, ECSManager* ECS)
{
	SelectionComponent* component = ECS->getComponentManager<SelectionComponent>()->getComponentChecked(entity);
	assert(component);
	component->hitEntities = hitEntities;
}

bool SelectionSystem::IsEntitySelected_internal(uint32 EntityID)
{
	auto& selectionCompArray = Engine::Get().getECSManager()->getComponentManager<SelectionComponent>()->getComponentArray();
	for (auto selectionComp : selectionCompArray)
	{
		for (auto hitEnt : selectionComp.hitEntities)
		{
			
			if (hitEnt == EntityID)
				return true;
		}
	}
	return false;
}

glm::vec3 SelectionSystem::getCursorWorldPosition(uint32 entity, uint32 cameraEntity, ECSManager* ECS)
{
	CameraComponent* cameraComponent{ ECS->getComponentManager<CameraComponent>()->getComponentChecked(cameraEntity) };
	TransformComponent* cameraTransformComp{ ECS->getComponentManager<TransformComponent>()->getComponentChecked(cameraEntity) };
	assert(cameraComponent);
	assert(cameraTransformComp);

	MousePosition mPos = Input::getInstance()->getMousePosition();

	float x = (2.0f * mPos.x) / Engine::Get().getWindowWidth() - 1.0f;
	float y = 1.0f - (2.0f * mPos.y) / Engine::Get().getWindowHeight();
	float z = 1.0f;
	glm::vec3 ray_nds = glm::vec3(x, y, z);


	glm::vec4 ray_clip = glm::vec4(ray_nds.x, ray_nds.y, -1.0, 1.0);

	glm::vec4 ray_eye = glm::inverse(cameraComponent->m_projectionMatrix) * ray_clip;
	ray_eye = glm::vec4(ray_eye.x, ray_eye.y, -1.0, 0.0);
	glm::vec3 ray_wor = glm::vec3(glm::inverse(cameraComponent->m_viewMatrix) * ray_eye);
	ray_wor = glm::normalize(ray_wor);


	glm::vec3 originOfCam = cameraTransformComp->transform[3];
	glm::vec3 normalOfPlane = glm::vec3(0.f, 1.f, 0.f);
	float distanceOffset = -1.f;

	float distanceFromRayOriginToPlane = -(glm::dot(originOfCam, normalOfPlane) + distanceOffset) / glm::dot(ray_wor, normalOfPlane);

	//initial loc av når du trykker
	if (distanceFromRayOriginToPlane <= 0)
		return glm::vec3();


		
	return originOfCam + ray_wor * distanceFromRayOriginToPlane;
}

glm::vec3 SelectionSystem::getCursorWorldPosition_Internal(uint32 entity)
{
	Engine& engine = Engine::Get();
	return getCursorWorldPosition(entity, engine.getCameraEntity(), engine.getECSManager());
}
