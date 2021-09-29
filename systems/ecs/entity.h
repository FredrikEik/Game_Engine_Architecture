#ifndef ENTITY_H
#define ENTITY_H
#include <QOpenGLFunctions_4_1_Core>


#include <vector>
#include <string>
#include "systems/ecs/Components.h"
#include "systems/ecs/resourcemanager.h"

class RenderWindow;

class Entity : public QOpenGLFunctions_4_1_Core
{
public:
//    Entity(int id);
//    Entity();
    MAX_ENTITIES_TYPE mEntityID{MAX_ENTITIES};

    Entity();
    virtual ~Entity();


    std::string mName{"NoName"};

    struct TransformComponent* mTransform{nullptr};
    struct MeshComponent* mMesh{nullptr};
    struct MaterialComponent* mMaterial{nullptr};
    struct collider *mCollider{nullptr};

    gsl::Vector3D mVelocity{10.f,0.f,0.f};
    gsl::Vector3D defaultPos{0.f,0.f,0.f};


    void setBoundingBox(gsl::Vector3D Min,gsl::Vector3D Max);
    void Collided();
    bool CheckCollide(gsl::Vector3D Pmin, gsl::Vector3D Pmax);

    gsl::Vector3D BoundingBoxMin;
    gsl::Vector3D BoundingBoxMax;


//    void transformX(double in){ this->mTransform->mMatrix.rotateX(in *-1);}
//    void transformY(double in){ this->mTransform->mMatrix.rotateY(in);}
//    void transformZ(double in){ this->mTransform->mMatrix.rotateZ(in);}

    bool isHidden{false}; //if true - do not render
    bool isCollidable{false}; //if true - has collider
    bool isPicked{false}; //picked up by player
    bool isDead{false}; //entity dead?
    bool ignoreFrustum{false};
    bool hit = false;

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
    void move(float x, float y, float z);

};

#endif // ENTITY_H
