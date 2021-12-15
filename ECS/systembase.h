#ifndef SYSTEM_H
#define SYSTEM_H

//Base for all systems in the engine. Each system keeps track of the
//enteties it cares about in a set.
class SystemBase
{
public:
    std::set<Entity> m_Entities;
};

#endif // SYSTEM_H
