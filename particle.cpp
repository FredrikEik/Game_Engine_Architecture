#include "particle.h"
#include "renderwindow.h"
#include "math.h"

particle::particle()
{

}

particle::particle(Camera* camera)
{
    getTransformComponent()->mMatrix.setPosition(0,0,0);
    cameraRef = camera;

}

particle::~particle()
{

}

void particle::init()
{
    //Setter opp shaderprogram
    getMaterialComponent()->mShaderProgram = 4;
    getMaterialComponent()->mTextureUnit = 0;

    //Initializer emitteren
    emitter->maxParticles = 100000;
    emitter->spawnRate = 30;
    emitter->particles.reserve(emitter->maxParticles);
    emitter->positionData = std::vector<float>(maxParticles * 4, 0.f);
    emitter->colorData = std::vector<float>(maxParticles * 8, 0.f);
    emitter->lifeSizeData = std::vector<float>(maxParticles * 4, 0.f);

    for (int i{}; i < maxParticles; ++i)
    {
        emitter->particles.push_back(ParticleComponent::Particle());
    }

    initializeOpenGLFunctions();

//Setter opp vertexer for partikler
    static const GLfloat g_vertex_buffer_data[] = {
        -0.5f, -0.5f, 0.0f, 0.f, 0.f,
         0.5f, -0.5f, 0.0f, 1.f, 0.f,
        -0.5f,  0.5f, 0.0f, 0.f, 1.f,
         0.5f,  0.5f, 0.0f, 0.f, 0.f
        };

    getMeshComponent()->mIndices.push_back(0);
    getMeshComponent()->mIndices.push_back(1);
    getMeshComponent()->mIndices.push_back(2);

    getMeshComponent()->mIndices.push_back(2);
    getMeshComponent()->mIndices.push_back(1);
    getMeshComponent()->mIndices.push_back(3);

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
    glBufferData(GL_ARRAY_BUFFER, maxParticles * 8 * sizeof(float),
        NULL, GL_STREAM_DRAW);

    glGenBuffers(1, &emitter->lifeSizeBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, emitter->lifeSizeBuffer);
    glBufferData(GL_ARRAY_BUFFER, maxParticles * 4 * sizeof(float),
        NULL, GL_STREAM_DRAW);



    //Binder buffere til shaderen

    // 1st attribute buffer : positions of particles' centers
    glBindBuffer(GL_ARRAY_BUFFER, emitter->positionBuffer);
    glVertexAttribPointer(
        0,
        4, // size : x + y + z + size => 4
        GL_FLOAT, // type
        GL_FALSE, // normalized?
        0, // stride
        (void*)0 // array buffer offset
    );
    glEnableVertexAttribArray(0);

    // 2nd attribute buffer : particles' startColors
    glBindBuffer(GL_ARRAY_BUFFER, emitter->colorBuffer);
    glVertexAttribPointer(
        1,
        4, // size : r + g + b + a => 4
        GL_FLOAT, // type
        GL_TRUE, // normalized? *** YES, this means that the unsigned char[4] will be accessible with a vec4 (floats) in the shader ***
        0, // stride
        (void*)0 // array buffer offset
    );
    glEnableVertexAttribArray(1);

    // 3rd attribute buffer : vertices
    glBindBuffer(GL_ARRAY_BUFFER, getMeshComponent()->mVBO);
    glVertexAttribPointer(
        2,
        3, // size
        GL_FLOAT, // type
        GL_FALSE, // normalized?
        5*sizeof(float), // stride
        (void*)0 // array buffer offset
    );
    glEnableVertexAttribArray(2);

    // 4th attribute buffer : lifeAttr
    glBindBuffer(GL_ARRAY_BUFFER, emitter->lifeSizeBuffer);
    glVertexAttribPointer(
        3,
        2, // size : r + g + b + a => 4
        GL_FLOAT, // type
        GL_TRUE, // normalized?
        sizeof(float) * 4, // stride. 8 floats for color start and color end
        (void*)0 // array buffer offset
    );
    glEnableVertexAttribArray(3);

    // 5th attribute buffer : particles' size
    glBindBuffer(GL_ARRAY_BUFFER, emitter->lifeSizeBuffer);
    glVertexAttribPointer(
        4,
        2, // size : start size, end size
        GL_FLOAT, // type
        GL_FALSE, // normalized?
        4 * sizeof(float), // stride 4 floats. Life start - end and size start - end
        (void*)(2 * sizeof(float)) // array buffer offset
    );
    glEnableVertexAttribArray(4);

    // 6th attribute buffer : particles' end colors
    glBindBuffer(GL_ARRAY_BUFFER, emitter->colorBuffer);
    glVertexAttribPointer(
        5,
        4, // size : r + g + b + a => 4
        GL_FLOAT, // type
        GL_TRUE, // normalized?
        sizeof(float) * 8, // stride. 8 floats for color start and color end
        (void*)(4 * sizeof(float)) // array buffer offset
    );
    glEnableVertexAttribArray(5);

    glBindBuffer(GL_ARRAY_BUFFER, emitter->positionBuffer);
    glBufferData(GL_ARRAY_BUFFER, emitter->maxParticles * sizeof(float) * 4, NULL, GL_STREAM_DRAW); // Buffer orphaning, a common way to improve streaming perf. See above link for details.

    glBindBuffer(GL_ARRAY_BUFFER, emitter->colorBuffer);
    glBufferData(GL_ARRAY_BUFFER, emitter->maxParticles * sizeof(float) * 8, NULL, GL_STREAM_DRAW); // Buffer orphaning, a common way to improve streaming perf. See above link for details.

    glBindBuffer(GL_ARRAY_BUFFER, emitter->lifeSizeBuffer);
    glBufferData(GL_ARRAY_BUFFER, emitter->maxParticles * sizeof(float) * 4, NULL, GL_STREAM_DRAW);

    glGenBuffers(1, &getMeshComponent()->mEAB);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, getMeshComponent()->mEAB);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, getMeshComponent()->mIndices.size() * sizeof(GLuint),
    getMeshComponent()->mIndices.data(), GL_STATIC_DRAW);

    glBindVertexArray(0);
}

