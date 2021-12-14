#include "collisionsystem.h"
#include "gsl/vector3d.h"

collisionSystem::collisionSystem()
{

}

bool collisionSystem::isColliding(MeshComponent *Mesh1,TransformComponent* transform1, MeshComponent* Mesh2, TransformComponent* transform2)
{
    if(Mesh1->IsCollidable && Mesh2->IsCollidable)
    {
        //length between 2 objects from center to center
        gsl::Vector3D lengthVec = transform1->mMatrix.getPosition() - transform2->mMatrix.getPosition();
        float distance = lengthVec.length();

        float radiusOfBothObjects = Mesh1->collisionRadius + Mesh2->collisionRadius;

        if(distance > radiusOfBothObjects){
            //not colliding
            return false;
        }
        else {
            //colliding if distance is <= radiusOfBothObjects
            return true;
        }

    }
    else
    {
        return false;
    }

}

