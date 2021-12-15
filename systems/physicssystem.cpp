#include "physicssystem.h"

PhysicsSystem::PhysicsSystem()
{

}

void PhysicsSystem::InitPhysicsSystem(MeshComponent *surfaceData, std::vector<Vertex> inVertexData)
{

    //!
    //!
    //! Saves the parameter for use later
    //!
    //!
        mSurfaceData = surfaceData;
        mSurfaceData->collisionRadius = 0.0f;

    vertexData = inVertexData;

}

void PhysicsSystem::move(float deltaTime, TransformComponent *Transf, float radius)
{
    //!
    //! Main physics movement function
    //! @param my - is our internal friction coefficient
    //! We first use FindTriangel(Transf); to collect the necessary data for our floor data
    //! Then we check if our object is in range of collision with the floor
    //! if collion is detected - then we run physics functions
    //! @param MirrorVector - then we change direction of velocity
    //! @param Friction - just a rudamentary slow down calculation. NOT A REAL FRICTION
    //! Finally we ofsett the clipping of the object to go above the floor
    //! -Physics simulated
    //!

    //friction
    float my = 0.03f; //friction coefficient


    QVector3D g = QVector3D(0.0f, -9.8067f*3.f, 0.0f);
    gsl::Vector3D pos = Transf->mMatrix.getPosition();

    if((pos.getX() >0.0f & pos.getX() < 600) && (pos.getZ() >0.0f & pos.getZ() < 700) && pos.getY() >0.0f) FindTriangle(Transf); // finds normal and height of plane

    //add velocity component V = v0 + at
    Transf->Velocity = gsl::Vector3D(Transf->Velocity.getX(), Transf->Velocity.getY() +( g.y() * deltaTime ), Transf->Velocity.getZ());

    float length = QVector3D().dotProduct(MakeQvec3D(Data.heightOfFloor) - pos.getQVector(),MakeQvec3D( Data.floorNormal));
    length =  std::abs(length);

    QVector3D ClippingVec = QVector3D(0.0f,0.0f,0.0f);
    if(length  < radius  && onTriangle)//if it is colliding with floor
    {
        if(once)
        {

            //Mirror vec
            QVector3D NewVector =  MirrorVector(MakeQvec3D( Transf->Velocity), MakeQvec3D( Data.floorNormal));
            NewVector.setY(NewVector.y()*(1.0f-my));
            //get speed
            float speed = Transf->Velocity.length();// * elasticity;

            //apply speed to new vector direction
            Transf->Velocity =  MakeGSLvec3D( NewVector)*speed ;
            once = false;

            //friction calc
            QVector3D Friction = -MakeQvec3D(Transf->Velocity);
            Friction.normalize();

            Friction = my*speed*Friction;
            Transf->Velocity = Transf->Velocity + MakeGSLvec3D( Friction );

            //clipping collision
            ClippingVec =deltaTime*(radius-length)*MakeQvec3D( Data.floorNormal);
            Transf->mMatrix.translate(MakeGSLvec3D( ClippingVec));

            Transf->mMatrix.setPosition(Transf->mMatrix.getPosition().getX(),Data.heightOfFloor.getY() + radius,Transf->mMatrix.getPosition().getZ());
        }

    }
    if(!once)
        once=true;


    // add strekning s = v0*t + 1/2 * a * t^2
    QVector3D distance = MakeQvec3D( Transf->mMatrix.getPosition()) + DistanceTraveled(MakeQvec3D(Transf->Velocity),g,deltaTime)*1.5f;
    Transf->mMatrix.setPosition(distance.x(),distance.y(), distance.z());

}



void PhysicsSystem::FindTriangle(TransformComponent *Transf)
{

    //! @details Finds correct triangle the object is on
    QVector3D p1,p2,p3,posBall, Baryc;
    posBall.setX( Transf->mMatrix.getPosition().getX());
    posBall.setY( Transf->mMatrix.getPosition().getY());
    posBall.setZ( Transf->mMatrix.getPosition().getZ());
    bool isFound = false; //optimization

    //every third triangle is our triangle.
    int index = Transf->LastTriangeindex ;
    //the vertex positions are flipped because of us using blender for the surface.

    //! We have optimisation code that saves the last position of the triangle, then checks if we are still on the same triangle. Removes nesessity to run code for all vertex positions
    //! @param isFound is true when we are on the correct surface
if(index =! -1){
    p1 = MakeQvec3D(vertexData[index + 0].getVertex());//1
    p2 = MakeQvec3D(vertexData[index + 2].getVertex());//3
    p3 = MakeQvec3D(vertexData[index + 1].getVertex());//2
    //had to make a translator between the different vec types, pls dont hate me, its a lill workaround as i dont want to change all vectors.

    Baryc = Barysentric( p1 , p2 , p3 , posBall );
    if(Baryc.x() >=0 && Baryc.y() >= 0 && Baryc.z() >= 0)
    {
        //use normals from plane here to displace ball in normal direction
        Data.floorNormal = MakeGSLvec3D( CalcPlaneNormal(p1,p2,p3));
        float height = p1.y()*Baryc.x() + p2.y()*Baryc.y() + p3.y()*Baryc.z();
        Data.heightOfFloor = gsl::Vector3D(posBall.x(), height, posBall.z());

        isFound = true;
    }
}
    if(!isFound )
        for(unsigned long long i = 0; i < vertexData.size() ; i = i + 3 ) // sum mad sketch movement in array
        {
            //the vertex positions are flipped because of us using blender for the surface.
            p1 = MakeQvec3D(vertexData[i + 0].getVertex());//1
            p2 = MakeQvec3D(vertexData[i + 2].getVertex());//3
            p3 = MakeQvec3D(vertexData[i + 1].getVertex());//2

            Baryc = Barysentric( p1 , p2 , p3 , posBall );

            if(Baryc.x() >=0 && Baryc.y() >= 0 && Baryc.z() >= 0)
            {
                //use normals from plane here to displace ball in normal direction
                Data.floorNormal = MakeGSLvec3D( CalcPlaneNormal(p1,p2,p3));
                float height = p1.y()*Baryc.x() + p2.y()*Baryc.y() + p3.y()*Baryc.z();

                Data.heightOfFloor = gsl::Vector3D(posBall.x(), height, posBall.z());

                onTriangle = true;
                Transf->LastTriangeindex = static_cast<int>(i);
                break;
            }
            if(onTriangle)
                onTriangle = false;
        }

}


QVector3D PhysicsSystem::CalcPlaneNormal(QVector3D p1,QVector3D p2,QVector3D p3)
{

    //! Calculates normal of the plane
    //! @param p1 - first vertex
    //! @param p2 - second vectex
    //! @param p3 - third vertex
    //! @return Normal - normalised
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

    return (-baryc);
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
    float dot = dotprod.dotProduct(Vector,normal);
    MVector = Vector -2.0f*(dot)*normal;
    MVector.normalize();
    return MVector ;
}

QVector3D PhysicsSystem::DistanceTraveled(QVector3D Velocity, QVector3D Acceleration, float DT)
{
    QVector3D Finaldistance;

    //s = v0*dt + 0.5*g*dt^2
    Finaldistance = QVector3D( Velocity.x()*DT + 0.5f*Acceleration.x()*DT*DT
                               ,Velocity.y()*DT + 0.5f*Acceleration.y()*DT*DT
                               ,Velocity.z()*DT + 0.5f*Acceleration.z()*DT*DT);
    return Finaldistance;
}