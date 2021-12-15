#ifndef ECSCOORDINATOR_H
#define ECSCOORDINATOR_H

#include "ECS/entitymanager.h"
#include "ECS/compmanager.h"
#include "ECS/systemmanager.h"
#include "ECS/ResourceManager/resourcemanager.h"

//!
//! \brief The ECScoordinator class, Is in charge of the ECS structure and its connection the rest of the project.
//!
//! This is the class that talks to all the managers and makes sure they all coordinate with each other.
//! Needs to be unique to prevent unexpected behaviour. Make sure to init() before starting to use.
//!
//! \see init()
class ECScoordinator
{
public:
    //!
    //! \brief Init(), initializing the coordinator.
    //!
    //! Registers all the components and systems the engine uses. Needs to be called before using any of the other functions!
    //!
    void Init()
    {
        //Create a pointer to each manager!
        m_ComponentManager = std::make_unique<ComponentManager>();
        m_SystemManager = std::make_unique<SystemManager>();
        m_EntityManager = EntityManager::Create();
        m_ResourceManager = ResourceManager::Create();

        //Register systems!
        RegisterComponent<TransformComponent>();
        RegisterComponent<MeshComponent>();
        RegisterComponent<MaterialComponent>();

    }

    //Entity functionality:

    //!
    //! \brief CreateEntity(), Creates a new entity through the entity manager.
    //! \return Returns the entity ID(GLuint).
    //!
    Entity CreateEntity()
    {
        return m_EntityManager->CreateEntity();
    }

    //!
    //! \brief DestroyEntity(), Destroys the entity passed in.
    //! \param entity - The entity that needs deleting.
    //!
    //! Makes sure the entity is removede from all the managers lists.
    //!
    void DestroyEntity(Entity entity)
    {
        m_EntityManager->DestroyEntity(entity);
        m_ComponentManager->DestroyEntity(entity);
        m_SystemManager->EntityDestroyed(entity);
    }

    //Component functionality:

    //!
    //! \brief RegisterComponent(), Register a component <T> with the component manager.
    //!
    //! Registers a new type of component, only needs to be done once per component.
    //!
    //! \typedef T - The type of component that is registered.
    //!
    template<typename T>
    void RegisterComponent()
    {
        m_ComponentManager->RegisterComponent<T>();
    }

    //!
    //! \brief AddComponent(), Adds a component to an entity.
    //!
    //! \param entity - The entity that gets a component.
    //! \param component - The type of component the entity gets.
    //!
    template<typename T>
    void AddComponent(Entity entity, T component)
    {
        m_ComponentManager->AddComponent<T>(entity, component);

        auto signature = m_EntityManager->GetSignature(entity);
        signature.set(m_ComponentManager->GetComponentType<T>(), true);
        m_EntityManager->SetSignature(entity, signature);

        m_SystemManager->EntitySignatureChanged(entity, signature);
    }

    //!
    //! \brief RemoveComponent(), Removes the component <T> from the array and changes the signarture of the entity.
    //! \param entity - The entity that gets a component removed.
    //!
    //! Finds the correct array from <T> and updates the signature of the entity.
    //! Then notifies the system manager that the entity's signature changed so it can update it aswell.
    //!
    //! \see ComponentManager::RemoveComponent()
    template<typename T>
    void RemoveComponent(Entity entity)
    {
        m_ComponentManager->RemoveComponent<T>(entity);

        auto signature = m_EntityManager->GetSignature(entity);
        signature.set(m_ComponentManager->GetComponentType<T>(), false);
        m_EntityManager->SetSignature(entity, signature);

        m_SystemManager->EntitySignatureChanged(entity, signature);
    }

    //!
    //! \brief GetComponent(), Gets a component of type <T> from the component manager.
    //! \param entity - The owner of the wanted component.
    //! \return Returns the entity's component of the give type <T>.
    //!
    //! \see ComponentManager::GetComponent()
    //!
    template<typename T>
    T& GetComponent(Entity entity)
    {
        return m_ComponentManager->GetComponent<T>(entity);
    }

    //!
    //! \brief GetComponentType(), Gives you the type of a component.
    //! \return The type of a component <T>.
    //!
    //! \see ComponentManager::GetComponentType()
    //!
    template<typename T>
    ComponentType GetComponentType()
    {
        return m_ComponentManager->GetComponentType<T>();
    }

    //System functionality:

    //!
    //! \brief RegisterSystem(), Registers a system with the system manager.
    //! \return A shared pointer to the system.
    //!
    //! \see SystemManager::RegisterSystem()
    //!
    template<typename T>
    std::shared_ptr<T> RegisterSystem()
    {
        return m_SystemManager->RegisterSystem<T>();
    }

    //!
    //! \brief SetSystemSignature(), Sets the signature of a system.
    //! \param signature - The signature you want the system <T> to have.
    //!
    //! The signature is a std::bitset.
    //!
    //! \see SystemManager::SetSignature()
    //!
    template<typename T>
    void SetSystemSignature(Signature signature)
    {
        m_SystemManager->SetSignature<T>(signature);
    }

    //Object functionality:

    //!
    //! \brief readObj(), Reads a object from file into memory.
    //! \param filepath - Where the object is stored on the harddrive.
    //! \return An index to the mesh in the resource manager.
    //!
    //! \see ResourceManager::readObj()
    //!
    int readObj(std::string filepath)
    {
        return m_ResourceManager->readObj(filepath);
    }

    //!
    //! \brief getMeshData(), Gives you the data of a mesh at an index.
    //! \param MeshIndex - Where the mesh is in the array.
    //! \return Reference to the data of the mesh.
    //!
    //! \see ResourceManager::getMeshdata()
    //!
    MeshData getMeshData(int MeshIndex)
    {
        return m_ResourceManager->getMeshdata(MeshIndex);
    }

private:
    //unique pointers to all the managers.
    std::unique_ptr<ComponentManager> m_ComponentManager;
    std::unique_ptr<EntityManager> m_EntityManager;
    std::unique_ptr<SystemManager> m_SystemManager;
    std::unique_ptr<ResourceManager> m_ResourceManager;

};

#endif // ECSCOORDINATOR_H
