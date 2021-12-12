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

	float SCR_WIDTH = Engine::Get().getWindowHeight();
	float SCR_HEIGHT = Engine::Get().getWindowWidth();

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
	unsigned int attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
	glDrawBuffers(3, attachments);
	// create and attach depth buffer (renderbuffer)
	unsigned int rboDepth;
	glGenRenderbuffers(1, &rboDepth);
	glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, SCR_WIDTH, SCR_HEIGHT);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);
	// finally check if framebuffer is complete
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Framebuffer not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

// renderQuad() renders a 1x1 XY quad in NDC
// -----------------------------------------
unsigned int quadVAO = 0;
unsigned int quadVBO;
void renderQuad()
{
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

void LightSystem::DefferedRendering(Shader* GeometryPassShader, Shader* LightPassShader, const std::string& uniformName, class ECSManager* manager, uint32 SystemEntity, uint32 cameraEntity)
{
	auto gBufferComp = manager->getComponentManager<GBufferComponent>()->getComponentChecked(SystemEntity);

	glBindFramebuffer(GL_FRAMEBUFFER, gBufferComp->gBuffer);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	CameraSystem::draw(cameraEntity, GeometryPassShader, manager);
	MeshSystem::draw(GeometryPassShader, uniformName, manager, cameraEntity);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	LightPassShader->use();
	glUniform1i(glGetUniformLocation(LightPassShader->getShaderID(), "gPosition"), 0);
	glUniform1i(glGetUniformLocation(LightPassShader->getShaderID(), "gNormal"), 1);
	glUniform1i(glGetUniformLocation(LightPassShader->getShaderID(), "gAlbedoSpec"), 2);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, gBufferComp->gPosition);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, gBufferComp->gNormal);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, gBufferComp->gAlbedoSpec);


	// send in light uniforms
	// render a quad
	renderQuad();

	glBindFramebuffer(GL_READ_FRAMEBUFFER, gBufferComp->gBuffer);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0); // write to default framebuffer
	// blit to default framebuffer. Note that this may or may not work as the internal formats of both the FBO and default framebuffer have to match.
	// the internal formats are implementation defined. This works on all of my systems, but if it doesn't on yours you'll likely have to write to the 		
	// depth buffer in another shader stage (or somehow see to match the default framebuffer's internal format with the FBO's internal format).
	float SCR_WIDTH = Engine::Get().getWindowHeight();
	float SCR_HEIGHT = Engine::Get().getWindowWidth();
	glBlitFramebuffer(0, 0, SCR_WIDTH, SCR_HEIGHT, 0, 0, SCR_WIDTH, SCR_HEIGHT, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	// render actual light objects;
} 