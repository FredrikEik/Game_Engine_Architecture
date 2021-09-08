#ifndef ENTITY_H
#define ENTITY_H


#include <vector>
#include <string>
#include "Components.h"

class Entity
{
public:
    Entity(int id);
    MAX_ENTITIES_TYPE mEntityID{MAX_ENTITIES};
        std::string mName{"NoName"};

    TransformComponent mTransform;
    MeshComponent mMesh;

    gsl::Vector3D mVelocity{0.f,0.f,0.f};
    gsl::Vector3D defaultPos{0.f,0.f,0.f};

    bool isHidden{false}; //if true - do not render
    bool isCollidable{false}; //if true - has collider
    bool isPicked{false}; //picked up by player
    bool isDead{false};
    int health;

    void setHealth(int hpInput)
    {
        health += hpInput;
        if (health <= 0)
            isDead = true;
    }
    int getHealth()
    {
        return health;
    }
    void move()
    {
        mTransform.mMatrix.getPosition() += mVelocity;
    }

};

#endif // ENTITY_H
