#include "ParticleSystem.h"
#include "../ECSManager.h"
#include "../Shader.h"
#include "TextureSystem.h"
#include <unordered_set>
#include "../Engine/Engine.h"
void ParticleSystem::init(uint32 entityID, class ECSManager* ECS, uint32 maxParticles, 
    std::filesystem::path path, int textureRows)
{
    if (!ECS->getComponentManager<ParticleComponent>())
        return;

	ParticleComponent* emitter = ECS->getComponentManager<ParticleComponent>()->getComponentChecked(entityID);
    if (!emitter)
    {
        ECS->addComponent<ParticleComponent>(entityID);
        emitter = ECS->getComponentManager<ParticleComponent>()->getComponentChecked(entityID);
    }
    emitter->maxParticles = maxParticles;

    initTexture(emitter, path, textureRows);
    initMesh(emitter, maxParticles);
}

void ParticleSystem::initMesh(ParticleComponent* emitter, uint32 maxParticles)
{
    emitter->maxParticles = maxParticles;
    emitter->positionData = std::vector<float>(maxParticles * 4, 0.f);
    emitter->colorData = std::vector<float>(maxParticles * 8, 0.f);
    emitter->lifeAndSizeData = std::vector<float>(maxParticles * 4, 0.f);
    emitter->particles.resize(maxParticles);

#pragma region Generating Quad
    // Not using the vertex class as it would waste space, and the quad is so simple anyway.
    static const GLfloat vertexData[] = {
    -0.5f, -0.5f, 0.0f, 0.f, 1.f, // V in UV flipped due to openGL reading texture bot-top
     0.5f, -0.5f, 0.0f, 1.f, 1.f,
    -0.5f,  0.5f, 0.0f, 0.f, 0.f,
     0.5f,  0.5f, 0.0f, 1.f, 0.f
    };

    emitter->mesh.m_indices.push_back(0);
    emitter->mesh.m_indices.push_back(1);
    emitter->mesh.m_indices.push_back(2);

    emitter->mesh.m_indices.push_back(2);
    emitter->mesh.m_indices.push_back(1);
    emitter->mesh.m_indices.push_back(3);

    //Generating Quad
#pragma endregion

#pragma region Generating Buffers

    glGenVertexArrays(1, &emitter->mesh.m_VAO);
    glBindVertexArray(emitter->mesh.m_VAO);

    glGenBuffers(1, &emitter->mesh.m_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, emitter->mesh.m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData),
        vertexData, GL_STATIC_DRAW);

    glGenBuffers(1, &emitter->positionBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, emitter->positionBuffer);
    glBufferData(GL_ARRAY_BUFFER, maxParticles * 4 * sizeof(float),
        NULL, GL_STREAM_DRAW);

    glGenBuffers(1, &emitter->colorBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, emitter->colorBuffer);
    glBufferData(GL_ARRAY_BUFFER, maxParticles * 8 * sizeof(float),
        NULL, GL_STREAM_DRAW);

    glGenBuffers(1, &emitter->lifeAndSizeDataBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, emitter->lifeAndSizeDataBuffer);
    glBufferData(GL_ARRAY_BUFFER, maxParticles * 4 * sizeof(float),
        NULL, GL_STREAM_DRAW);

    // Generating Buffers
#pragma endregion 

#pragma region Vertex Attrib Pointers
    // 1st attribute buffer : vertices
    glBindBuffer(GL_ARRAY_BUFFER, emitter->mesh.m_VBO);
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

    // 3rd attribute buffer : positions of particles' centers
    glBindBuffer(GL_ARRAY_BUFFER, emitter->positionBuffer);
    glVertexAttribPointer(
        2,
        4, // size : x + y + z + size => 4
        GL_FLOAT, // type
        GL_FALSE, // normalized?
        0, // stride
        (void*)0 // array buffer offset
    );
    glEnableVertexAttribArray(2);

    // 4th attribute buffer : particles' start colors
    glBindBuffer(GL_ARRAY_BUFFER, emitter->colorBuffer);
    glVertexAttribPointer(
        3,
        4, // size : r + g + b + a => 4
        GL_FLOAT, // type
        GL_TRUE, // normalized? 
        sizeof(float) * 8, // stride. 8 floats for color start and color end
        (void*)0 // array buffer offset
    );
    glEnableVertexAttribArray(3);

    // 5th attribute buffer : particles' life
    glBindBuffer(GL_ARRAY_BUFFER, emitter->lifeAndSizeDataBuffer);
    glVertexAttribPointer(
        4,
        2, // size : current life, total life
        GL_FLOAT, // type
        GL_FALSE, // normalized? 
        4*sizeof(float), // stride 4 floats. Life start - end and size start - end
        (void*)0 // array buffer offset
    );
    glEnableVertexAttribArray(4);

    // 6th attribute buffer : particles' size
    glBindBuffer(GL_ARRAY_BUFFER, emitter->lifeAndSizeDataBuffer);
    glVertexAttribPointer(
        5,
        2, // size : start size, end size
        GL_FLOAT, // type
        GL_FALSE, // normalized? 
        4 * sizeof(float), // stride 4 floats. Life start - end and size start - end
        (void*)(2 * sizeof(float)) // array buffer offset
    );
    glEnableVertexAttribArray(5);

    // 7th attribute buffer : particles' end colors
    glBindBuffer(GL_ARRAY_BUFFER, emitter->colorBuffer);
    glVertexAttribPointer(
        6,
        4, // size : r + g + b + a => 4
        GL_FLOAT, // type
        GL_TRUE, // normalized? 
        sizeof(float) * 8, // stride. 8 floats for color start and color end
        (void*)(4 * sizeof(float)) // array buffer offset
    );
    glEnableVertexAttribArray(6);
    //Vertex Attrib Pointers
