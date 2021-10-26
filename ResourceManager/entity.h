#ifndef ENTITY_H
#define ENTITY_H

#include <vector>
#include <string>
#include "components.h"
#include "vertex.h"

/**
    @brief Entity Class.
 */

class Entity
{
public:
    Entity()
    {

    };

    void move()
    {
        mTransform.position += mVelocity;
    }

//    MAX_ENTITIES_TYPE mEntityID{MAX_ENTITIES};
    std::string mName{"NoName"};
    gsl::Vector3D mVelocity{0.f, 0.f, 0.f};
    gsl::Vector3D mInitPos{0.f, 0.f, 0.f};



    float mCurrentHP;

//    //Not final version - this is just to make the components work for now!
//    MAX_ENTITIES_TYPE mMaterialIDIndex;

    MeshComponent mMesh;
    TransformComponent mTransform;
//    MaterialID mMaterialID;
//    Collider mCollider;
//    Sound mSound;
//    Script mScript;
//    Light mLight;

};

#endif // ENTITY_H
