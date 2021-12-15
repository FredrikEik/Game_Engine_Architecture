#ifndef ENTITYMANAGER_H
#define ENTITYMANAGER_H

using Signature = std::bitset<MAX_COMPONENTS>;

//!
//! \brief The EntityManager class, Manages all the entities and their signatures.
//!
//! Has a que of availible IDs for entities to use, as well as signatures of existing entities. Works through the ECS coordinator.
//! The ECS coordinator is a friend of this class.
//!
//! \see ECScoordinator
//!
class EntityManager
{
    friend class ECScoordinator;

public:
    Entity CreateEntity();
    void DestroyEntity(Entity entity);
    void SetSignature(Entity entity, Signature signature);
    Signature GetSignature(Entity entity);

private:
    //! \brief Private constructor!
    //! \see Create()
    EntityManager();

    //! \brief Used by the ECS coordinator to create a unique manager.
    static std::unique_ptr<EntityManager> Create()
    { return std::unique_ptr<EntityManager>( new EntityManager() ); }

    //!Holds availeble Id's for entitys to use.
    std::queue<Entity> m_availableEntityIDs;

    //!Holds the signatures of the existing enteties.
    std::array<Signature, MAX_ENTITIES> m_signatures;

    //!A simple counter used to determined if we went above the maximum allowed enteties.
    Entity m_livingEntitiyCounter{0};
};

#endif // ENTITYMANAGER_H