#pragma endregion

#pragma region Buffer Data

    glBindBuffer(GL_ARRAY_BUFFER, emitter->positionBuffer);
    glBufferData(GL_ARRAY_BUFFER, emitter->maxParticles * sizeof(float) * 4, NULL, GL_STREAM_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, emitter->colorBuffer);
    glBufferData(GL_ARRAY_BUFFER, emitter->maxParticles * sizeof(float) * 8, NULL, GL_STREAM_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, emitter->lifeAndSizeDataBuffer);
    glBufferData(GL_ARRAY_BUFFER, emitter->maxParticles * sizeof(float) * 4, NULL, GL_STREAM_DRAW);
    //Buffer Data
#pragma endregion

    // Binding indices
    glGenBuffers(1, &emitter->mesh.m_EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, emitter->mesh.m_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, emitter->mesh.m_indices.size() * sizeof(GLuint),
        emitter->mesh.m_indices.data(), GL_STATIC_DRAW);

    glBindVertexArray(0);
}

void ParticleSystem::initTexture(ParticleComponent* emitter, const std::filesystem::path& path, int8 textureRows)
{
    TextureSystem::loadImageWithAlpha(path, &emitter->texture);
    emitter->textureRows = textureRows;
    emitter->texture.path = path.string();
}

void ParticleSystem::update(uint32 cameraEntity, class Shader* shader, ECSManager* ECS, float deltaTime)
{
#pragma region Setting up emitters

    auto transformManager = ECS->getComponentManager<TransformComponent>();
    if (!ECS->getComponentManager<ParticleComponent>() || !transformManager)
        return;

    TransformComponent* cameraTransform = transformManager->getComponentChecked(cameraEntity);
    glm::vec3 cameraPosition = glm::vec3(cameraTransform->transform[3]);
    auto& emitters = ECS->getComponentManager<ParticleComponent>()->getComponentArray();

    std::vector<int> sortedEmitters;
    for (int i{}; i < emitters.size(); ++i)
    {
        sortedEmitters.push_back(i);
    }
    // Sorting emitter indices furthest < nearest
    sortEmitters(ECS, cameraPosition, emitters, sortedEmitters);

    //Setting up emitters
#pragma endregion

    shader->use();
    glEnable(GL_BLEND);
    std::unordered_set<uint32> entitiesToDestroy;
    // Iterating through all emitters, starting with the one furthest from the camera
    for (auto index : sortedEmitters)
    {
        ParticleComponent& emitter = emitters[index];

        // Checks if the emitter is active
        if (!emitter.bLoops)
        {
            emitter.emitterLifeTime -= deltaTime;
            if (emitter.emitterLifeTime <= 0.f)
            {
                if (emitter.destroyOnLifetimeEnd && Engine::Get().getIsPlaying())
                    entitiesToDestroy.insert(emitter.entityID);
                continue;
            }
        }

        TransformComponent* emitterTransform = transformManager->getComponentChecked(emitter.entityID);
        const glm::vec3& emitterPosition = glm::vec3(emitterTransform->transform[3]);

        // Spawning and sorting particles based on distance from camera and particle.active
        // This way the blending will be correct, and inactive particles won't be rendered
        spawnParticles(emitter, deltaTime, glm::vec3(emitterPosition), cameraPosition);
        if (emitter.activeParticles <= 0)
            continue;
        std::sort(&emitter.particles[0], &emitter.particles[emitter.activeParticles]);
        // ---------------------

        // Updating all particles, breaks on the first inactive particle
        for (uint32 i{}; 
            i < emitter.maxParticles && 
            updateParticle(i, emitter, emitter.particles[i], emitterPosition, cameraPosition, deltaTime); 
            ++i){}
        // ----------------------

        render(emitter, shader);
    }
    glDisable(GL_BLEND);
    for (auto it : entitiesToDestroy)
    {
        ECS->destroyEntity(it);
    }
}

