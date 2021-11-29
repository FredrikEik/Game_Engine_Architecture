﻿#include "physicsballsystem.h"
#include "queue"
#include <stdlib.h>

//PhysicsBallSystem* PhysicsBallSystem::mInstance = nullptr;

PhysicsBallSystem::PhysicsBallSystem()
{

}

void PhysicsBallSystem::update(GameObject& ballInn)
{

    gsl::Vector3D conv = ballInn.mTransformComp->mMatrix.getPosition();
    QVector2D xzBall {conv.x,conv.z};

//    ballInn.mTransformComp->mMatrix.getPosition();
    gsl::Vector3D currentpos = ballInn.mTransformComp->mMatrix.getPosition();

    std::pair<QVector3D,QVector3D> aVectorAndCollVector = getAkselerationVector(xzBall);
    QVector3D aVector = aVectorAndCollVector.first;
    collisionVector = aVectorAndCollVector.second;
    collisionVector.normalize();

    float dt = 0.0007f;
    mPosition = QVector3D(ballInn.mTransformComp->mMatrix.getPosition().x,ballInn.mTransformComp->mMatrix.getPosition().y,ballInn.mTransformComp->mMatrix.getPosition().z );
    mVelocity = mVelocity + aVector * dt;

    if(onNewTriangle)
    {
        qDebug() << "LastNormal: " << normalLastAndCurrentQ.front() << "currentNormal: " << normalLastAndCurrentQ.back();
        QVector3D m = normalLastAndCurrentQ.front();
        QVector3D n = normalLastAndCurrentQ.back();

        collisionNormal = (m + n)/ (m.length()+n.length());

        collisionNormal.normalize();


//        if(collisionNormal == normalLastAndCurrentQ.back())
//        {
//            collisionNormal = QVector3D{0,0,0};
//        }
        qDebug() << "NewTriangle";
        QVector3D nextVelocity = mVelocity - (2 * (mVelocity * collisionVector) * collisionVector);
        mVelocity = nextVelocity;
    }

    mPosition = mPosition + mVelocity * dt;
    ballInn.mTransformComp->mMatrix.setPosition(mPosition.x(), mPosition.y(), mPosition.z() );


//DEBUGONLY
    //qDebug() << "Height: " << ballInn.mTransformComp->mMatrix.getPosition().y;
//getHeightbyBarycentricCoordinates(QVector2D{ballInn.mTransformComp->mMatrix.getPosition().x,ballInn.mTransformComp->mMatrix.getPosition().z});

    ballInn.mTransformComp->mMatrix.translate(mVelocity.x(),mVelocity.y(),mVelocity.z());
    ballInn.mTransformComp->mMatrix.setPosition(ballInn.mTransformComp->mMatrix.getPosition().x,
                                                getHeightbyBarycentricCoordinates(xzBall),
                                                ballInn.mTransformComp->mMatrix.getPosition().z );
}

void PhysicsBallSystem::SetTerrainData(GameObject &TerrainInn)
{
    mTerrain = &TerrainInn;

    // --> TODO: Make a loop to open file and get all the heights and store them in an array
    // this way i can use them in the getZbyBarycentricCoordinates

    //qDebug() << "TarrainPos X: " << mTerrain->mTransformComp->mMatrix.getPosition().x << "Y: " << mTerrain->mTransformComp->mMatrix.getPosition().y;
}

void PhysicsBallSystem::move(float dt)
{

}



//PhysicsBallSystem * PhysicsBallSystem::getInstance()
//{
//    if(!mInstance)
//    {
//        mInstance = new PhysicsBallSystem();
//    }
//    return mInstance;
//}

QVector3D PhysicsBallSystem::barycentricCoordinates(const QVector2D &dette, const QVector2D &p1, const QVector2D &p2, const QVector2D &p3)
{
    QVector2D p12a = p2-p1;
    QVector2D p13a = p3-p1;
    QVector3D p12{p12a.x(),p12a.y(),0};
    QVector3D p13{p13a.x(),p13a.y(),0};
    QVector3D n = QVector3D::crossProduct(p12, p13);
    float areal_123 = n.length(); // dobbelt areal
    QVector3D baryc; // til retur. Husk

    // u
    QVector2D p = p2 - dette;
    QVector2D q = p3 - dette;
    QVector3D pa{p.x(), p.y(), 0};
    QVector3D qa{q.x(), q.y(), 0};
    n = QVector3D::crossProduct(pa,qa);
    baryc.setX(n.z()/areal_123);

    // v
    p = p3 - dette;
    q = p1 - dette;
    pa = {p.x(), p.y(), 0};
    qa = {q.x(), q.y(), 0};
    n = QVector3D::crossProduct(pa,qa);
    baryc.setY(n.z()/areal_123);

    // w
    p = p1 - dette;
    q = p2 - dette;
    pa = {p.x(), p.y(), 0};
    qa = {q.x(), q.y(), 0};
    n = QVector3D::crossProduct(pa,qa);
    baryc.setZ(n.z()/areal_123);

    return baryc;
}


