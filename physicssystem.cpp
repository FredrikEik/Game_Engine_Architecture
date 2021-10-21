#include "physicssystem.h"

PhysicsSystem::PhysicsSystem()
{

}

void PhysicsSystem::freeFall(float deltaTime, TransformComponent *Transf, float radius)
{
    float g = -9.8067f;
    float heightfromfloor = Transf->mMatrix.getPosition().getY() + radius;
    if(heightfromfloor > radius )
    {
        Transf->Velocity.setY(Transf->Velocity.getY() +  g*deltaTime);
        Transf->mMatrix.translate(Transf->Velocity.getX(), Transf->Velocity.getY(),Transf->Velocity.getZ());

        //qDebug()<<"///////HEIGHT: "<<Transf->Velocity.getY();
    }


}

void PhysicsSystem::bounce_floor(float deltaTime, TransformComponent *Transf,float radius)
{

    //length between ball and floor
    float distance = Transf->mMatrix.getPosition().getY();
    //qDebug()<<"///////DISTANCE TO FLOOR: "<<distance;
    float elasticity = 0.1f;
    if(distance < radius){
        //add bouncing force
        if(Transf->Velocity.getY() < 0)
        {
            //powerloss from bounce
             Transf->Velocity = gsl::Vector3D(Transf->Velocity.getX(), Transf->Velocity.getY()*elasticity,Transf->Velocity.getZ());
           //turn vec up
            Transf->Velocity.setY( -Transf->Velocity.getY() );
            //set position above floor
            Transf->mMatrix.setPosition(Transf->mMatrix.getPosition().getX(),radius,Transf->mMatrix.getPosition().getZ());
            if(once)
            {
                Transf->Velocity = Transf->Velocity+gsl::Vector3D(0.0f, 0.0f,0.0f);
                once = false;
            }
        }
    }

}