void ParticleSystem::setParticleActive(uint32 entityID, bool bShouldBeActive, ECSManager* ECS)
{
    setParticleActive(ECS->getComponentManager<ParticleComponent>()->getComponentChecked(entityID), bShouldBeActive);
}

void ParticleSystem::setParticleActive(ParticleComponent* emitter, bool bShouldBeActive)
{
    if (bShouldBeActive)
    {
        emitter->timeSinceLastSpawn = emitter->spawnFrequency;
        emitter->particles = std::vector<ParticleComponent::Particle>(emitter->maxParticles);
    }
    emitter->emitterLifeTime = emitter->emitterTotalLifeTime * bShouldBeActive;
}

void ParticleSystem::setParticleActive_Internal(uint32 entityID, bool bShouldBeActive)
{
    setParticleActive(entityID, bShouldBeActive, Engine::Get().getECSManager());
}


void ParticleSystem::spawnParticles(ParticleComponent& emitter, float deltaTime,
    const glm::vec3& emitterLocation, const glm::vec3& cameraLocation)
{
    emitter.timeSinceLastSpawn += deltaTime;
    if (emitter.timeSinceLastSpawn < emitter.spawnFrequency)
        return;

    uint32 spawnAmount = emitter.timeSinceLastSpawn * emitter.spawnRate;
    if (spawnAmount <= 0)
        return;
    emitter.timeSinceLastSpawn = 0;
    if (spawnAmount > emitter.spawnRate)
        spawnAmount = emitter.spawnRate;
 
    // Random float in range
    auto randf = [](const float& min, const float& max) {
        return min + static_cast <float> (std::rand()) / (static_cast <float> (RAND_MAX / (max - min)));
    };

    //float cameraDistance = glm::length((emitter.particleBlueprint.particle.position + emitterLocation) - cameraLocation);
    auto& bp = emitter.particleBlueprint;

    uint32 start = (emitter.lastUsedParticle) % emitter.maxParticles;
    for (uint32 i{ start }; i < (start + spawnAmount); ++i)
    {

        uint32 index = i % emitter.maxParticles; 
        auto& particle = emitter.particles[index];

        particle = bp.particle;
        particle.active = true;

        particle.currentLife += randf(bp.lifeMinOffset, bp.lifeMaxOffset);
        particle.totalLife = particle.currentLife;
        float sizeOffset{ randf(bp.sizeMinOffset, bp.sizeMaxOffset) };
        particle.startSize += sizeOffset;
        particle.endSize += sizeOffset;

        particle.startColor += glm::vec4(
            randf(bp.colorMinOffset.x, bp.colorMaxOffset.x), 
            randf(bp.colorMinOffset.y, bp.colorMaxOffset.y), 
            randf(bp.colorMinOffset.z, bp.colorMaxOffset.z), 
            randf(bp.colorMinOffset.w, bp.colorMaxOffset.w));
        
        particle.endColor += glm::vec4(
            randf(bp.colorMinOffset.x, bp.colorMaxOffset.x),
            randf(bp.colorMinOffset.y, bp.colorMaxOffset.y),
            randf(bp.colorMinOffset.z, bp.colorMaxOffset.z),
            randf(bp.colorMinOffset.w, bp.colorMaxOffset.w));

        particle.velocity += glm::vec3(
            randf(bp.velocityMinOffset.x, bp.velocityMaxOffset.x),
            randf(bp.velocityMinOffset.y, bp.velocityMaxOffset.y),
            randf(bp.velocityMinOffset.z, bp.velocityMaxOffset.z)
        );

        particle.acceleration += glm::vec3(
            randf(bp.accelerationMinOffset.x, bp.accelerationMaxOffset.x),
            randf(bp.accelerationMinOffset.y, bp.accelerationMaxOffset.y),
            randf(bp.accelerationMinOffset.z, bp.accelerationMaxOffset.z)
        );

        particle.position += glm::vec3(
            randf(bp.positionMinOffset.x, bp.positionMaxOffset.x),
            randf(bp.positionMinOffset.y, bp.positionMaxOffset.y),
            randf(bp.positionMinOffset.z, bp.positionMaxOffset.z)
        );

        particle.cameraDistance = glm::length((particle.position + emitterLocation) - cameraLocation);
        emitter.lastUsedParticle = index+1;
        emitter.activeParticles = emitter.lastUsedParticle;
    }
}