float PhysicsBallSystem::getHeightbyBarycentricCoordinates(const QVector2D &ballPos)
{
    //qDebug() << "Barri: " << barycentricCoordinates(QVector2D{1,1},QVector2D{1,0},QVector2D{1,1},QVector2D{0,1});
    //world koordinater, men i forhold til height mapet der 0,0 er nederst til venstre (sør-vest).

//    qDebug() << "ballPos X: " << ballPos.x() << "Y: " << ballPos.y();
    xPosOnTarrain = (ballPos.x() - mTerrain->mTransformComp->mMatrix.getPosition().x) ;
    zPosOnTarrain = (ballPos.y() - mTerrain->mTransformComp->mMatrix.getPosition().z);

    // xyScale og Gridsize er Hardkodet til å være 2 nå
    gridSquareSize = xzScale;
    //hvilken grid spilleren er på
    gridXPos = int(floor(xPosOnTarrain / xzScale));
    gridZPos = int(floor(zPosOnTarrain / xzScale));

    if(gridXPos >= 0 && gridZPos >= 0 && gridXPos < mCols && gridZPos < mRows)
    {   //Koordinat grid
//        qDebug() << "gridPos x: " << gridXPos << "z:" << gridZPos;

        float xCoordInSquare = fmod(xPosOnTarrain,gridSquareSize)/gridSquareSize;
        float zCoordInSquare = fmod(zPosOnTarrain,gridSquareSize)/gridSquareSize;

        //qDebug () << "xz in Square x: " << xCoordInSquare << "z: " << zCoordInSquare;
        QVector3D uvw;
        float answer;
        QVector2D p1{0,0};
        QVector2D p2{1,0};
        QVector2D p3{0,1};
        QVector2D p4{1,1};

        //TODO: make p1 p2 etc. in the right order. get the right y height from each vertex.

//        float p1y = -0.000000f;
//        float p2y = 0.203227f;
//        float p3y = 0.203227f;
//        float p4y = -0.000000f;

//       float p1y = sArrayHeights[(gridYPos)*rows + (gridXPos)];
//        float p2y = sArrayHeights[(gridYPos+1)*rows + (gridXPos)];
//        float p3y = sArrayHeights[(gridYPos)*rows + (gridXPos+1)];
//        float p4y = sArrayHeights[(gridYPos+1)*rows + (gridXPos+1)];

        float p1y = mTerrain->mMeshComp->mVertices[0].at(gridZPos*mCols + (100-gridXPos)).mXYZ.getY();
        float p2y = mTerrain->mMeshComp->mVertices[0].at(gridZPos*mCols + (100-gridXPos-1)).mXYZ.getY();
        float p3y = mTerrain->mMeshComp->mVertices[0].at((gridZPos+1)*mCols + (100-gridXPos)).mXYZ.getY();
        float p4y = mTerrain->mMeshComp->mVertices[0].at((gridZPos+1)*mCols + (100-gridXPos-1)).mXYZ.getY();


        //Finne hvilken av de to trekantene spilleren står på
        if(xCoordInSquare <= (1-zCoordInSquare))
        {
            uvw = barycentricCoordinates(QVector2D{float(xCoordInSquare),float(zCoordInSquare)},p1,p2,p3);
            qDebug() << "uvw: " << uvw;
            answer = p1y*uvw.x()+p2y*uvw.y()+p3y*uvw.z();
            return answer;
        }
        if(xCoordInSquare > (1-zCoordInSquare))
        {   QVector2D posInsSquare{xCoordInSquare,zCoordInSquare};
            uvw = barycentricCoordinates(posInsSquare,p2,p4,p3);
            qDebug() << "uvw: " << uvw;
            answer = p2y*uvw.x()+p4y*uvw.y()+p3y*uvw.z();
            return answer;
        }
    }else{
        qDebug() << "Ball is out of bounds!";
    }
    return 0.f;








//    //qDebug() << "Barri: " << barycentricCoordinates(QVector2D{1,1},QVector2D{1,0},QVector2D{1,1},QVector2D{0,1});
//    //world koordinater, men i forhold til height mapet der 0,0 er nederst til venstre (sør-vest).

//    mCols = 100;
//    mRows = 146;
////    qDebug() << "ballPos X: " << ballPos.x() << "Y: " << ballPos.y();
//    xPosOnTarrain = (ballPos.x() - mTerrain->mTransformComp->mMatrix.getPosition().x) ;
//    zPosOnTarrain = (ballPos.y() - mTerrain->mTransformComp->mMatrix.getPosition().z);

//    // xyScale og Gridsize er Hardkodet til å være 2 nå
//    gridSquareSize = xzScale;
//    //hvilken grid spilleren er på
//    gridXPos = int(floor(xPosOnTarrain / xzScale));
//    gridZPos = int(floor(zPosOnTarrain / xzScale));

//    if(gridXPos >= 0 && gridZPos >= 0 && gridXPos < mCols && gridZPos < mRows)
//    {   //Koordinat grid
////        qDebug() << "gridPos x: " << gridXPos << "z:" << gridZPos;

//        float xCoordInSquare = fmod(xPosOnTarrain,gridSquareSize)/gridSquareSize;
//        float zCoordInSquare = fmod(zPosOnTarrain,gridSquareSize)/gridSquareSize;

//        //qDebug () << "xz in Square x: " << xCoordInSquare << "z: " << zCoordInSquare;
//        QVector3D uvw;
//        float answer;
//        QVector2D p1{0,0};
//        QVector2D p2{1,0};
//        QVector2D p3{0,1};
//        QVector2D p4{1,1};

////        float p1y = 15.000000f;
////        float p2y = 17.203227f;
////        float p3y = 17.203227f;
////        float p4y = 15.000000f;

//        //TODO: make p1 p2 etc. in the right order. get the right y height from each vertex.

//        float p1y  = mTerrain->mMeshComp->mVertices[0].at(gridZPos*mCols + (100-gridXPos-1)).mXYZ.getY();
//        float p2y  = mTerrain->mMeshComp->mVertices[0].at(gridZPos*mCols + (100-gridXPos)).mXYZ.getY();
//        float p3y  = mTerrain->mMeshComp->mVertices[0].at((gridZPos+1)*mCols + (100-gridXPos-1)).mXYZ.getY();
//        float p4y  = mTerrain->mMeshComp->mVertices[0].at((gridZPos+1)*mCols + (100-gridXPos)).mXYZ.getY();
//        qDebug() << "p1y" << p1y << "p2y" << p2y << "p3y" << p3y << "p4y" << p4y;

//        //Finne hvilken av de to trekantene spilleren står på
//        if(xCoordInSquare <= (1-zCoordInSquare))
//        {
//            uvw = barycentricCoordinates(QVector2D{float(xCoordInSquare),float(zCoordInSquare)},p1,p2,p3);
////            uvw = {std::abs(uvw.x()),std::abs(uvw.y()),std::abs(uvw.z())};
//            answer = p1y*uvw.x()+p2y*uvw.y()+p3y*uvw.z();
//            qDebug() << "uvw: " << uvw;
//            return answer;
//        }
//        if(xCoordInSquare > (1-zCoordInSquare))
//        {   QVector2D posInsSquare{xCoordInSquare,zCoordInSquare};
//            uvw = barycentricCoordinates(posInsSquare,p2,p4,p3);
////            uvw = {std::abs(uvw.x()),std::abs(uvw.y()),std::abs(uvw.z())};
//            answer = p2y*uvw.x()+p4y*uvw.y()+p3y*uvw.z();
//            qDebug() << "uvw: " << uvw;
//            return answer;
//        }
//    }else{
//        qDebug() << "Ball is out of bounds!";
//    }
//    return 0.f;
}

