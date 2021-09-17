#ifndef ENTITY_H
#define ENTITY_H

template<class T>
class Entity
{
public:
    Entity();
    ~Entity();

    int getEntityID(){ return entityID; }
    int getEntityTypeID() { return entityTypeID; }

private:

   const int entityID;
   const int entityTypeID;
};

#endif // ENTITY_H
