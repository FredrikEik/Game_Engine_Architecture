#include "ParticleSystem.h"
#include "../ECSManager.h"
#include "../Shader.h"
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

    for (uint32 i{}; i < maxParticles; ++i)
    {
        emitter->particles.push_back(ParticleComponent::Particle());
    }

    static const GLfloat g_vertex_buffer_data[] = {
 -0.5f, -0.5f, 0.0f,
 0.5f, -0.5f, 0.0f,
 -0.5f, 0.5f, 0.0f,
 0.5f, 0.5f, 0.0f,
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
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, emitter->mesh.m_VBO);
    glVertexAttribPointer(
        0, 
        3, // size
        GL_FLOAT, // type
        GL_FALSE, // normalized?
        0, // stride
        (void*)0 // array buffer offset
    );

    // 2nd attribute buffer : positions of particles' centers
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, emitter->positionBuffer);
    glVertexAttribPointer(
        1,
        4, // size : x + y + z + size => 4
        GL_FLOAT, // type
        GL_FALSE, // normalized?
        0, // stride
        (void*)0 // array buffer offset
    );

    // 3rd attribute buffer : particles' colors
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, emitter->colorBuffer);
    glVertexAttribPointer(
        2,
        4, // size : r + g + b + a => 4
        GL_FLOAT, // type
        GL_TRUE, // normalized? *** YES, this means that the unsigned char[4] will be accessible with a vec4 (floats) in the shader ***
        0, // stride
        (void*)0 // array buffer offset
    );

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
    glVertexAttribDivisor(0, 0); // particles vertices : always reuse the same 4 vertices -> 0
    glVertexAttribDivisor(1, 1); // positions : one per quad (its center) -> 1
    glVertexAttribDivisor(2, 1); // color : one per quad -> 1
    //glEnable(GL_BLEND);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    for (auto& it : emitters)
    {
        TransformComponent* emitterTransform = transformManager->getComponentChecked(it.entityID);
        spawnParticles(it, it.spawnRate * deltaTime);
        if (it.activeParticles <= 0)
            continue;
        std::sort(&it.particles[0], &it.particles[it.lastUsedParticle]);
        for (uint32 i{}; i < it.maxParticles; ++i)
        {
            auto& p = it.particles[i];

            //// Problem with 0 being inactive probably
            //if (!p.active)
            //{
            //    it.lastUsedParticle = i;
            //    break;
            //}
            //p.life -= deltaTime;
            //if (p.life < 0.f)
            //{
            //    p.active = false;
            //    p.cameraDistance = -1.f;
            //    //--it.activeParticles;
            //    //if (it.activeParticles < 0)
            //    //    it.activeParticles = 0;
            //    continue;
            //}
            p.velocity += p.acceleration * deltaTime;
            //p.position += p.velocity * deltaTime + 0.5f * p.acceleration * deltaTime * deltaTime;
            p.position += glm::normalize(p.velocity) * 3.f *deltaTime ;
            
            p.cameraDistance = glm::length(p.position - cameraPosition);

            it.positionData[4 * i + 0] = p.position.x + emitterTransform->transform[3].x;
            it.positionData[4 * i + 1] = p.position.y + emitterTransform->transform[3].y;
            it.positionData[4 * i + 2] = p.position.z + emitterTransform->transform[3].z;
            it.positionData[4 * i + 3] = p.size;

            //it.positionData[4 * i + 0] = 10.f;
            //it.positionData[4 * i + 1] = 10.f;
            //it.positionData[4 * i + 2] = 10.f;
            //it.positionData[4 * i + 3] = 10.f;

            it.colorData[4 * i + 0] = p.color.x;
            it.colorData[4 * i + 1] = p.color.y;
            it.colorData[4 * i + 2] = p.color.z;
            it.colorData[4 * i + 3] = p.color.w;

            //it.positionData[4 * i + 0] = p.position.x + emitterTransform->transform[3].x;
            //it.positionData[4 * i + 1] = p.position.y + emitterTransform->transform[3].y;
            //it.positionData[4 * i + 2] = p.position.z + emitterTransform->transform[3].z;
            //it.positionData[4 * i + 3] = p.size;

            //it.colorData[4 * i + 0] = 0.f;
            //it.colorData[4 * i + 1] = 1.f;
            //it.colorData[4 * i + 2] = 0.f;
            //it.colorData[4 * i + 3] = 1.f;
        }
        auto& p = it.particles[0];
        //std::cout << "Should render "<< it.activeParticles <<" particles\n";
        std::cout << "x: "<< p.position.x<<" y: "<<p.position.y<<" z: "<<p.position.z << " particles\n";



        //glBindBuffer(GL_ARRAY_BUFFER, emitter->positionBuffer);

        //glBindBuffer(GL_ARRAY_BUFFER, emitter->colorBuffer);


        //    // 1rst attribute buffer : vertices
        //glEnableVertexAttribArray(0);
        //glBindBuffer(GL_ARRAY_BUFFER, it.mesh.m_VBO);
        //glVertexAttribPointer(
        //    0,
        //    3, // size
        //    GL_FLOAT, // type
        //    GL_FALSE, // normalized?
        //    0, // stride
        //    (void*)0 // array buffer offset
        //);

        //// 2nd attribute buffer : positions of particles' centers
        //glEnableVertexAttribArray(1);
        //glBindBuffer(GL_ARRAY_BUFFER, it.positionBuffer);
        //glVertexAttribPointer(
        //    1,
        //    4, // size : x + y + z + size => 4
        //    GL_FLOAT, // type
        //    GL_FALSE, // normalized?
        //    0, // stride
        //    (void*)0 // array buffer offset
        //);

        //// 3rd attribute buffer : particles' colors
        //glEnableVertexAttribArray(2);
        //glBindBuffer(GL_ARRAY_BUFFER, it.colorBuffer);
        //glVertexAttribPointer(
        //    2,
        //    4, // size : r + g + b + a => 4
        //    GL_FLOAT, // type
        //    GL_TRUE, // normalized? *** YES, this means that the unsigned char[4] will be accessible with a vec4 (floats) in the shader ***
        //    0, // stride
        //    (void*)0 // array buffer offset
        //);

        // Det er noe funky her, hvor subdata må være 16*float*quads ellerno, idfk
        glBindBuffer(GL_ARRAY_BUFFER, it.positionBuffer);
        glBufferData(GL_ARRAY_BUFFER, it.maxParticles * sizeof(float) * 4, NULL, GL_STREAM_DRAW); // Buffer orphaning, a common way to improve streaming perf. See above link for details.
        glBufferSubData(GL_ARRAY_BUFFER, 0, 4*4 * sizeof(float), it.positionData.data()); 

        glBindBuffer(GL_ARRAY_BUFFER, it.colorBuffer);
        glBufferData(GL_ARRAY_BUFFER, it.maxParticles * sizeof(float) * 4, NULL, GL_STREAM_DRAW); // Buffer orphaning, a common way to improve streaming perf. See above link for details.
        glBufferSubData(GL_ARRAY_BUFFER, 0, 4*4 * sizeof(float), it.colorData.data());

        glBindVertexArray(it.mesh.m_VAO);
        glDrawElementsInstanced(GL_TRIANGLE_STRIP, it.mesh.m_indices.size(),
            GL_UNSIGNED_INT, 0, 4);
    }
    //glDisable(GL_BLEND);
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

void ParticleSystem::spawnParticles(ParticleComponent& emitter, uint32 spawnAmount)
{
    if (spawnAmount > emitter.spawnRate)
        spawnAmount = emitter.spawnRate;
    if (emitter.lastUsedParticle > 10)
        return;
    uint32 start = (emitter.lastUsedParticle) % emitter.maxParticles;
    for (uint32 i{ start }; i < start + spawnAmount; ++i)
    {
        // The particles vector acts as a ring buffer
        uint32 index = i % emitter.maxParticles; 
        emitter.particles[index] = emitter.particleBlueprint;
        emitter.particles[index].active = true;
        emitter.lastUsedParticle = index+1;
        emitter.activeParticles = emitter.lastUsedParticle;
        //emitter.particles[index].acceleration = glm::vec3(std::rand() % 2, std::rand() % 2 * -1, std::rand() % 2);
    }
}
