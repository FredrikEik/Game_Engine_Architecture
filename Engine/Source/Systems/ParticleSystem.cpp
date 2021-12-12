#include "ParticleSystem.h"
#include "../ECSManager.h"
#include "../Shader.h"
#include "TextureSystem.h"
void ParticleSystem::init(uint32 entityID, uint32 maxParticles, uint32 spawnRate,
    const  ParticleComponent::Particle& particleBlueprint, ECSManager* ECS)
{
	ParticleComponent* emitter = ECS->getComponentManager<ParticleComponent>()->getComponentChecked(entityID);
	assert(emitter);
	//constructQuad(emitter->mesh);
    emitter->maxParticles = maxParticles;
    emitter->spawnRate = spawnRate;
    emitter->particleBlueprint = particleBlueprint;
    emitter->particles.reserve(maxParticles);
    emitter->positionData = std::vector<float>(maxParticles * 4, 1.f);
    emitter->colorData = std::vector<float>(maxParticles * 4, 1.f);
    TextureSystem::loadImageWithAlpha("Assets/particleAlpha.png", &emitter->texture);

    for (uint32 i{}; i < maxParticles; ++i)
    {
        emitter->particles.push_back(ParticleComponent::Particle());
    }

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


    // 1rst attribute buffer : vertices
    glBindBuffer(GL_ARRAY_BUFFER, emitter->mesh.m_VBO);
    glVertexAttribPointer(
        0, 
        3, // size
        GL_FLOAT, // type
        GL_FALSE, // normalized?
        5*sizeof(float), // stride
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
        (void*)(3*sizeof(float)) // array buffer offset
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

void ParticleSystem::update(uint32 cameraEntity, class Shader* shader, ECSManager* ECS, float deltaTime)
{
    auto transformManager = ECS->getComponentManager<TransformComponent>();
    TransformComponent* cameraTransform = transformManager->getComponentChecked(cameraEntity);
    glm::vec3 cameraPosition = glm::vec3(cameraTransform->transform[3]);
    auto& emitters = ECS->getComponentManager<ParticleComponent>()->getComponentArray();
    shader->use();

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    static float lastSpawned = 0;
    lastSpawned += deltaTime;
    for (auto& it : emitters)
    {
        if (it.texture.rgbImage)
        {
            glBindTexture(GL_TEXTURE_2D, it.texture.textureID);
        }
        TransformComponent* emitterTransform = transformManager->getComponentChecked(it.entityID);
        spawnParticles(it, deltaTime, glm::vec3(emitterTransform->transform[3]), cameraPosition);

        if (it.activeParticles <= 0)
            continue;
        std::sort(&it.particles[0], &it.particles[it.lastUsedParticle]);
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
            p.life -= deltaTime;
            if (p.life < 0.f)
            {
                p.active = false;
                p.cameraDistance = -1.f;
                //continue;
            }
            p.velocity += p.acceleration * deltaTime;
            p.position += p.velocity * deltaTime + 0.5f * p.acceleration * deltaTime * deltaTime;
            p.position += glm::normalize(p.velocity) * 3.f *deltaTime ;
            
            p.size -= 0.8*deltaTime;
            if (p.size <= 0)
                p.size = 0;

            p.cameraDistance = glm::length(p.position - cameraPosition);

            it.positionData[4 * i + 0] = p.position.x + emitterTransform->transform[3].x;
            it.positionData[4 * i + 1] = p.position.y + emitterTransform->transform[3].y;
            it.positionData[4 * i + 2] = p.position.z + emitterTransform->transform[3].z;
            it.positionData[4 * i + 3] = p.size;

            it.colorData[4 * i + 0] = p.color.x;
            it.colorData[4 * i + 1] = p.color.y;
            it.colorData[4 * i + 2] = p.color.z;
            it.colorData[4 * i + 3] = p.color.w;


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
        glBindBuffer(GL_ARRAY_BUFFER, it.positionBuffer);
        glBufferSubData(GL_ARRAY_BUFFER, 0, 4*it.activeParticles * sizeof(float), it.positionData.data()); 


        glBindBuffer(GL_ARRAY_BUFFER, it.colorBuffer);
        glBufferSubData(GL_ARRAY_BUFFER, 0, 4*it.activeParticles * sizeof(float), it.colorData.data());

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
    // TODO: Fix this shit
    //if (emitter.lastUsedParticle > 10)
    //    return;
    float r, g, b;
    r = std::rand() % 12398;
    r /= 12398.f;

    g = std::rand() % 46578;
    g /= 46578.f;

    b = std::rand() % 13574;
    b /= 13574.f;

    float cameraDistance = glm::length((emitter.particleBlueprint.position + emitterLocation) - cameraLocation);

    uint32 start = (emitter.lastUsedParticle) % emitter.maxParticles;
    for (uint32 i{ start }; i < (start + spawnAmount); ++i)
    {
        // The particles vector acts as a ring buffer
        uint32 index = i % emitter.maxParticles; 
        emitter.particles[index] = emitter.particleBlueprint;
        emitter.particles[index].active = true;
        emitter.particles[index].cameraDistance = cameraDistance;
        emitter.particles[index].color = glm::vec4(r, g, b, 0.5f);
        //emitter.particles[index].size = 1;
        emitter.lastUsedParticle = index+1;
        emitter.activeParticles = emitter.lastUsedParticle;
        emitter.particles[index].acceleration = glm::vec3((std::rand() % 16)-8, (4+std::rand() % 12) * -1, (std::rand() % 16) -8);
    }
}
