#ifndef SYSTEMMANAGER_H
#define SYSTEMMANAGER_H

#include "entitymanager.h"
#include "systembase.h"

//!
//! \brief The SystemManager class, A manager for all the systems in the project.
//!
//! It uses bitsets as a way to recognize entities that has the correct amount of components the systems cares about.
//! The entity does not need to have the exact amount of components, as long as it has the components the system needs.
//!
class SystemManager
{
public:
    //!
    //! \brief RegisterSystem(), Registers a system of type <T>.
    //! \return Shared pointer to the system registered.
    //!
    //! It also inserts the system in an unordered map to keep track of them.
    //!
    template<typename T>
    std::shared_ptr<T> RegisterSystem()
    {
        const char* typeName = typeid(T).name();

        assert(m_Systems.find(typeName) == m_Systems.end() && "Trying to register a system more than once!");

        auto system = std::make_shared<T>();
        m_Systems.insert({typeName, system});

        return system;
    }

    //!
    //! \brief SetSignature(), sets the signature of the system. It determines what kind of components it cares about.
    //! \param signature - The signature the system <T> cares about.
    //!
    template<typename T>
    void SetSignature(Signature signature)
    {
        const char* typeName = typeid(T).name();

        assert(m_Systems.find(typeName) != m_Systems.end() && "Trying to use a unregistered system!");

        m_Signatures.insert({typeName, signature});
    }

    //!
    //! \brief EntityDestroyed(), This function removes the destroyed entity from being tracked by the systems.
    //! \param entity - The destroyed entity.
    //!
    void EntityDestroyed(Entity entity)
    {
        for (auto const& pair : m_Systems)
        {
            auto const& system = pair.second;

            system->m_Entities.erase(entity);
        }
    }

    //!
    //! \brief EntitySignatureChanged(), notifies the systems that an entity signature changed.
    //! \param entity - The entity that changed signature.
    //! \param entitySignature - The new signature of the entity.
    //!
    //! This can happen if a component is removed OR added.
    //!
    void EntitySignatureChanged(Entity entity, Signature entitySignature)
    {
        //Notify our systems that an entity's signature has changed.
        for (auto const& pair : m_Systems)
        {
            auto const& type = pair.first;
            auto const& system = pair.second;
            auto const& systemSignature = m_Signatures[type];

            //If the entity matches the system signature, insert it into the set.
            if ((entitySignature & systemSignature) == systemSignature)
                system->m_Entities.insert(entity);
            //Erase if it don't match.
            else
                system->m_Entities.erase(entity);
        }
    }

private:
    //!Map of system type string pointers to a signature.
    std::unordered_map<const char*, Signature> m_Signatures{};

    //!Map of system type string pointers to a system.
    std::unordered_map<const char*, std::shared_ptr<SystemBase>> m_Systems{};
};

#endif // SYSTEMMANAGER_H
