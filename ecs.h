#ifndef ECS_H
#define ECS_H

#include "componentmanager.h"
#include "entitymanager.h"
#include "resourcemanager.h"
#include "componenttypes.h"
#include <QOpenGLFunctions_4_1_Core>
#include "constants.h"


class ECS : public QOpenGLFunctions_4_1_Core
{
public:
    ECS();

    void init();

    gsl::Entity CreateEntity();

    void InitEntityMesh(gsl::Entity entity);

    void DrawEntity(GLuint &vao, GLenum drawMode);

    void DestroyEntity(gsl::Entity entity);

    template<typename T>
    void RegisterComponent();

    template<typename T>
    void AddComponent(gsl::Entity entity, T component);

    template<typename T>
    void RemoveComponent(gsl::Entity entity);

    template<typename T>
    T& GetComponent(gsl::Entity entity);

    template<typename T>
    gsl::ComponentType GetComponentType();



private:
    std::unique_ptr<ComponentManager> mComponentManager{};
    std::unique_ptr<EntityManager> mEntityManager{};
    //std::unique_ptr<SystemManager> mSystemManager;
};

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

#endif // ECS_H
