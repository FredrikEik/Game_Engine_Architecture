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
    QVector3D g = QVector3D(0.0f, -9.8067f*3, 0.0f);
    gsl::Vector3D pos = Transf->mMatrix.getPosition();
    FindTriangle(Transf, radius); // finds normal and height of plane

    //check collision with floor
    //add mirror vector to velocity
    //apply velocity
    //add acceleration

    //add velocity component V = v0 + at
    Transf->Velocity = gsl::Vector3D(Transf->Velocity.getX(), Transf->Velocity.getY() +( g.y() * deltaTime ), Transf->Velocity.getZ());
    //Transf->mMatrix.translate(Transf->Velocity.getX()*deltaTime, Transf->Velocity.getY()*deltaTime + 0.5f*g.y()*deltaTime*deltaTime ,Transf->Velocity.getZ()*deltaTime);


    float length = QVector3D().dotProduct(MakeQvec3D(Data.heightOfFloor) - pos.getQVector(),MakeQvec3D( Data.floorNormal));
    length =  std::abs(length);

    if(length  < radius  && onTriangle)//if it is colliding with floor
    {
        if(once)
        {
            float elasticity = 1.f;

            //turn vec towards normal
            QVector3D NewVector =  MirrorVector(MakeQvec3D( Transf->Velocity), MakeQvec3D( Data.floorNormal));
            float speed = Transf->Velocity.length() * elasticity;

            Transf->Velocity =  MakeGSLvec3D( NewVector)*speed;
            once = false;
        }

    }
    if(!once)
        once=true;

    // add strekning s = v0*t + 1/2 * a * t^2
    QVector3D distance = MakeQvec3D( Transf->mMatrix.getPosition()) + DistanceTraveled(MakeQvec3D(Transf->Velocity),g,deltaTime);
    Transf->mMatrix.setPosition(distance.x(),distance.y(), distance.z());




}

void PhysicsSystem::bounce_floor(float deltaTime, TransformComponent *Transf,float radius)
{

    //length between ball and floor
    float distance = Transf->mMatrix.getPosition().getY();
    //qDebug()<<"///////DISTANCE TO FLOOR: "<<distance;
    float elasticity = 0.9f;
    if(distance < radius){
        //add bouncing force
        if(Transf->Velocity.getY() < 0)
        {
            //powerloss from bounce
            Transf->Velocity = gsl::Vector3D(Transf->Velocity.getX()*elasticity, Transf->Velocity.getY()*elasticity,Transf->Velocity.getZ()*elasticity);
            //turn vec up
            Transf->Velocity.setY( -Transf->Velocity.getY() );
            //set position above floor
            Transf->mMatrix.setPosition(Transf->mMatrix.getPosition().getX(),radius,Transf->mMatrix.getPosition().getZ());
            if(once)
            {
                //here we add a single direction once to make it go brrr
                Transf->Velocity = Transf->Velocity + gsl::Vector3D(0.0f, 0.0f,0.0f);
                once = false;
            }
        }
    }

}

void PhysicsSystem::FindTriangle(TransformComponent *Transf, float collisionRadius)
{
    //we need to setup the baryc function that returns baryc uvw numbers in vector3d    check
    //use these numbers in a if check to see if it is indeed on the same triangle       check
    // if barycx barycy and bvaryz is positive, we on the correct plane                 check
    //when correct plane is found, get normal vec from plane                            check
    //use normal to calculate the mirror vector fig. 8.8
    //add mirror vec to the main velocity of ball,
    //then start on rotation, after we got movement on a plane :D

    QVector3D p1,p2,p3,posBall, Baryc;
    posBall.setX( Transf->mMatrix.getPosition().getX());
    posBall.setY( Transf->mMatrix.getPosition().getY());
    posBall.setZ( Transf->mMatrix.getPosition().getZ());
    //every third triangle is our triangle.
    for(int i = 0; i < mSurfaceData->mVertices.size() ; i = i + 3) // sum mad sketch movement in array
    {
        //the vertex positions are flipped because of us using blender for the surface.
        p1 = MakeQvec3D(mSurfaceData->mVertices[i + 0].getVertex());//1
        p3 = MakeQvec3D(mSurfaceData->mVertices[i + 1].getVertex());//3
        p2 = MakeQvec3D(mSurfaceData->mVertices[i + 2].getVertex());//2
        //had to make a translator between the different vec types, pls dont hate me, its a lill workaround as i dont want to change all vectors.

        Baryc = Barysentric( p1 , p2 , p3 , posBall );
        //qDebug() << "BARYC: "<<Baryc;
        if(Baryc.x() >=0 && Baryc.y() >= 0 && Baryc.z() >= 0)
        {
            //use normals from plane here to displace ball in normal direction
            Data.floorNormal = MakeGSLvec3D( CalcPlaneNormal(p1,p2,p3));
            float height = p1.y()*Baryc.x() + p2.y()*Baryc.y() + p3.y()*Baryc.z();
            //  qDebug() << "/////////////// FLOOR NORMAL: "<<Data.floorNormal;
            //  qDebug() << "BARYC HEIGHT: "<<height;
            Data.heightOfFloor = gsl::Vector3D(posBall.x(), height, posBall.z());
            //Transf->mMatrix.setPosition(Transf->mMatrix.getPosition().getX(), height + collisionRadius, Transf->mMatrix.getPosition().getZ());
            onTriangle = true;
            break;
        }
        if(onTriangle)
            onTriangle = false;
    }



}

