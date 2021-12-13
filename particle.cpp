#include "particle.h"
#include "renderwindow.h"
#include "math.h"

particle::particle()
{

}

particle::particle(Camera* camera)
{
    getTransformComponent()->mMatrix.setPosition(5,5,5);
    getMeshComponent()->mVertices.push_back(Vertex(-0.5f, -0.5f, 0, 0, 0, 0, 0, 0));
    getMeshComponent()->mVertices.push_back(Vertex(0.5f, -0.5f, 0, 0, 0, 0, 1, 0));
    getMeshComponent()->mVertices.push_back(Vertex(-0.5f, 0.5f, 0, 0, 0, 0, 0, 1));
    getMeshComponent()->mVertices.push_back(Vertex(0.5f, 0.5f, 0, 0, 0, 0, 1, 1));

    getMeshComponent()->mIndices.push_back(0);
    getMeshComponent()->mIndices.push_back(1);
    getMeshComponent()->mIndices.push_back(2);

    getMeshComponent()->mIndices.push_back(2);
    getMeshComponent()->mIndices.push_back(1);
    getMeshComponent()->mIndices.push_back(3);

    cameraRef = camera;

}

particle::~particle()
{

}



void particle::init()
{

    emitter->maxParticles = 10000;
    emitter->spawnRate = 100;
    emitter->particles.reserve(maxParticles);
    emitter->positionData = std::vector<float>(maxParticles * 4, 0.f);
    emitter->colorData = std::vector<float>(maxParticles * 4, 0.f);

    for (int i{}; i < maxParticles; ++i)
    {
        emitter->particles.push_back(ParticleComponent::Particle());
    }

    initializeOpenGLFunctions();

    static const GLfloat g_vertex_buffer_data[] = {
        -0.5f, -0.5f, 0.0f, 0.f, 0.f,
         0.5f, -0.5f, 0.0f, 1.f, 0.f,
        -0.5f,  0.5f, 0.0f, 0.f, 1.f,
         0.5f,  0.5f, 0.0f, 0.f, 0.f
        };

    glGenVertexArrays(1, &getMeshComponent()->mVAO);
    glBindVertexArray(getMeshComponent()->mVAO);

    glGenBuffers(1, &getMeshComponent()->mVBO);
    glBindBuffer(GL_ARRAY_BUFFER, getMeshComponent()->mVBO);
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
    glBindBuffer(GL_ARRAY_BUFFER, getMeshComponent()->mVBO);
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


    glGenBuffers(1, &getMeshComponent()->mEAB);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, getMeshComponent()->mEAB);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, getMeshComponent()->mIndices.size() * sizeof(GLuint),
    getMeshComponent()->mIndices.data(), GL_STATIC_DRAW);

    glBindVertexArray(0);
}

void particle::update(float deltaTime, Camera* camera)
{

       gsl::Vector3D cameraPosition = gsl::Vector3D(cameraRef->mPosition);
       //qDebug()<< cameraPosition;

       static float lastSpawned = 0;
       lastSpawned += deltaTime;
           spawnParticles(deltaTime, getTransformComponent()->mMatrix.getPosition(), cameraPosition);
           lastSpawned = 0;

           std::sort(&emitter->particles[0], &emitter->particles[emitter->lastParticle]);
           int breakIndex{};

           for (int i{}; i < emitter->maxParticles; ++i)
           {
               auto& p = emitter->particles[i];

               if (!p.active)
               {
                   emitter->lastParticle = i;
                   emitter->activeParticles = i;
                   breakIndex = i;
                   break;
               }
               p.lifeSpan-= deltaTime;
               if (p.lifeSpan < 0.f)
               {
                   p.active = false;
                   p.cameraDistance = -1.f;
               }
               p.velocity += p.acceleration * deltaTime;
               p.position += p.velocity * deltaTime + p.acceleration * deltaTime * deltaTime;
               //qDebug() << p.position;
               p.cameraDistance =  sqrt((p.position*p.position) + (cameraPosition*cameraPosition));

               emitter->positionData[4 * i + 0] = p.position.x + getTransformComponent()->mMatrix.getPosition().x;
               emitter->positionData[4 * i + 1] = p.position.y + getTransformComponent()->mMatrix.getPosition().y;
               emitter->positionData[4 * i + 2] = p.position.z + getTransformComponent()->mMatrix.getPosition().z;
               emitter->positionData[4 * i + 3] = p.size;

               emitter->colorData[4 * i + 0] = p.color.x;
               emitter->colorData[4 * i + 1] = p.color.y;
               emitter->colorData[4 * i + 2] = p.color.z;
               emitter->colorData[4 * i + 3] = p.color.w;

           }
           //auto& p = particleComp->particles[breakIndex-1];
           //std::cout << "Should break at "<< breakIndex<<". Active particles:"<<it.activeParticles << " \n";
           //std::cout << "x: "<< p.position.x<<" y: "<<p.position.y<<" z: "<<p.position.z << " particles\n";
           //std::cout << "r: "<< p.color.x<<" g: "<<p.color.y<<" b: "<<p.color.z << " \n";

           draw();

       }

