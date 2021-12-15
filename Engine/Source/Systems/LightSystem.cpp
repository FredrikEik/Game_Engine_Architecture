#include "LightSystem.h"
#include "../Input/Input.h"
#include "../ECSManager.h"
#include "../Shader.h"
#include "../Engine/Engine.h"
#include "glm/glm.hpp"
#include "MeshSystem.h"
#include <glm/gtc/matrix_transform.hpp> 
#include "CameraSystem.h"


void LightSystem::InitGBuffer(GBufferComponent* GBufferComp)
{

	float SCR_WIDTH = Engine::Get().getWindowWidth(); 
	float SCR_HEIGHT = Engine::Get().getWindowHeight();

	glGenFramebuffers(1, &GBufferComp->gBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, GBufferComp->gBuffer);

	// position color buffer
	glGenTextures(1, &GBufferComp->gPosition);
	glBindTexture(GL_TEXTURE_2D, GBufferComp->gPosition);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, GBufferComp->gPosition, 0);
	// normal color buffer
	glGenTextures(1, &GBufferComp->gNormal);
	glBindTexture(GL_TEXTURE_2D, GBufferComp->gNormal);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, GBufferComp->gNormal, 0);
	// color + specular color buffer
	glGenTextures(1, &GBufferComp->gAlbedoSpec);
	glBindTexture(GL_TEXTURE_2D, GBufferComp->gAlbedoSpec);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, GBufferComp->gAlbedoSpec, 0);
	// tell OpenGL which color attachments we'll use (of this framebuffer) for rendering 
	GBufferComp->attachments[0] = GL_COLOR_ATTACHMENT0;
	GBufferComp->attachments[1] = GL_COLOR_ATTACHMENT1;
	GBufferComp->attachments[2] = GL_COLOR_ATTACHMENT2;
	glDrawBuffers(3, GBufferComp->attachments);
	// create and attach depth buffer (renderbuffer)
	glGenRenderbuffers(1, &GBufferComp->rboDepth);
	glBindRenderbuffer(GL_RENDERBUFFER, GBufferComp->rboDepth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, SCR_WIDTH, SCR_HEIGHT);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, GBufferComp->rboDepth);
	// finally check if framebuffer is complete
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Framebuffer not complete!" << std::endl;
	
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void LightSystem::InitSBuffer(ShadowBufferComponent* SComp)
{
	
	glGenFramebuffers(1, &SComp->depthMapFBO);
	glGenTextures(1, &SComp->depthCubemap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, SComp->depthCubemap);
	for (uint i = 0; i < 6; ++i)
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, SComp->SHADOW_WIDTH, SComp->SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	// attach depth texture as FBO's depth buffer
	glBindFramebuffer(GL_FRAMEBUFFER, SComp->depthMapFBO);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, SComp->depthCubemap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void LightSystem::DrawShadows(Shader* ShadowShader, Shader* ShadowDepthShader, const std::string& uniformName, class ECSManager* ECS, uint32 SystemEntity, uint32 cameraEntity, float deltaTime)
{
	auto sBufferComp = ECS->getComponentManager<ShadowBufferComponent>()->getComponentChecked(SystemEntity);
	auto cameraComp = ECS->getComponentManager<CameraComponent>()->getComponentChecked(cameraEntity);
	
	sBufferComp->m_theta += glm::radians(45.f) * deltaTime;
	const float radius{ 100 };
	sBufferComp->lightPos += glm::vec3(radius * std::sin(sBufferComp->m_theta), 0, radius * std::cos(sBufferComp->m_theta)) * deltaTime * 0.1f;


	
	glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f),
		(float)sBufferComp->SHADOW_WIDTH / (float)sBufferComp->SHADOW_HEIGHT,
		cameraComp->near, cameraComp->far);
	std::vector<glm::mat4> shadowTransforms;
	shadowTransforms.push_back(shadowProj * glm::lookAt(sBufferComp->lightPos, sBufferComp->lightPos + glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
	shadowTransforms.push_back(shadowProj * glm::lookAt(sBufferComp->lightPos, sBufferComp->lightPos + glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
	shadowTransforms.push_back(shadowProj * glm::lookAt(sBufferComp->lightPos, sBufferComp->lightPos + glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
	shadowTransforms.push_back(shadowProj * glm::lookAt(sBufferComp->lightPos, sBufferComp->lightPos + glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)));
	shadowTransforms.push_back(shadowProj * glm::lookAt(sBufferComp->lightPos, sBufferComp->lightPos + glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
	shadowTransforms.push_back(shadowProj * glm::lookAt(sBufferComp->lightPos, sBufferComp->lightPos + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));

	// 1. render scene to depth cubemap
	// --------------------------------
	glViewport(0, 0, sBufferComp->SHADOW_WIDTH, sBufferComp->SHADOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, sBufferComp->depthMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);
	ShadowDepthShader->use();
	for (unsigned int i = 0; i < 6; ++i)
		ShadowDepthShader->setMat4("u_shadowMatrices[" + std::to_string(i) + "]", shadowTransforms[i]);
	ShadowDepthShader->setFloat("u_far_plane", cameraComp->far);
	ShadowDepthShader->setVec3("u_lightPos", sBufferComp->lightPos);
	CameraSystem::draw(cameraEntity, ShadowDepthShader, ECS);
	MeshSystem::draw(ShadowDepthShader, uniformName, ECS, cameraEntity);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// 2. render scene as normal 
	// -------------------------
	float SCR_WIDTH = Engine::Get().getWindowWidth();
	float SCR_HEIGHT = Engine::Get().getWindowHeight();
	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	ShadowShader->use();
	
	// set lighting uniforms
	ShadowShader->setVec3("u_lightPos", sBufferComp->lightPos);
	TransformComponent* cameraTransformComp{ ECS->getComponentManager<TransformComponent>()->getComponentChecked(cameraEntity) };
	glm::vec3 originOfCam = cameraTransformComp->transform[3];

	ShadowShader->setVec3("u_camPos", originOfCam);
	ShadowShader->setInt("u_shadows", 1); // enable/disable shadows by pressing 'SPACE'
	ShadowShader->setFloat("u_far_plane", cameraComp->far);

	ShadowShader->setInt("u_depthMap", 3);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_CUBE_MAP, sBufferComp->depthCubemap);

}

void renderQuad()
{
	unsigned int quadVAO = 0;
	unsigned int quadVBO;
	if (quadVAO == 0)
	{
		float quadVertices[] = {
			// positions        // texture Coords
			-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
			 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		};
		// setup plane VAO
		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &quadVBO);
		glBindVertexArray(quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	}
	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}

void LightSystem::SetLightValues(uint32 entityID, class ECSManager* ECS, glm::vec3 lightColor, float linear, float quadratic)
{
	LightComponent* ligthComp = ECS->getComponentManager<LightComponent>()->getComponentChecked(entityID);
	ligthComp->m_LightColor = lightColor;
	ligthComp->m_Linear = linear;
	ligthComp->m_Quadratic = quadratic;
}

void LightSystem::DeferredRendering(Shader* GeometryPassShader, Shader* LightPassShader, const std::string& uniformName, class ECSManager* manager, uint32 SystemEntity, uint32 cameraEntity)
{
	auto gBufferComp = manager->getComponentManager<GBufferComponent>()->getComponentChecked(SystemEntity);


	glBindFramebuffer(GL_FRAMEBUFFER, gBufferComp->gBuffer);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	CameraSystem::draw(cameraEntity, GeometryPassShader, manager);
	MeshSystem::draw(GeometryPassShader, uniformName, manager, cameraEntity, true);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	LightPassShader->use();
	LightPassShader->setInt("gPosition", 0);
	LightPassShader->setInt("gNormal", 1);
	LightPassShader->setInt("gAlbedoSpec", 2);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, gBufferComp->gPosition);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, gBufferComp->gNormal);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, gBufferComp->gAlbedoSpec);

	
	
	ComponentManager<LightComponent>* lightManager = manager->getComponentManager<LightComponent>();
	if (lightManager)
	{
		auto& lightArray = lightManager->getComponentArray();
		LightPassShader->setInt("u_NumLights", lightArray.size());
		int i = 0;
		for (auto& light : lightArray)
		{

			TransformComponent* lightTransform{ manager->getComponentManager<TransformComponent>()->getComponentChecked(light.entityID) };
			glm::vec3 lightPos = lightTransform->transform[3];

			const glm::vec3 ligthColor = light.m_LightColor;
			const float linear = light.m_Linear;
			const float quadratic = light.m_Quadratic;

			LightPassShader->setVec3("u_Lights[" + std::to_string(i) + "].Position", lightPos);
			LightPassShader->setVec3("u_Lights[" + std::to_string(i) + "].Color", ligthColor);

			LightPassShader->setFloat("u_Lights[" + std::to_string(i) + "].Linear", linear);
			LightPassShader->setFloat("u_Lights[" + std::to_string(i) + "].Quadratic", quadratic);
			++i;
		}

	}
	TransformComponent* cameraTransformComp{ manager->getComponentManager<TransformComponent>()->getComponentChecked(cameraEntity) };
	glm::vec3 originOfCam = cameraTransformComp->transform[3];
	LightPassShader->setVec3("u_CameraPos", originOfCam);

	renderQuad();
	


	glBindFramebuffer(GL_READ_FRAMEBUFFER, gBufferComp->gBuffer);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0); // write to default framebuffer

	float SCR_WIDTH = Engine::Get().getWindowWidth();
	float SCR_HEIGHT = Engine::Get().getWindowHeight();
	glBlitFramebuffer(0, 0, SCR_WIDTH, SCR_HEIGHT, 0, 0, SCR_WIDTH, SCR_HEIGHT, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
} 