#ifndef SYSTEMMANAGER_H
#define SYSTEMMANAGER_H

#include <typeinfo>
#include "components.h"
#include "entitymanager.h"
#include "systembase.h"


class SystemManager
{
public:
    template<typename T>
    std::shared_ptr<T> RegisterSystem()
    {
        const char* typeName = typeid(T).name();

        assert(m_Systems.find(typeName) == m_Systems.end() && "Trying to register a system more than once!");

        auto system = std::make_shared<T>();
        m_Systems.insert({typeName, system});

        return system;
    }

    template<typename T>
    void SetSignature(Signature signature)
    {
        const char* typeName = typeid(T).name();

        assert(m_Systems.find(typeName) != m_Systems.end() && "Trying to use a unregistered system!");

        m_Signatures.insert({typeName, signature});
    }

    void EntityDestroyed(Entity entity)
    {
        for (auto const& pair : m_Systems)
        {
            auto const& system = pair.second;

            system->m_Entities.erase(entity);
        }
    }

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
    //Map of system type string pointers to a signature.
    std::unordered_map<const char*, Signature> m_Signatures{};

    //Map of system type string pointers to a system.
    std::unordered_map<const char*, std::shared_ptr<SystemBase>> m_Systems{};
};

#endif // SYSTEMMANAGER_H