QVector3D PhysicsSystem::CalcPlaneNormal(QVector3D p1,QVector3D p2,QVector3D p3)
{

    QVector3D normal;
    normal = normal.crossProduct( (p3-p1) , (p2-p1) );
    normal.normalize();
    return normal;
}

QVector3D PhysicsSystem::Barysentric(QVector3D p1,QVector3D p2,QVector3D p3, QVector3D pos)
{


    p1.setY(0);
    p2.setY(0);
    p3.setY(0);
    pos.setY(0);

    //Tatt fra matte3 barysentriske koden.
    QVector3D p12 = p2-p1;
    QVector3D p13 = p3-p1;

    QVector3D n = QVector3D::crossProduct(p12,p13);
    float areal_123 = n.length(); //Dobbelt areal bby :D
    //qDebug() <<"AREA: " <<areal_123;
    QVector3D baryc; //Til return verdig, we fill this up with u, v, w
    //u
    QVector3D p = p2 - QVector3D(pos.x(),0.0f,pos.z());
    QVector3D q = p3 - QVector3D(pos.x(),0.0f,pos.z());
    n = QVector3D::crossProduct(p,q);
    baryc.setX(n.y()/areal_123);
    //qDebug() <<"AREA: " <<baryc.x();
    //v
    p = p3 - QVector3D(pos.x(),0.0f,pos.z());
    q = p1 - QVector3D(pos.x(),0.0f,pos.z());
    n = QVector3D::crossProduct(p,q);
    baryc.setY(n.y()/areal_123);

    //w
    p = p1 - QVector3D(pos.x(),0.0f,pos.z());
    q = p2 - QVector3D(pos.x(),0.0f,pos.z());
    n = QVector3D::crossProduct(p,q);
    baryc.setZ(n.y()/areal_123);

    //qDebug() <<"HEIGHT OF VERTEX " <<*heightmap->GetSurfacePos((int)px,(int)pz - 1);
    //qDebug() <<"BARISENTRIC COORDINATES: " <<baryc;

    //i have no fucking clue why it needs to be negative. but hey it works ;)
    return (-1*baryc);
}

gsl::Vector3D PhysicsSystem::MakeGSLvec3D(QVector3D vec)
{
    gsl::Vector3D temp;
    temp.setX(vec.x());
    temp.setY(vec.y());
    temp.setZ(vec.z());
    return temp;
}

QVector3D PhysicsSystem::MakeQvec3D(gsl::Vector3D vec)
{
    QVector3D temp;
    temp.setX(vec.getX());
    temp.setY(vec.getY());
    temp.setZ(vec.getZ());
    return temp;
}

QVector3D PhysicsSystem::MirrorVector(QVector3D Vector, QVector3D normal)
{
    QVector3D MVector;
    QVector3D dotprod;
    qDebug()<<"-__--_-___MIroror VECTOR, NORMAL "<<Vector<<normal;
    float dot = dotprod.dotProduct(Vector,normal);
    MVector = Vector -2.0f*(dot)*normal;
    MVector.normalize();
    return MVector ;
}

QVector3D PhysicsSystem::DistanceTraveled(QVector3D Velocity, QVector3D Acceleration, float DT)
{
    QVector3D Finaldistance;

    //s = v0*dt + 0.5*g*t^2
    Finaldistance = QVector3D( Velocity.x()*DT + 0.5f*Acceleration.x()*DT*DT
                               ,Velocity.y()*DT + 0.5f*Acceleration.y()*DT*DT
                               ,Velocity.z()*DT + 0.5f*Acceleration.z()*DT*DT);
    return Finaldistance;
}
