#include "ecs.h"

ECS::ECS()
{

}

void ECS::init()
{

}

gsl::Entity ECS::CreateEntity()
{
    return mEntityManager->CreateEntity();
}

void ECS::DestroyEntity(gsl::Entity entity)
{
    mEntityManager->DestroyEntity(entity);

    mComponentManager->EntityDestroyed(entity);

    //mSystemManager->EntityDestroyed(entity);
}

template<typename T>
void ECS::RegisterComponent()
{
    mComponentManager->RegisterComponent<T>();
}

template<typename T>
void ECS::AddComponent(gsl::Entity entity, T component)
{
    mComponentManager->AddComponent<T>(entity, component);

    auto signature = mEntityManager->GetSignature(entity);
    signature.set(mComponentManager->GetComponentType<T>(), true);
    mEntityManager->SetSignature(entity, signature);

    //mSystemManager->EntitySignatureChanged(entity, signature);
}

template<typename T>
void ECS::RemoveComponent(gsl::Entity entity)
{
    mComponentManager->RemoveComponent<T>(entity);

    auto signature = mEntityManager->GetSignature(entity);
    signature.set(mComponentManager->GetComponentType<T>(), false);
    mEntityManager->SetSignature(entity, signature);

    ///mSystemManager->EntitySignatureChanged(entity, signature);

}

template<typename T>
T& ECS::GetComponent(gsl::Entity entity)
{
    return mComponentManager->GetComponent<T>(entity);
}

template<typename T>
gsl::ComponentType ECS::GetComponentType()
{
    return mComponentManager->GetComponentType<T>();
}

// System methods
/*
template<typename T>
std::shared_ptr<T> RegisterSystem()
{
    return mSystemManager->RegisterSystem<T>();
}

template<typename T>
void SetSystemSignature(Signature signature)
{
    mSystemManager->SetSignature<T>(signature);
}
*/
