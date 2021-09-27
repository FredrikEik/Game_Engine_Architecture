#ifndef ECS_H
#define ECS_H

#include "componentmanager.h"
#include "entitymanager.h"
#include "systemmanager.h"
#include "constants.h"


class ECS
{
public:
    ECS();

    void init();

    gsl::Entity CreateEntity();

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
    std::unique_ptr<ComponentManager> mComponentManager;
    std::unique_ptr<EntityManager> mEntityManager;
    //std::unique_ptr<SystemManager> mSystemManager;
};

#endif // ECS_H
