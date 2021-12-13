#include "ParticleWidget.h"
#include "../imgui/docking/imgui.h"
#include "../imgui/docking/imgui_impl_opengl3.h"
#include "../imgui/docking/imgui_impl_glfw.h"
#include "../ECSManager.h"
#include "../FileSystemHelpers.h"
#include "../Systems/ParticleSystem.h"
#include "../SaveLoad/Save.h"
#include "../SaveLoad/Load.h"

ParticleWidget::ParticleWidget(std::string inWindowName, ECSManager* inECS)
	:Window(inWindowName, inECS)
{
}

void ParticleWidget::begin(Viewport* inViewport, int32 inReservedEntities)
{
	Window::begin(inViewport, inReservedEntities);
}

void ParticleWidget::update(int32 entityID, bool& entitiesChanged)
{
	if (entityID >= reservedEntities)
	{
		ImGui::Text("Particle");
		static bool shouldEdit = false;
		entitiesChanged = false;
		if (ImGui::Button("Save Particle##particle"))
		{
			Save::saveComponent(entityID, std::type_index(typeid(ParticleComponent)), ECS);
		}

		if (ImGui::Button("Load Particle##particle"))
		{
			std::string path;
			if (FileSystemHelpers::getPathFromFileExplorer(path))
			{
				//ECS->removeComponent<TextureComponent>(entityID);
				//ECS->loadAsset(entityID, path);
				Load::loadComponent(path, ECS->getComponentManager<ParticleComponent>()->getComponentChecked(entityID));
				entitiesChanged = true;
			}
		}

		ImGui::Checkbox("Edit Particles", &shouldEdit);
		if (shouldEdit)
		{
			ImGui::Begin("Particle Editor");
			ParticleComponent* component = ECS->getComponentManager<ParticleComponent>()->getComponentChecked(entityID);
			assert(component);

			bool bIsAdditive = component->blendDFactor == GL_ONE;
			bool bShouldBeAdditive = bIsAdditive;
			ImGui::Checkbox("Additive Blending", &bShouldBeAdditive);

			if (bIsAdditive != bShouldBeAdditive)
			{
				component->blendDFactor = bShouldBeAdditive ? GL_ONE : GL_ONE_MINUS_SRC_ALPHA;
			}



			ImGui::SliderInt("Texture Rows##particle", &component->textureRows, 1, 20);
			ImGui::InputInt("Spawn Rate##particle", (int*)&component->spawnRate, 1, 10);
			if (ImGui::InputInt("Max Particles##particle",
				(int*)&component->maxParticles, 10, 100))
			{
				ParticleSystem::initMesh(component, component->maxParticles);
			}
		
			ImGui::NewLine();

			if (ImGui::CollapsingHeader("Particle Defaults"))
			{
				//ImGui::InputFloat3("Position", &component->particleBlueprint.particle.position.x);
				ImGui::DragFloat3("Position##particle", &component->particleBlueprint.particle.position.x, 0.1f, -10000.f, 10000.f);
				ImGui::DragFloat3("Initial Velocity##particle", &component->particleBlueprint.particle.velocity.x, 0.1f, -10000.f, 10000.f);
				ImGui::DragFloat3("Acceleration##particle", &component->particleBlueprint.particle.acceleration.x, 0.1f, -10000.f, 10000.f);
				ImGui::InputFloat("Lifetime##particle", &component->particleBlueprint.particle.currentLife, 0.01f, 0.1f);
				ImGui::InputFloat("Start Size##particle", &component->particleBlueprint.particle.startSize, 0.01f, 0.1f);
				ImGui::InputFloat("End Size##particle", &component->particleBlueprint.particle.endSize, 0.01f, 0.1f);

				if (ImGui::CollapsingHeader("Color"))
				{ 
					ImGui::ColorPicker4("Start Color", &component->particleBlueprint.particle.startColor.x);
					ImGui::ColorPicker4("End Color", &component->particleBlueprint.particle.endColor.x);
				}
			}

			ImGui::NewLine();

			if (ImGui::CollapsingHeader("Particle Offsets"))
			{
				ImGui::DragFloat3("Min Position Offset##particle", &component->particleBlueprint.positionMinOffset.x, 0.1f, -10000.f, 10000.f);
				ImGui::DragFloat3("Max Position Offset##particle", &component->particleBlueprint.positionMaxOffset.x, 0.1f, -10000.f, 10000.f);

				ImGui::DragFloat3("Min Velocity Offset##particle", &component->particleBlueprint.velocityMinOffset.x, 0.1f, -10000.f, 10000.f);
				ImGui::DragFloat3("Max Velocity Offset##particle", &component->particleBlueprint.velocityMaxOffset.x, 0.1f, -10000.f, 10000.f);

				ImGui::DragFloat3("Min Acceleration Offset##particle", &component->particleBlueprint.accelerationMinOffset.x, 0.1f, -10000.f, 10000.f);
				ImGui::DragFloat3("Max Acceleration Offset##particle", &component->particleBlueprint.accelerationMaxOffset.x, 0.1f, -10000.f, 10000.f);

				ImGui::SliderFloat4("Min Color Offset##particle", &component->particleBlueprint.colorMinOffset.x, -1, 1);
				ImGui::SliderFloat4("Max Color Offset##particle", &component->particleBlueprint.colorMaxOffset.x, -1, 1);

				ImGui::InputFloat("Min Size Offset##particle", &component->particleBlueprint.sizeMinOffset, 0.01f, 0.1f);
				ImGui::InputFloat("Max Size Offset##particle", &component->particleBlueprint.sizeMaxOffset, 0.01f, 0.1f);

				ImGui::InputFloat("Min Lifetime Offset##particle", &component->particleBlueprint.lifeMinOffset, 0.01f, 0.1f);
				ImGui::InputFloat("Max Lifetime Offset##particle", &component->particleBlueprint.lifeMaxOffset, 0.01f, 0.1f);
			}

			if (ImGui::Button("Load Texture##particle"))
			{
				std::string path;
				if (FileSystemHelpers::getPathFromFileExplorer(path))
				{
					//ECS->removeComponent<TextureComponent>(entityID);
					//ECS->loadAsset(entityID, path);
					ParticleSystem::initTexture(component, path, component->textureRows);
					entitiesChanged = true;
				}
			}
			ImGui::End();
		}

	}
}

void ParticleWidget::end()
{
	Window::end();
}

