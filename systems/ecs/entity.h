#ifndef ENTITY_H
#define ENTITY_H
#include <QOpenGLFunctions_4_1_Core>


#include <vector>
#include <string>
#include "Components.h"
class RenderWindow;

class Entity : public QOpenGLFunctions_4_1_Core
{
public:
//    Entity(int id);
//    Entity();
//    MAX_ENTITIES_TYPE mEntityID{MAX_ENTITIES};

    Entity();
    virtual ~Entity();

    std::string mName{"NoName"};

    struct TransformComponent* mTransform{nullptr};
    struct MeshComponent* mMesh{nullptr};
    struct MaterialComponent* mMaterial{nullptr};

    gsl::Vector3D mVelocity{10.f,0.f,0.f};
    gsl::Vector3D defaultPos{0.f,0.f,0.f};

//    void transformX(double in){ this->mTransform->mMatrix.rotateX(in *-1);}
//    void transformY(double in){ this->mTransform->mMatrix.rotateY(in);}
//    void transformZ(double in){ this->mTransform->mMatrix.rotateZ(in);}

    bool isHidden{false}; //if true - do not render
    bool isCollidable{false}; //if true - has collider
    bool isPicked{false}; //picked up by player
    bool isDead{false};
    bool ignoreFrustum{false};
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
       // mTransform->mMatrix.getPosition() += mVelocity;
        mTransform->mMatrix.setPosition(100.f, 10.f,0.f);
    }

};

#endif // ENTITY_H