void ParticleSystem::sortEmitters(ECSManager* ECS, const glm::vec3& cameraPosition,
    const std::vector<ParticleComponent>& emitters,
    std::vector <int>& OUTsortedEmitters)
{

    auto sortFunc = [ECS, cameraPosition, emitters](const int& indexA, const int& indexB)
    {
        auto manager = ECS->getComponentManager<TransformComponent>();
        const ParticleComponent& a = emitters[indexA];
        const ParticleComponent& b = emitters[indexB];

        TransformComponent* transformA{manager->getComponentChecked(a.entityID)};
        TransformComponent* transformB{manager->getComponentChecked(b.entityID)};

        float lengthA = glm::length(glm::vec3(transformA->transform[3]) - cameraPosition);
        float lengthB = glm::length(glm::vec3(transformB->transform[3]) - cameraPosition);
        return lengthA > lengthB;
    };

    std::sort(OUTsortedEmitters.begin(), OUTsortedEmitters.end(), sortFunc);
}

bool ParticleSystem::updateParticle(const uint32& i, ParticleComponent& emitter, ParticleComponent::Particle& particle,
    const glm::vec3& emitterPosition, const glm::vec3& cameraPosition, const float& deltaTime)
{
    auto& p = particle;

    if (!p.active)
    {
        emitter.lastUsedParticle = i;
        emitter.activeParticles = i;
        return false;
    }
    p.currentLife -= deltaTime;
    if (p.currentLife <= 0.f)
    {
        p.active = false;
        p.cameraDistance = -100000.f;
    }
    p.velocity += p.acceleration * deltaTime;
    p.position += p.velocity * deltaTime + 0.5f * p.acceleration * deltaTime * deltaTime;

    p.cameraDistance = glm::length((p.position+ emitterPosition) - cameraPosition);

    emitter.positionData[4 * i + 0] = p.position.x + emitterPosition.x;
    emitter.positionData[4 * i + 1] = p.position.y + emitterPosition.y;
    emitter.positionData[4 * i + 2] = p.position.z + emitterPosition.z;
    emitter.positionData[4 * i + 3] = p.startSize;

    emitter.colorData[8 * i + 0] = p.startColor.x;
    emitter.colorData[8 * i + 1] = p.startColor.y;
    emitter.colorData[8 * i + 2] = p.startColor.z;
    emitter.colorData[8 * i + 3] = p.startColor.w;

    emitter.colorData[8 * i + 4] = p.endColor.x;
    emitter.colorData[8 * i + 5] = p.endColor.y;
    emitter.colorData[8 * i + 6] = p.endColor.z;
    emitter.colorData[8 * i + 7] = p.endColor.w;

    emitter.lifeAndSizeData[4 * i + 0] = p.currentLife;
    emitter.lifeAndSizeData[4 * i + 1] = p.totalLife;

    emitter.lifeAndSizeData[4 * i + 2] = p.startSize;
    emitter.lifeAndSizeData[4 * i + 3] = p.endSize;
    return true;
}

void ParticleSystem::render(const ParticleComponent& emitter, Shader* shader)
{
    if (emitter.texture.rgbImage)
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, emitter.texture.textureID);
    }
    glUniform1f(glGetUniformLocation(shader->getShaderID(), "u_textureRows"), emitter.textureRows);
    glBlendFunc(emitter.blendSFactor, emitter.blendDFactor);

    glBindVertexArray(emitter.mesh.m_VAO);
    glVertexAttribDivisor(0, 0); // particles vertices : always reuse the same 4 vertices -> 0
    glVertexAttribDivisor(1, 0); // particles UV : always reuse the same 4 UV -> 0
    glVertexAttribDivisor(2, 1); // positions : one per quad (its center) -> 1
    glVertexAttribDivisor(3, 1); // color : one per quad -> 1
    glVertexAttribDivisor(4, 1); // uv blending : one per quad -> 1
    glVertexAttribDivisor(5, 1); // life blending : one per quad -> 1
    glVertexAttribDivisor(6, 1); // size blending : one per quad -> 1


    glBindBuffer(GL_ARRAY_BUFFER, emitter.positionBuffer);
    glBufferSubData(GL_ARRAY_BUFFER, 0, 4 * emitter.activeParticles * sizeof(float), emitter.positionData.data());

    glBindBuffer(GL_ARRAY_BUFFER, emitter.colorBuffer);
    glBufferSubData(GL_ARRAY_BUFFER, 0, 8 * emitter.activeParticles * sizeof(float), emitter.colorData.data());

    glBindBuffer(GL_ARRAY_BUFFER, emitter.lifeAndSizeDataBuffer);
    glBufferSubData(GL_ARRAY_BUFFER, 0, 4 * emitter.activeParticles * sizeof(float), emitter.lifeAndSizeData.data());

    glDrawElementsInstanced(GL_TRIANGLES, emitter.mesh.m_indices.size(),
        GL_UNSIGNED_INT, 0, emitter.activeParticles);
}
