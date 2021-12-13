#include "ParticleSystem.h"
#include "../ECSManager.h"
#include "../Shader.h"
#include "TextureSystem.h"

void ParticleSystem::init(uint32 entityID, uint32 maxParticles, uint32 spawnRate,
    const  ParticleComponent::ParticleBlueprint& particleBlueprint, ECSManager* ECS)
{
    if (!ECS->getComponentManager<ParticleComponent>())
        return;

	ParticleComponent* emitter = ECS->getComponentManager<ParticleComponent>()->getComponentChecked(entityID);
	assert(emitter);
	//constructQuad(emitter->mesh);
    emitter->maxParticles = maxParticles;
    emitter->spawnRate = spawnRate;
    emitter->particleBlueprint = particleBlueprint;

    //emitter->positionData = std::vector<float>(maxParticles * 4, 0.f);
    //emitter->colorData = std::vector<float>(maxParticles * 4, 0.f);
    //emitter->uvBlendingData = std::vector<float>(maxParticles * 2, 0.f);
    //emitter->particles.resize(maxParticles);

    //TextureSystem::loadImageWithAlpha("Assets/particleAtlas.png", &emitter->texture);
    //emitter->textureRows = 8;
    initTexture(emitter, "Assets/particleAtlas.png", 8);
    initMesh(emitter, maxParticles);

    //static const GLfloat g_vertex_buffer_data[] = {
    //    -0.5f, -0.5f, 0.0f, 0.f, 0.f,
    //     0.5f, -0.5f, 0.0f, 1.f, 0.f,
    //    -0.5f,  0.5f, 0.0f, 0.f, 1.f,
    //     0.5f,  0.5f, 0.0f, 0.f, 0.f
    //    };

    //emitter->mesh.m_indices.push_back(0);
    //emitter->mesh.m_indices.push_back(1);
    //emitter->mesh.m_indices.push_back(2);

    //emitter->mesh.m_indices.push_back(2);
    //emitter->mesh.m_indices.push_back(1);
    //emitter->mesh.m_indices.push_back(3);

    //glGenVertexArrays(1, &emitter->mesh.m_VAO);
    //glBindVertexArray(emitter->mesh.m_VAO);

    //glGenBuffers(1, &emitter->mesh.m_VBO);
    //glBindBuffer(GL_ARRAY_BUFFER, emitter->mesh.m_VBO);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data),
    //    g_vertex_buffer_data, GL_STATIC_DRAW);

    //glGenBuffers(1, &emitter->positionBuffer);
    //glBindBuffer(GL_ARRAY_BUFFER, emitter->positionBuffer);
    //glBufferData(GL_ARRAY_BUFFER, maxParticles * 4 * sizeof(float),
    //    NULL, GL_STREAM_DRAW);

    //glGenBuffers(1, &emitter->colorBuffer);
    //glBindBuffer(GL_ARRAY_BUFFER, emitter->colorBuffer);
    //glBufferData(GL_ARRAY_BUFFER, maxParticles * 4 * sizeof(float),
    //    NULL, GL_STREAM_DRAW);

    //glGenBuffers(1, &emitter->uvBlendingBuffer);
    //glBindBuffer(GL_ARRAY_BUFFER, emitter->uvBlendingBuffer);
    //glBufferData(GL_ARRAY_BUFFER, maxParticles * 2 * sizeof(float),
    //    NULL, GL_STREAM_DRAW);

    //// 1rst attribute buffer : vertices
    //glBindBuffer(GL_ARRAY_BUFFER, emitter->mesh.m_VBO);
    //glVertexAttribPointer(
    //    0, 
    //    3, // size
    //    GL_FLOAT, // type
    //    GL_FALSE, // normalized?
    //    5*sizeof(float), // stride
    //    (void*)0 // array buffer offset
    //);
    //glEnableVertexAttribArray(0);

    //// 2nd attribute buffer : vertices
    //glVertexAttribPointer(
    //    1,
    //    2, // size
    //    GL_FLOAT, // type
    //    GL_FALSE, // normalized?
    //    5 * sizeof(float), // stride
    //    (void*)(3*sizeof(float)) // array buffer offset
    //);

    //glEnableVertexAttribArray(1);

    //// 2nd attribute buffer : positions of particles' centers
    //glBindBuffer(GL_ARRAY_BUFFER, emitter->positionBuffer);
    //glVertexAttribPointer(
    //    2,
    //    4, // size : x + y + z + size => 4
    //    GL_FLOAT, // type
    //    GL_FALSE, // normalized?
    //    0, // stride
    //    (void*)0 // array buffer offset
    //);
    //glEnableVertexAttribArray(2);

    //// 3rd attribute buffer : particles' colors
    //glBindBuffer(GL_ARRAY_BUFFER, emitter->colorBuffer);
    //glVertexAttribPointer(
    //    3,
    //    4, // size : r + g + b + a => 4
    //    GL_FLOAT, // type
    //    GL_TRUE, // normalized? *** YES, this means that the unsigned char[4] will be accessible with a vec4 (floats) in the shader ***
    //    0, // stride
    //    (void*)0 // array buffer offset
    //);
    //glEnableVertexAttribArray(3);

    //// 3rd attribute buffer : particles' colors
    //glBindBuffer(GL_ARRAY_BUFFER, emitter->uvBlendingBuffer);
    //glVertexAttribPointer(
    //    4,
    //    2, // size : rows, columns, currentIndex
    //    GL_FLOAT, // type
    //    GL_TRUE, // normalized? *** YES, this means that the unsigned char[4] will be accessible with a vec4 (floats) in the shader ***
    //    0, // stride
    //    (void*)0 // array buffer offset
    //);
    //glEnableVertexAttribArray(4);

    //glBindBuffer(GL_ARRAY_BUFFER, emitter->positionBuffer);
    //glBufferData(GL_ARRAY_BUFFER, emitter->maxParticles * sizeof(float) * 4, NULL, GL_STREAM_DRAW); // Buffer orphaning, a common way to improve streaming perf. See above link for details.

    //glBindBuffer(GL_ARRAY_BUFFER, emitter->colorBuffer);
    //glBufferData(GL_ARRAY_BUFFER, emitter->maxParticles * sizeof(float) * 4, NULL, GL_STREAM_DRAW); // Buffer orphaning, a common way to improve streaming perf. See above link for details.


    //glGenBuffers(1, &emitter->mesh.m_EBO);
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, emitter->mesh.m_EBO);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, emitter->mesh.m_indices.size() * sizeof(GLuint),
    //    emitter->mesh.m_indices.data(), GL_STATIC_DRAW);

    //glBindVertexArray(0);
}

