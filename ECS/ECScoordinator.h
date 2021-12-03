#ifndef ECSCOORDINATOR_H
#define ECSCOORDINATOR_H

#include <iostream>
//#include "components.h"

#include "ECS/entitymanager.h"
#include "ECS/compmanager.h"
#include "ECS/systemmanager.h"
#include "ECS/ResourceManager/resourcemanager.h"

class ECScoordinator
{
public:
    void init()
    {
        //Create a pointer to each manager!
        m_ComponentManager = std::make_unique<ComponentManager>();
        m_SystemManager = std::make_unique<SystemManager>();
        m_EntityManager = EntityManager::Create();
        m_ResourceManager = ResourceManager::Create();

        RegisterComponent<TransformComponent>();
        RegisterComponent<MeshComponent>();
        RegisterComponent<MaterialComponent>();
    }

    //Entity functionality:
    Entity CreateEntity()
    {
        return m_EntityManager->CreateEntity();
    }

    void DestroyEntity(Entity entity)
    {
        m_EntityManager->DestroyEntity(entity);
        m_ComponentManager->DestroyEntity(entity);
        m_SystemManager->EntityDestroyed(entity);
    }

    //Component functionality:
    template<typename T>
    void RegisterComponent()
    {
        m_ComponentManager->RegisterComponent<T>();
    }

    template<typename T>
    void AddComponent(Entity entity, T component)
    {
        m_ComponentManager->AddComponent<T>(entity, component);

        auto signature = m_EntityManager->GetSignature(entity);
        signature.set(m_ComponentManager->GetComponentType<T>(), true);
        m_EntityManager->SetSignature(entity, signature);

        m_SystemManager->EntitySignatureChanged(entity, signature);
    }

    template<typename T>
    void RemoveComponent(Entity entity)
    {
        m_ComponentManager->RemoveComponent<T>(entity);

        auto signature = m_EntityManager->GetSignature(entity);
        signature.set(m_ComponentManager->GetComponentType<T>(), false);
        m_EntityManager->SetSignature(entity, signature);

        m_SystemManager->EntitySignatureChanged(entity, signature);
    }

    template<typename T>
    T& GetComponent(Entity entity)
    {
        return m_ComponentManager->GetComponent<T>(entity);
    }

    template<typename T>
    ComponentType GetComponentType()
    {
        return m_ComponentManager->GetComponentType<T>();
    }

    //System functionality:
    template<typename T>
    std::shared_ptr<T> RegisterSystem()
    {
        return m_SystemManager->RegisterSystem<T>();
    }

    template<typename T>
    void SetSystemSignature(Signature signature)
    {
        m_SystemManager->SetSignature<T>(signature);
    }

    //Object functionality:
    int readObj(std::string filepath)
    {
        return m_ResourceManager->readObj(filepath);
    }

    std::vector<Vertex> getMeshVertices(int meshIndex)
    {
        return m_ResourceManager->getMeshVertices(meshIndex);
    }

private:
    std::unique_ptr<ComponentManager> m_ComponentManager;
    std::unique_ptr<EntityManager> m_EntityManager;
    std::unique_ptr<SystemManager> m_SystemManager;
    std::unique_ptr<ResourceManager> m_ResourceManager;

};

#endif // ECSCOORDINATOR_H
