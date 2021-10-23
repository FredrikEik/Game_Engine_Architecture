#include "physicssystem.h"

PhysicsSystem::PhysicsSystem()
{

}

void PhysicsSystem::InitPhysicsSystem(MeshComponent *surfaceData)
{
    mSurfaceData = surfaceData;
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
    float elasticity = 0.7f;
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
                //here we add a single direction once to make it go brrr
                Transf->Velocity = Transf->Velocity + gsl::Vector3D(0.1f, 0.0f,0.0f);
                once = false;
            }
        }
    }

}

void PhysicsSystem::FindTriangle(TransformComponent *Transf)
{
    /*
    //every third triangle is our triangle.
    for(int i = 0; i < mSurfaceData->mVertices.size() ; i = i + 3) // sum mad sketch movement
    {
        Data.CurrentTriangle[0] = mSurfaceData->mVertices[i + 0].getVertex();//1
        Data.CurrentTriangle[2] = mSurfaceData->mVertices[i + 1].getVertex();//3
        Data.CurrentTriangle[1] = mSurfaceData->mVertices[i + 2].getVertex();//2
        Data.floorNormal = mSurfaceData->mVertices[i].getNormal();

        float distance = distanceToPlane(Transf);
    }
*/


}

float PhysicsSystem::distanceToPlane(TransformComponent *Transf)
{
    //https://mathworld.wolfram.com/Point-PlaneDistance.html
    float distance = 0.0f;
    /*
    float x,y,z; // position physics object
    float a,b,c; //position surface normalvector

    Transf->mMatrix.getPosition()
    distance =
*/
    return distance;
}
