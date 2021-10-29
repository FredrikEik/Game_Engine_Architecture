#include "ecs.h"

ECS::ECS()
{

}

void ECS::init()
{
    mComponentManager = std::make_unique<ComponentManager>();
    mEntityManager = std::make_unique<EntityManager>();
    //mSystemManager = std::make_unique<SystemManager>();
}

gsl::Entity ECS::CreateEntity()
{
    qDebug() << "Living entities: " << mEntityManager->getLivingEntities();
    return mEntityManager->CreateEntity();
}
void ECS::DrawEntity(GLuint &vao, GLenum drawMode)
{
    glBindVertexArray( vao );
    glDrawArrays(drawMode, 0, 3);
    glBindVertexArray(0);
}
void ECS::InitEntityMesh(gsl::Entity entity)
{
    initializeOpenGLFunctions();

    Mesh &meshComp = GetComponent<Mesh>(entity);

    glGenVertexArrays( 1, &meshComp.mVAO );
    glBindVertexArray( meshComp.mVAO );

    //Vertex Buffer Object to hold vertices - VBO
    glGenBuffers( 1, &meshComp.mVBO );
    glBindBuffer( GL_ARRAY_BUFFER, meshComp.mVBO );

    //Vertex Buffer Object to hold vertices - VBO
    glBufferData( GL_ARRAY_BUFFER, meshComp.mVertices.size()*sizeof( Vertex ), meshComp.mVertices.data(), GL_STATIC_DRAW );

    // 1rst attribute buffer : vertices
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0  );          // array buffer offset
    glEnableVertexAttribArray(0);

    // 2nd attribute buffer : colors
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,  sizeof(Vertex),  (GLvoid*)(3 * sizeof(GLfloat)) );
    glEnableVertexAttribArray(1);

    // 3rd attribute buffer : uvs
    glVertexAttribPointer(2, 2,  GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)( 6 * sizeof(GLfloat)) );
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
}

void ECS::DestroyEntity(gsl::Entity entity)
{
    mEntityManager->DestroyEntity(entity);

    mComponentManager->EntityDestroyed(entity);

    //mSystemManager->EntityDestroyed(entity);
}

