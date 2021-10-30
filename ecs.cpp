#include "ecs.h"

ECS::ECS()
{

}

void ECS::init()
{
    mComponentManager = std::make_unique<ComponentManager>();
    mEntityManager = std::make_unique<EntityManager>();
    mResourceManager = std::make_unique<ResourceManager>();
    //mSystemManager = std::make_unique<SystemManager>();
}

gsl::Entity ECS::CreateEntity()
{
    return mEntityManager->CreateEntity();
}
void ECS::DrawEntity(gsl::Entity entity)
{
    Mesh mesh = GetComponent<Mesh>(entity);
    glBindVertexArray(mesh.mVAO);
    /*
    if(indexCount > 0)
    {
        glDrawElements(drawMode, vertexCount,)
    }
    */
    glDrawArrays(mesh.mDrawType, 0, mesh.mVertexCount);
    glBindVertexArray(0);
}

void ECS::DestroyEntity(gsl::Entity entity)
{
    mEntityManager->DestroyEntity(entity);

    mComponentManager->EntityDestroyed(entity);

    //mSystemManager->EntityDestroyed(entity);
}

void ECS::addMesh(std::string filePath)
{
    mResourceManager->addMesh(filePath);
}

Mesh ECS::getMesh(std::string filePath)
{
    return mResourceManager->getMesh(filePath);
}