std::pair<QVector3D,QVector3D> PhysicsBallSystem::getAkselerationVector(const QVector2D &ballPos)
{        
    //qDebug() << "Barri: " << barycentricCoordinates(QVector2D{1,1},QVector2D{1,0},QVector2D{1,1},QVector2D{0,1});
    //world koordinater, men i forhold til height mapet der 0,0 er nederst til venstre (sør-vest).

    mCols = 100;
    mRows = 146;

    xPosOnTarrain = (ballPos.x() - mTerrain->mTransformComp->mMatrix.getPosition().x);
    zPosOnTarrain = (ballPos.y() - mTerrain->mTransformComp->mMatrix.getPosition().z);

//    qDebug () << "xPosOnTerrain: " << xPosOnTarrain << "zPosOnTerrain: " << zPosOnTarrain;


    // xyScale og Gridsize er Hardkodet til å være 2 nå
    gridSquareSize = xzScale;
    //hvilken grid spilleren er på
    gridXPos = int(floor(xPosOnTarrain / xzScale));
    gridZPos = int(floor(zPosOnTarrain / xzScale));

    float xCoordInSquare = fmod(xPosOnTarrain,gridSquareSize)/gridSquareSize;
    float zCoordInSquare = fmod(zPosOnTarrain,gridSquareSize)/gridSquareSize;

//    qDebug () << "xCoordInSquare: " << xCoordInSquare << "zCoorInSquare: " << zCoordInSquare;
//    qDebug () << "gridXPos: " << gridXPos << "gridZPos: " << gridZPos;


    if(gridXPos >= 0 && gridZPos >= 0 && gridXPos < mCols && gridZPos < mRows)
    {   //Koordinat grids

        QVector2D p1_2D{10,0};
        QVector2D p2_2D{0,0};
        QVector2D p3_2D{10,10};
        QVector2D p4_2D{0,10};


        float p1y = mTerrain->mMeshComp->mVertices[0].at(gridZPos*mCols + (100-gridXPos-1)).mXYZ.getY();
        float p2y = mTerrain->mMeshComp->mVertices[0].at(gridZPos*mCols + (100-gridXPos)).mXYZ.getY();
        float p3y = mTerrain->mMeshComp->mVertices[0].at((gridZPos+1)*mCols + (100-gridXPos-1)).mXYZ.getY();
        float p4y = mTerrain->mMeshComp->mVertices[0].at((gridZPos+1)*mCols + (100-gridXPos)).mXYZ.getY();



        QVector3D p1{p1_2D.x(),p1y,p1_2D.y()};
        QVector3D p2{p2_2D.x(),p2y,p2_2D.y()};
        QVector3D p3{p3_2D.x(),p3y,p3_2D.y()};
        QVector3D p4{p4_2D.x(),p4y,p4_2D.y()};

//        qDebug() << "p1: " << p1 << "p2: " << p2 << "p3: " << p3 << "p4: " << p4;

        //Finne hvilken av de to trekantene spilleren står på
        if(xCoordInSquare <= (zCoordInSquare))
        {
            qDebug () << "UPPER";
            // p1, p3, p4
            QVector3D v0 = p2 - p4;
            QVector3D v1 = p3 - p4;

            normal = QVector3D::crossProduct(v1,v0);
            normal.normalize();
            qDebug() << "Actual normal" << normal;


            // make a quick true false thing when it quickly is ture then pring in the other fucntion
            if(normalSetup)
            {
                normalLastAndCurrentQ.push(normal);
                normalLastAndCurrentQ.push(normal);
                normalSetup = false;
            }
            if(normalLastAndCurrentQ.back() != normal)
            {
                normalLastAndCurrentQ.pop();
                normalLastAndCurrentQ.push(normal);
                onNewTriangle = true;
                id++;
            }else
            {
                onNewTriangle = false;
            }

            QVector3D Vec{9.81f*normal.x()*normal.y(),9.81f*((normal.y()*normal.y())-1),9.81f*normal.y()*normal.z()};

            return std::pair<QVector3D,QVector3D>{Vec,collisionNormal};
        }
        if(xCoordInSquare > (zCoordInSquare))
        {
            qDebug () << "LOWER";
            // p1, p2, p4
            QVector3D v0 = p2 - p1;
            QVector3D v1 = p3 - p1;

            normal = QVector3D::crossProduct(v0,v1);
            normal.normalize();
            qDebug() << "Actual normal" << normal;

            //qDebug() << normal;
            if(normalSetup)
            {
                normalLastAndCurrentQ.push(normal);
                normalLastAndCurrentQ.push(normal);
                normalSetup = false;
            }
            if(normalLastAndCurrentQ.back() != normal)
            {
                id++;
                normalLastAndCurrentQ.pop();
                normalLastAndCurrentQ.push(normal);
                onNewTriangle = true;
            }else
            {
                onNewTriangle = false;
            }

            QVector3D aVec{9.81f*normal.x()*normal.y(),9.81f*((normal.y()*normal.y())-1),9.81f*normal.y()*normal.z()};

            return std::pair<QVector3D,QVector3D>{aVec,collisionNormal};
        }
    }else{
        qDebug() << "Ball is out of bounds!";
    }
    //    return std::pair<QVector3D,QVector3D>{QVector3D{0,0,0},QVector3D{0,0,0}};
}

void PhysicsBallSystem::resetBall()
{
//    mVelocity = {0,0,0};
//    mPosition = {0,0,0};
//    collisionVector = {0,0,0};
//    lastNormal  = {0,0,0};
//    currentNormal  = {0,0,0};
//    normalLastAndCurrentQ.pop();
//    normalLastAndCurrentQ.pop();
//    onNewTriangle = false;
//    id = 0;
}

//GameObject *PhysicsBallSystem::GetTerrain()
//{
//    return terrain;
//}