void particle::update(float deltaTime, Camera* camera)
{


       gsl::Vector3D cameraPosition = gsl::Vector3D(cameraRef->mPosition);

       static float lastSpawned = 0;
       lastSpawned += deltaTime;
           spawnParticles(deltaTime, getTransformComponent()->mMatrix.getPosition(), cameraPosition);
           lastSpawned = 0;

           //qDebug() << emitter->activeParticles;

           //Sorter rekkefølgen av objekter som skal rendres
           std::sort(&emitter->particles[0], &emitter->particles[emitter->maxParticles-1]);
           int breakIndex{};

           for (int i{}; i < emitter->maxParticles; ++i)
           {
               auto& p = emitter->particles[i];

               if (!p.active)
               {
                   emitter->lastParticle = i;
                   //emitter->activeParticles = i;
                   breakIndex = i;
                   break;
               }
               else{
               p.lifeLeft-= deltaTime;
               if (p.lifeLeft <= 0.f)
               {
                   p.active = false;
                   p.cameraDistance = (-100000.f);
               }
               //Regner ut hvor partikler skal flytte seg
               p.velocity += p.acceleration * deltaTime;
               p.position += p.velocity * deltaTime + p.acceleration * deltaTime * deltaTime;
               p.cameraDistance =  sqrt(((getTransformComponent()->mMatrix.getPosition()+p.position)
                                        *(getTransformComponent()->mMatrix.getPosition()+p.position))
                                        +(cameraPosition*cameraPosition));



               //Sett inn posisjon til rendering
               emitter->positionData[4 * i + 0] = getTransformComponent()->mMatrix.getPosition().x + p.position.x;
               emitter->positionData[4 * i + 1] = getTransformComponent()->mMatrix.getPosition().y + p.position.y;
               emitter->positionData[4 * i + 2] = getTransformComponent()->mMatrix.getPosition().z + p.position.z;

               //Sett inn farge til rendering
               emitter->colorData[8 * i + 0] = p.startColor.x;
               emitter->colorData[8 * i + 1] = p.startColor.y;
               emitter->colorData[8 * i + 2] = p.startColor.z;
               emitter->colorData[8 * i + 3] = p.startColor.w;

               emitter->colorData[8 * i + 4] = p.startColor.x;
               emitter->colorData[8 * i + 5] = p.startColor.y;
               emitter->colorData[8 * i + 6] = p.startColor.z;
               emitter->colorData[8 * i + 7] = p.startColor.w;


               emitter->lifeSizeData[4 * i + 0] = p.lifeLeft;
               emitter->lifeSizeData[4 * i + 1] = p.lifeSpan;

               emitter->lifeSizeData[4 * i + 2] = p.startSize;
               emitter->lifeSizeData[4 * i + 3] = p.endSize;

               }

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
    int spawnAmount = emitter->timeSinceLastSpawn * emitter->spawnRate; //Hvor mange som skal lages
    if (spawnAmount <= 0)
        return;
    emitter->timeSinceLastSpawn = 0;
    if (spawnAmount > emitter->spawnRate)
        spawnAmount = emitter->spawnRate;

    // Random float in range
    auto randf = [](const float& min, const float& max) {
        return min + static_cast <float> (std::rand()) / (static_cast <float> (RAND_MAX / (max - min)));
    };

    auto& bp = emitter->particleBlueprint;
    int start = (emitter->lastParticle) % emitter->maxParticles;
    for (int i{ start }; i < (start + spawnAmount); ++i)
    {

        int index = i % emitter->maxParticles;
        auto& particle = emitter->particles[index];

        particle = bp.particle;
        particle.active = true;
        particle.lifeSpan += randf(bp.lifeMinOffset, bp.lifeMaxOffset);
        particle.lifeLeft = particle.lifeSpan;

        float sizeOffset{ randf(bp.sizeMinOffset, bp.sizeMaxOffset) };
        particle.startSize += sizeOffset;
        //sizeOffset = { randf(bp.sizeMinOffset, bp.sizeMaxOffset) };
        particle.endSize += sizeOffset/2;

        particle.startColor = 0;
        particle.startColor += gsl::Vector4D(
            randf(bp.colorMinOffset.x, bp.colorMaxOffset.x),
            randf(bp.colorMinOffset.y, bp.colorMaxOffset.y),
            randf(bp.colorMinOffset.z, bp.colorMaxOffset.z),
            randf(bp.colorMinOffset.w, bp.colorMaxOffset.w));

            particle.endColor = 0;
            particle.endColor += gsl::Vector4D(
            randf(bp.colorMinOffset.x, bp.colorMaxOffset.x),
            randf(bp.colorMinOffset.y, bp.colorMaxOffset.y),
            randf(bp.colorMinOffset.z, bp.colorMaxOffset.z),
            0);

        particle.velocity = 0;
        particle.velocity += gsl::Vector3D(
            randf(bp.velocityMinOffset.x, bp.velocityMaxOffset.x),
            randf(bp.velocityMinOffset.y, bp.velocityMaxOffset.y),
            randf(bp.velocityMinOffset.z, bp.velocityMaxOffset.z)
        );
        particle.acceleration = 0;
        particle.acceleration += gsl::Vector3D(
            randf(bp.accelerationMinOffset.x, bp.accelerationMaxOffset.x),
            randf(bp.accelerationMinOffset.y, bp.accelerationMaxOffset.y),
            randf(bp.accelerationMinOffset.z, bp.accelerationMaxOffset.z)
        );
        particle.position = 0;
        particle.position += gsl::Vector3D(
            randf(bp.positionMinOffset.x, bp.positionMaxOffset.x),
            randf(bp.positionMinOffset.y, bp.positionMaxOffset.y),
            randf(bp.positionMinOffset.z, bp.positionMaxOffset.z)
        );

        emitter->particles[index].cameraDistance = sqrt(((getTransformComponent()->mMatrix.getPosition()+emitter->particles[index].position)
                                                       *(getTransformComponent()->mMatrix.getPosition()+emitter->particles[index].position))
                                                       +(cameraPosition*cameraPosition));
        emitter->lastParticle = index+1;
        emitter->activeParticles = emitter->lastParticle;
    }
}

void particle::draw()
{

    glBindVertexArray(getMeshComponent()->mVAO);
    glVertexAttribDivisor(0, 1); // particles vertices : always reuse the same 4 vertices -> 0
    glVertexAttribDivisor(1, 1); // color
    glVertexAttribDivisor(2, 0); // Vertex
    glVertexAttribDivisor(3, 1); // color : one per quad -> 1
    glVertexAttribDivisor(4, 1); // size
    glVertexAttribDivisor(5, 1); // color : one per quad -> 1


    glBindBuffer(GL_ARRAY_BUFFER, emitter->positionBuffer);
    glBufferSubData(GL_ARRAY_BUFFER, 0, 4*emitter->activeParticles * sizeof(float), emitter->positionData.data());


    glBindBuffer(GL_ARRAY_BUFFER, emitter->colorBuffer);
    glBufferSubData(GL_ARRAY_BUFFER, 0, 8*emitter->activeParticles * sizeof(float), emitter->colorData.data());

    glBindBuffer(GL_ARRAY_BUFFER, emitter->lifeSizeBuffer);
    glBufferSubData(GL_ARRAY_BUFFER, 0, 4 * emitter->activeParticles * sizeof(float), emitter->lifeSizeData.data());


    glDrawElementsInstanced(GL_TRIANGLES, getMeshComponent()->mIndices.size(),
    GL_UNSIGNED_INT, 0, emitter->activeParticles);

}

void particle::move(float x, float y, float z)
{

}