void particle::spawnParticles(float deltaTime, gsl::Vector3D emitterPosition, gsl::Vector3D cameraPosition)
{
    emitter->timeSinceLastSpawn += deltaTime;
    int spawnAmount = emitter->timeSinceLastSpawn * emitter->spawnRate;
    if (spawnAmount <= 0)
        return;
    emitter->timeSinceLastSpawn = 0;
    if (spawnAmount > emitter->spawnRate)
        spawnAmount = emitter->spawnRate;

    // Random float in range
    auto randf = [](const float& min, const float& max) {
        return min + static_cast <float> (std::rand()) / (static_cast <float> (RAND_MAX / (max - min)));
    };

    float cameraDistance = sqrt((emitter->particleBlueprint.particle.position)*(emitter->particleBlueprint.particle.position)
                                - (cameraPosition*cameraPosition));
    auto& bp = emitter->particleBlueprint;
    int start = (emitter->lastParticle) % emitter->maxParticles;
    for (int i{ start }; i < (start + spawnAmount); ++i)
    {

        int index = i % emitter->maxParticles;
        auto& particle = emitter->particles[index];

        particle = bp.particle;
        particle.active = true;

        particle.timeAlive += randf(bp.lifeMinOffset, bp.lifeMaxOffset);
        particle.lifeSpan = particle.timeAlive;
        float sizeOffset{ randf(bp.sizeMinOffset, bp.sizeMaxOffset) };
        particle.size += sizeOffset;

        particle.color += gsl::Vector4D(
            randf(bp.colorMinOffset.x, bp.colorMaxOffset.x),
            randf(bp.colorMinOffset.y, bp.colorMaxOffset.y),
            randf(bp.colorMinOffset.z, bp.colorMaxOffset.z),
            randf(bp.colorMinOffset.w, bp.colorMaxOffset.w));

        particle.color += gsl::Vector4D(
            randf(bp.colorMinOffset.x, bp.colorMaxOffset.x),
            randf(bp.colorMinOffset.y, bp.colorMaxOffset.y),
            randf(bp.colorMinOffset.z, bp.colorMaxOffset.z),
            randf(bp.colorMinOffset.w, bp.colorMaxOffset.w));

        particle.velocity += gsl::Vector3D(
            randf(bp.velocityMinOffset.x, bp.velocityMaxOffset.x),
            randf(bp.velocityMinOffset.y, bp.velocityMaxOffset.y),
            randf(bp.velocityMinOffset.z, bp.velocityMaxOffset.z)
        );

        particle.acceleration += gsl::Vector3D(
            randf(bp.accelerationMinOffset.x, bp.accelerationMaxOffset.x),
            randf(bp.accelerationMinOffset.y, bp.accelerationMaxOffset.y),
            randf(bp.accelerationMinOffset.z, bp.accelerationMaxOffset.z)
        );

        particle.position += gsl::Vector3D(
            randf(bp.positionMinOffset.x, bp.positionMaxOffset.x),
            randf(bp.positionMinOffset.y, bp.positionMaxOffset.y),
            randf(bp.positionMinOffset.z, bp.positionMaxOffset.z)
        );

        particle.cameraDistance = sqrt((emitter->particleBlueprint.particle.position)*(emitter->particleBlueprint.particle.position)
                                       + (cameraPosition*cameraPosition));
        //qDebug() << cameraPosition;
        //qDebug() << emitter->particleBlueprint.particle.position;
        qDebug() << particle.cameraDistance;
        emitter->lastParticle = index+1;
        emitter->activeParticles = emitter->lastParticle;
    }
}

void particle::draw()
{

    glBindVertexArray(getMeshComponent()->mVAO);
    glVertexAttribDivisor(0, 0); // particles vertices : always reuse the same 4 vertices -> 0
    glVertexAttribDivisor(1, 0); // particles UV : always reuse the same 4 UV -> 0
    glVertexAttribDivisor(2, 1); // positions : one per quad (its center) -> 1
    glVertexAttribDivisor(3, 1); // color : one per quad -> 1


    glBindBuffer(GL_ARRAY_BUFFER, emitter->positionBuffer);
    glBufferSubData(GL_ARRAY_BUFFER, 0, 4*emitter->activeParticles * sizeof(float), emitter->positionData.data());


    glBindBuffer(GL_ARRAY_BUFFER, emitter->colorBuffer);
    glBufferSubData(GL_ARRAY_BUFFER, 0, 4*emitter->activeParticles * sizeof(float), emitter->colorData.data());

    glDrawElementsInstanced(GL_TRIANGLES, getMeshComponent()->mIndices.size(),
        GL_UNSIGNED_INT, 0, emitter->activeParticles);
}

void particle::move(float x, float y, float z)
{

}


