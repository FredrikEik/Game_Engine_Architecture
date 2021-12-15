#include "HudSystem.h"
#include "../ECSManager.h"
#include "MeshSystem.h"
#include <chrono>
#include "../Shader.h"
#include "../Engine/Engine.h"
#include "../Systems/CameraSystem.h"
void HudSystem::init(uint32 entity, ECSManager* ECS, const std::filesystem::path& texturePath)
{
    HudComponent* hud = ECS->getComponentManager<HudComponent>()->getComponentChecked(entity);
    if (texturePath.string() != "")
    {
        TextureSystem::loadImageWithAlpha(texturePath, &hud->texture);
    }

    // Not using the vertex class as it would waste space, and the quad is so simple anyway.
    static const GLfloat vertexData[] = {
    -0.5f, -0.5f, 0.0f, 0.f, 1.f, // V in UV flipped due to openGL reading texture bot-top
     0.5f, -0.5f, 0.0f, 1.f, 1.f,
    -0.5f,  0.5f, 0.0f, 0.f, 0.f,
     0.5f,  0.5f, 0.0f, 1.f, 0.f
    };

    hud->mesh.m_indices.push_back(0);
    hud->mesh.m_indices.push_back(1);
    hud->mesh.m_indices.push_back(2);

    hud->mesh.m_indices.push_back(2);
    hud->mesh.m_indices.push_back(1);
    hud->mesh.m_indices.push_back(3);

    glGenVertexArrays(1, &hud->mesh.m_VAO);
    glBindVertexArray(hud->mesh.m_VAO);

    glGenBuffers(1, &hud->mesh.m_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, hud->mesh.m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData),
        vertexData, GL_STATIC_DRAW);

    // 1st attribute buffer : vertices
    glBindBuffer(GL_ARRAY_BUFFER, hud->mesh.m_VBO);
    glVertexAttribPointer(
        0,
        3, // size
        GL_FLOAT, // type
        GL_FALSE, // normalized?
        5 * sizeof(float), // stride. 3 positions + 2 UV's
        (void*)0 // array buffer offset
    );
    glEnableVertexAttribArray(0);

    // 2nd attribute buffer : UV's
    glVertexAttribPointer(
        1,
        2, // size
        GL_FLOAT, // type
        GL_FALSE, // normalized?
        5 * sizeof(float), // stride. 3 positions + 2 UV's
        (void*)(3 * sizeof(float)) // array buffer offset
    );
    glEnableVertexAttribArray(1);

    // Binding indices
    glGenBuffers(1, &hud->mesh.m_EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, hud->mesh.m_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, hud->mesh.m_indices.size() * sizeof(GLuint),
        hud->mesh.m_indices.data(), GL_STATIC_DRAW);

    glBindVertexArray(0);
}

void HudSystem::render(ECSManager* ECS, class Shader* shader)
{
    auto manager = ECS->getComponentManager<HudComponent>();
    if (!manager)
        return;

    auto& components = manager->getComponentArray();
    shader->use();
    uint32 cameraEntity = Engine::Get().getCameraEntity();
    //CameraSystem::draw(cameraEntity, shader, ECS);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    float height = Engine::Get().getWindowHeight();
    float width = Engine::Get().getWindowWidth();

    float aspectRatio = height / width;
    
    for (auto& it : components)
    {
        glBindVertexArray(it.mesh.m_VAO);
        glActiveTexture(GL_TEXTURE0);

        glBindTexture(GL_TEXTURE_2D, it.texture.textureID);

        glm::vec4 center = it.center;
        center.z *= aspectRatio;

        glUniform4fv(glGetUniformLocation(shader->getShaderID(), "u_center"), 1, &center.x);
        glDrawElements(it.mesh.m_drawType, it.mesh.m_indices.size(), GL_UNSIGNED_INT, 0);

        glBindVertexArray(0);

    }
    glDisable(GL_BLEND);

}

void HudSystem::setPosition(uint32 entity, const glm::vec2& position, ECSManager* ECS)
{
    HudComponent* component = ECS->getComponentManager<HudComponent>()->getComponentChecked(entity);
    component->center.x = position.x;
    component->center.y = position.y;
}

void HudSystem::setScale(uint32 entity, const glm::vec2& scale, ECSManager* ECS)
{
    HudComponent* component = ECS->getComponentManager<HudComponent>()->getComponentChecked(entity);
    component->center.z = scale.x;
    component->center.w = scale.y;
}

void HudSystem::showHud_Internal(uint32 entity, MonoString* path)
{
    ECSManager* ECS = Engine::Get().getECSManager();
    auto manager = ECS->getComponentManager<HudComponent>();
    if (!manager || !manager->getComponentChecked(entity))
        ECS->addComponent<HudComponent>(entity);
    init(entity, ECS, mono_string_to_utf8(path));
}

void HudSystem::removeHud_Internal(uint32 entity)
{
    ECSManager* ECS = Engine::Get().getECSManager();
    auto manager = ECS->getComponentManager<HudComponent>();
    if (manager && manager->getComponentChecked(entity))
        ECS->removeComponent<HudComponent>(entity);
}
