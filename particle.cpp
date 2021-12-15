//#include "particle.h"
//#include "rendersystem.h"
//#include "math.h"
//#include "components.h"
//#include "resourcemanager.h"
//#include "shaderhandler.h"
//#include "texture.h"

/********************************************//**
* ... Was starting on a particle system, but didnt have time to
* complete or use it. took inspiration from johannes in class and tried to make it fit my engine
* but most of this is just me starting a bit late on working on particle for the engine
* cant take any credit for this.
***********************************************/
//using namespace ecs;

//particle::particle()
//{

//}
//particle::particle(Camera* camera)
//{
//    ecs::TransformComponent().mMatrix.setPosition(1,1,1);

//    cameraRef = camera;
//}
//particle::~particle()
//{

//}
//void particle::init()
//{
//    MaterialComponent().mShaderProgram[gsl::PARTICLESHADER];

//    emitter->maxParticles = 100000;
//    emitter->spawnRate = 100;
//    emitter->particles.reserve(emitter->maxParticles);
//    emitter->positionData = std::vector<float>(maxParticles * 4, 0.f);
//    emitter->colorData = std::vector<float>(maxParticles * 4, 0.f);

//    for (int i{}; i < maxParticles; ++i)
//    {
//        emitter->particles.push_back(ParticleComponent::Particle());
//    }

//    initializeOpenGLFunctions();

//    static const GLfloat g_vertex_buffer_data[] = {
//        -0.5f, -0.5f, 0.0f, 0.f, 0.f,
//         0.5f, -0.5f, 0.0f, 1.f, 0.f,
//        -0.5f,  0.5f, 0.0f, 0.f, 1.f,
//         0.5f,  0.5f, 0.0f, 0.f, 0.f
//        };

//    MeshComponent()->mIndices.push_back(0);
//    MeshComponent()->mIndices.push_back(1);
//    MeshComponent()->mIndices.push_back(2);

//    getMeshComponent()->mIndices.push_back(2);
//    getMeshComponent()->mIndices.push_back(1);
//    getMeshComponent()->mIndices.push_back(3);

//    glGenVertexArrays(1, &getMeshComponent()->mVAO);
//    glBindVertexArray(getMeshComponent()->mVAO);

//    glGenBuffers(1, &getMeshComponent()->mVBO);
//    glBindBuffer(GL_ARRAY_BUFFER, getMeshComponent()->mVBO);
//    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data),
//        g_vertex_buffer_data, GL_STATIC_DRAW);

//    glGenBuffers(1, &emitter->positionBuffer);
//    glBindBuffer(GL_ARRAY_BUFFER, emitter->positionBuffer);
//    glBufferData(GL_ARRAY_BUFFER, maxParticles * 4 * sizeof(float),
//        NULL, GL_STREAM_DRAW);

//    glGenBuffers(1, &emitter->colorBuffer);
//    glBindBuffer(GL_ARRAY_BUFFER, emitter->colorBuffer);
//    glBufferData(GL_ARRAY_BUFFER, maxParticles * 4 * sizeof(float),
//        NULL, GL_STREAM_DRAW);


//    // 1rst attribute buffer : positions of particles' centers
//    glBindBuffer(GL_ARRAY_BUFFER, emitter->positionBuffer);
//    glVertexAttribPointer(
//        0,
//        4, // size : x + y + z + size => 4
//        GL_FLOAT, // type
//        GL_FALSE, // normalized?
//        0, // stride
//        (void*)0 // array buffer offset
//    );
//    glEnableVertexAttribArray(0);

//    // 2nd attribute buffer : particles' colors
//    glBindBuffer(GL_ARRAY_BUFFER, emitter->colorBuffer);
//    glVertexAttribPointer(
//        1,
//        4, // size : r + g + b + a => 4
//        GL_FLOAT, // type
//        GL_TRUE, // normalized? *** YES, this means that the unsigned char[4] will be accessible with a vec4 (floats) in the shader ***
//        0, // stride
//        (void*)0 // array buffer offset
//    );
//    glEnableVertexAttribArray(1);

//    // 3rd attribute buffer : vertices
//    glBindBuffer(GL_ARRAY_BUFFER, getMeshComponent()->mVBO);
//    glVertexAttribPointer(
//        2,
//        3, // size
//        GL_FLOAT, // type
//        GL_FALSE, // normalized?
//        5*sizeof(float), // stride
//        (void*)0 // array buffer offset
//    );
//    glEnableVertexAttribArray(2);

//    glBindBuffer(GL_ARRAY_BUFFER, emitter->positionBuffer);
//    glBufferData(GL_ARRAY_BUFFER, emitter->maxParticles * sizeof(float) * 4, NULL, GL_STREAM_DRAW); // Buffer orphaning, a common way to improve streaming perf. See above link for details.

//    glBindBuffer(GL_ARRAY_BUFFER, emitter->colorBuffer);
//    glBufferData(GL_ARRAY_BUFFER, emitter->maxParticles * sizeof(float) * 4, NULL, GL_STREAM_DRAW); // Buffer orphaning, a common way to improve streaming perf. See above link for details.


//    glGenBuffers(1, &getMeshComponent()->mEAB);
//    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, getMeshComponent()->mEAB);
//    glBufferData(GL_ELEMENT_ARRAY_BUFFER, getMeshComponent()->mIndices.size() * sizeof(GLuint),
//    getMeshComponent()->mIndices.data(), GL_STATIC_DRAW);

//    glBindVertexArray(0);
//}



//void particle::draw()
//{

//    glBindVertexArray(MeshComponent().mVAO);
//    glVertexAttribDivisor(0, 1); // particles vertices : always reuse the same 4 vertices -> 0
//    glVertexAttribDivisor(1, 1); // positions : one per quad (its center) -> 1
//    glVertexAttribDivisor(2, 0); // color : one per quad -> 1


//    glBindBuffer(GL_ARRAY_BUFFER, emitter->positionBuffer);
//    glBufferData(GL_ARRAY_BUFFER, emitter->maxParticles * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);
//    glBufferSubData(GL_ARRAY_BUFFER, 0, 4*1 * sizeof(float), emitter->positionData.data());


//    glBindBuffer(GL_ARRAY_BUFFER, emitter->colorBuffer);
//    glBufferData(GL_ARRAY_BUFFER, emitter->maxParticles * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);
//    glBufferSubData(GL_ARRAY_BUFFER, 0, 4*1 * sizeof(float), emitter->colorData.data());

//    glDrawElementsInstanced(GL_TRIANGLES, getMeshComponent()->mIndices.size(),
//    GL_UNSIGNED_INT, 0, 1);

//}

//void particle::move(float x, float y, float z)
//{

//}