void ParticleSystem::initMesh(ParticleComponent* emitter, uint32 maxParticles)
{
    emitter->positionData = std::vector<float>(maxParticles * 4, 0.f);
    emitter->colorData = std::vector<float>(maxParticles * 4, 0.f);
    emitter->uvBlendingData = std::vector<float>(maxParticles * 2, 0.f);
    emitter->particles.resize(maxParticles);

    static const GLfloat g_vertex_buffer_data[] = {
    -0.5f, -0.5f, 0.0f, 0.f, 0.f,
     0.5f, -0.5f, 0.0f, 1.f, 0.f,
    -0.5f,  0.5f, 0.0f, 0.f, 1.f,
     0.5f,  0.5f, 0.0f, 0.f, 0.f
    };

    emitter->mesh.m_indices.push_back(0);
    emitter->mesh.m_indices.push_back(1);
    emitter->mesh.m_indices.push_back(2);

    emitter->mesh.m_indices.push_back(2);
    emitter->mesh.m_indices.push_back(1);
    emitter->mesh.m_indices.push_back(3);

    glGenVertexArrays(1, &emitter->mesh.m_VAO);
    glBindVertexArray(emitter->mesh.m_VAO);

    glGenBuffers(1, &emitter->mesh.m_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, emitter->mesh.m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data),
        g_vertex_buffer_data, GL_STATIC_DRAW);

    glGenBuffers(1, &emitter->positionBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, emitter->positionBuffer);
    glBufferData(GL_ARRAY_BUFFER, maxParticles * 4 * sizeof(float),
        NULL, GL_STREAM_DRAW);

    glGenBuffers(1, &emitter->colorBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, emitter->colorBuffer);
    glBufferData(GL_ARRAY_BUFFER, maxParticles * 4 * sizeof(float),
        NULL, GL_STREAM_DRAW);

    glGenBuffers(1, &emitter->uvBlendingBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, emitter->uvBlendingBuffer);
    glBufferData(GL_ARRAY_BUFFER, maxParticles * 2 * sizeof(float),
        NULL, GL_STREAM_DRAW);

    // 1rst attribute buffer : vertices
    glBindBuffer(GL_ARRAY_BUFFER, emitter->mesh.m_VBO);
    glVertexAttribPointer(
        0,
        3, // size
        GL_FLOAT, // type
        GL_FALSE, // normalized?
        5 * sizeof(float), // stride
        (void*)0 // array buffer offset
    );
    glEnableVertexAttribArray(0);

    // 2nd attribute buffer : vertices
    glVertexAttribPointer(
        1,
        2, // size
        GL_FLOAT, // type
        GL_FALSE, // normalized?
        5 * sizeof(float), // stride
        (void*)(3 * sizeof(float)) // array buffer offset
    );

    glEnableVertexAttribArray(1);

    // 2nd attribute buffer : positions of particles' centers
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

    // 3rd attribute buffer : particles' colors
    glBindBuffer(GL_ARRAY_BUFFER, emitter->colorBuffer);
    glVertexAttribPointer(
        3,
        4, // size : r + g + b + a => 4
        GL_FLOAT, // type
        GL_TRUE, // normalized? *** YES, this means that the unsigned char[4] will be accessible with a vec4 (floats) in the shader ***
        0, // stride
        (void*)0 // array buffer offset
    );
    glEnableVertexAttribArray(3);

    // 3rd attribute buffer : particles' colors
    glBindBuffer(GL_ARRAY_BUFFER, emitter->uvBlendingBuffer);
    glVertexAttribPointer(
        4,
        2, // size : rows, columns, currentIndex
        GL_FLOAT, // type
        GL_TRUE, // normalized? *** YES, this means that the unsigned char[4] will be accessible with a vec4 (floats) in the shader ***
        0, // stride
        (void*)0 // array buffer offset
    );
    glEnableVertexAttribArray(4);

    glBindBuffer(GL_ARRAY_BUFFER, emitter->positionBuffer);
    glBufferData(GL_ARRAY_BUFFER, emitter->maxParticles * sizeof(float) * 4, NULL, GL_STREAM_DRAW); // Buffer orphaning, a common way to improve streaming perf. See above link for details.

    glBindBuffer(GL_ARRAY_BUFFER, emitter->colorBuffer);
    glBufferData(GL_ARRAY_BUFFER, emitter->maxParticles * sizeof(float) * 4, NULL, GL_STREAM_DRAW); // Buffer orphaning, a common way to improve streaming perf. See above link for details.


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
    auto transformManager = ECS->getComponentManager<TransformComponent>();
    if (!ECS->getComponentManager<ParticleComponent>() || !transformManager)
        return;
    TransformComponent* cameraTransform = transformManager->getComponentChecked(cameraEntity);
    glm::vec3 cameraPosition = glm::vec3(cameraTransform->transform[3]);
    auto& emitters = ECS->getComponentManager<ParticleComponent>()->getComponentArray();
    shader->use();

    glEnable(GL_BLEND);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    static float lastSpawned = 0;
    lastSpawned += deltaTime;
    for (auto& it : emitters)
    {
        if (it.texture.rgbImage)
        {
            glBindTexture(GL_TEXTURE_2D, it.texture.textureID);
        }
        TransformComponent* emitterTransform = transformManager->getComponentChecked(it.entityID);
        //spawnParticles(it, deltaTime, glm::vec3(emitterTransform->transform[3]), cameraPosition);


        spawnParticles(it, deltaTime, glm::vec3(emitterTransform->transform[3]), cameraPosition);
        lastSpawned = 0;
        
        if (it.activeParticles <= 0)
            continue;

        glBlendFunc(it.blendSFactor, it.blendDFactor);

        std::sort(&it.particles[0], &it.particles[it.lastUsedParticle]);
        //std::sort(it.particles.begin(), it.particles.end());
        int breakIndex{};
        for (uint32 i{}; i < it.maxParticles; ++i)
        {
            auto& p = it.particles[i];

            if (!p.active)
            {
                it.lastUsedParticle = i;
                it.activeParticles = i;
                breakIndex = i;
                break;
            }
            p.currentLife -= deltaTime;
            if (p.currentLife <= 0.f)
            {
                p.active = false;
                p.cameraDistance = -100000.f;
            }
            p.velocity += p.acceleration * deltaTime;
            p.position += p.velocity * deltaTime + 0.5f * p.acceleration * deltaTime * deltaTime;

            p.textureIndex = std::lround(((it.particleBlueprint.particle.currentLife - p.currentLife) / it.particleBlueprint.particle.currentLife) * 64);

            p.cameraDistance = glm::length(p.position - cameraPosition);

            it.positionData[4 * i + 0] = p.position.x + emitterTransform->transform[3].x;
            it.positionData[4 * i + 1] = p.position.y + emitterTransform->transform[3].y;
            it.positionData[4 * i + 2] = p.position.z + emitterTransform->transform[3].z;
            it.positionData[4 * i + 3] = p.startSize;

            it.colorData[4 * i + 0] = p.startColor.x;
            it.colorData[4 * i + 1] = p.startColor.y;
            it.colorData[4 * i + 2] = p.startColor.z;
            it.colorData[4 * i + 3] = p.startColor.w;

            it.uvBlendingData[2 * i + 0] = it.textureRows; // Could be uniform
            it.uvBlendingData[2 * i + 1] = p.textureIndex;

        }
        auto& p = it.particles[breakIndex-1];
        //std::cout << "Should break at "<< breakIndex<<". Active particles:"<<it.activeParticles << " \n";
        //std::cout << "x: "<< p.position.x<<" y: "<<p.position.y<<" z: "<<p.position.z << " particles\n";
        //std::cout << "r: "<< p.color.x<<" g: "<<p.color.y<<" b: "<<p.color.z << " \n";

        glBindVertexArray(it.mesh.m_VAO);
        glVertexAttribDivisor(0, 0); // particles vertices : always reuse the same 4 vertices -> 0
        glVertexAttribDivisor(1, 0); // particles UV : always reuse the same 4 UV -> 0
        glVertexAttribDivisor(2, 1); // positions : one per quad (its center) -> 1
        glVertexAttribDivisor(3, 1); // color : one per quad -> 1
        glVertexAttribDivisor(4, 1); // uv blending : one per quad -> 1


        glBindBuffer(GL_ARRAY_BUFFER, it.positionBuffer);
        glBufferSubData(GL_ARRAY_BUFFER, 0, 4*it.activeParticles * sizeof(float), it.positionData.data()); 


        glBindBuffer(GL_ARRAY_BUFFER, it.colorBuffer);
        glBufferSubData(GL_ARRAY_BUFFER, 0, 4*it.activeParticles * sizeof(float), it.colorData.data());

        glBindBuffer(GL_ARRAY_BUFFER, it.uvBlendingBuffer);
        glBufferSubData(GL_ARRAY_BUFFER, 0, 2 * it.activeParticles * sizeof(float), it.uvBlendingData.data());

        glDrawElementsInstanced(GL_TRIANGLES, it.mesh.m_indices.size(),
            GL_UNSIGNED_INT, 0, it.activeParticles);
    }
    glDisable(GL_BLEND);
}

void ParticleSystem::constructQuad(MeshComponent& mesh)
{
    mesh.m_vertices.push_back(Vertex(-0.5f, -0.5f, 0, 0, 0, 0, 0, 0));
    mesh.m_vertices.push_back(Vertex(0.5f, -0.5f, 0, 0, 0, 0, 1, 0));
    mesh.m_vertices.push_back(Vertex(-0.5f, 0.5f, 0, 0, 0, 0, 0, 1));
    mesh.m_vertices.push_back(Vertex(0.5f, 0.5f, 0, 0, 0, 0, 1, 1));

    mesh.m_indices.push_back(0);
    mesh.m_indices.push_back(1);
    mesh.m_indices.push_back(2);

    mesh.m_indices.push_back(2);
    mesh.m_indices.push_back(1);
    mesh.m_indices.push_back(3);
}

void ParticleSystem::spawnParticles(ParticleComponent& emitter, float deltaTime,
    const glm::vec3& emitterLocation, const glm::vec3& cameraLocation)
{
    emitter.timeSinceLastSpawn += deltaTime;
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

    float cameraDistance = glm::length((emitter.particleBlueprint.particle.position + emitterLocation) - cameraLocation);
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
