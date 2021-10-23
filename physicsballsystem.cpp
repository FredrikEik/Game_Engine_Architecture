#include "physicsballsystem.h"
#include "queue"

//PhysicsBallSystem* PhysicsBallSystem::mInstance = nullptr;

PhysicsBallSystem::PhysicsBallSystem()
{

}

void PhysicsBallSystem::update(GameObject& ballInn)
{
    gsl::Vector3D conv = ballInn.mTransformComp->mMatrix.getPosition();
    QVector2D xzBall {conv.x,conv.z};

    ballInn.mTransformComp->mMatrix.getPosition();
    //qDebug() << "y: " << getZbyBarycentricCoordinates(xzBall);
    gsl::Vector3D currentpos = ballInn.mTransformComp->mMatrix.getPosition();
    //ballInn.mTransformComp->mMatrix.setPosition(currentpos.x,getZbyBarycentricCoordinates(xzBall)-0.99f, currentpos.z);
    QVector3D aVector = getAkselerationVector(xzBall);

    float dt = 0.017f;
    mPosition =QVector3D(ballInn.mTransformComp->mMatrix.getPosition().x,ballInn.mTransformComp->mMatrix.getPosition().y,ballInn.mTransformComp->mMatrix.getPosition().z );
    mVelocity = mVelocity + aVector * dt;
    mPosition = mPosition + mVelocity * dt;
    //qDebug() << mPosition;
    ballInn.mTransformComp->mMatrix.setPosition(mPosition.x(), mPosition.y(), mPosition.z() );


    //ballInn.mTransformComp->mMatrix.translate(getAkselerationVector(xzBall).x(),getAkselerationVector(xzBall).y(),getAkselerationVector(xzBall).z());
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

QVector3D PhysicsBallSystem::getAkselerationVector(const QVector2D &ballPos)
{        
    //qDebug() << "Barri: " << barycentricCoordinates(QVector2D{1,1},QVector2D{1,0},QVector2D{1,1},QVector2D{0,1});
    //world koordinater, men i forhold til height mapet der 0,0 er nederst til venstre (sør-vest).

//    qDebug() << "ballPos X: " << ballPos.x() << "Y: " << ballPos.y();
    xPosOnTarrain = (ballPos.x() - mTerrain->mTransformComp->mMatrix.getPosition().x);
    zPosOnTarrain = (ballPos.y() - mTerrain->mTransformComp->mMatrix.getPosition().z);

    //qDebug() << "xTerrain: " << xPosOnTarrain << "zTerrain: " << zPosOnTarrain;

    // xyScale og Gridsize er Hardkodet til å være 2 nå
    gridSquareSize = xzScale;
    //hvilken grid spilleren er på
    gridXPos = int(floor(xPosOnTarrain / xzScale));
    gridZPos = int(floor(zPosOnTarrain / xzScale));

    float xCoordInSquare = fmod(xPosOnTarrain,gridSquareSize)/gridSquareSize;
    float zCoordInSquare = fmod(zPosOnTarrain,gridSquareSize)/gridSquareSize;

//    qDebug() << "GridX: " <<gridXPos << "GridZ: " << gridZPos;
    if(setup)
    {
        lastAndCurrentQ.push(lastGridXZPos);
        lastAndCurrentQ.push(currentGridXZPos);
        setup = false;
    }

    currentGridXZPos = QVector2D(float(gridXPos),float(gridZPos));
    if(lastAndCurrentQ.back() != currentGridXZPos)
    {
        lastAndCurrentQ.push(currentGridXZPos);
        lastAndCurrentQ.pop();
    }

    if(gridXPos >= 0 && gridZPos >= 0 && gridXPos < mCols && gridZPos < mRows)
    {   //Koordinat grid
//        qDebug() << "gridPos x: " << gridXPos << "z:" << gridZPos;


        //qDebug () << "xz in Square x: " << xCoordInSquare << "z: " << zCoordInSquare;
        QVector2D p1_2D{0,0};
        QVector2D p2_2D{1,0};
        QVector2D p3_2D{0,1};
        QVector2D p4_2D{1,1};

        //TODO: make p1y p2y etc. in the right order. get the right y height from each vertex.

        float p1y = -0.000000f;
        float p2y = 0.203227f;
        float p3y = 0.203227f;
        float p4y = -0.000000f;

        QVector3D p1{p1_2D.x(),p1y,p1_2D.y()};
        QVector3D p2{p2_2D.x(),p2y,p2_2D.y()};
        QVector3D p3{p3_2D.x(),p3y,p3_2D.y()};
        QVector3D p4{p4_2D.x(),p4y,p4_2D.y()};

        //qDebug() << "p1: " << p1 << "p2: " << p2 << "p3: " << p3 << "p4: " << p4;

//       float p1y = sArrayHeights[(gridYPos)*rows + (gridXPos)];
//        float p2y = sArrayHeights[(gridYPos+1)*rows + (gridXPos)];
//        float p3y = sArrayHeights[(gridYPos)*rows + (gridXPos+1)];
//        float p4y = sArrayHeights[(gridYPos+1)*rows + (gridXPos+1)];

    //qDebug() <<"x: " <<  xCoordInSquare << "y: " << zCoordInSquare;
        //Finne hvilken av de to trekantene spilleren står på
        if(xCoordInSquare <= (zCoordInSquare))
        {
            // p1, p3, p4
            QVector3D normal;
            QVector3D v0 = p3 - p1;
            QVector3D v1 = p4 - p1;

            normal = QVector3D::crossProduct(v0,v1);
            normal.normalize();
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
            }
            QVector3D m = normalLastAndCurrentQ.front();
            QVector3D n = normalLastAndCurrentQ.back();
            QVector3D x{0,0,0};

            QVector3D mn = m+n;
            float mnLen = mn.length();
            float mnPow = mnLen*mnLen;
            // x = normalen på kolisjonsplanet
            x = {mn.x()/mnLen,mn.y()/mnLen,mn.z()/mnLen};
            qDebug() << "lastNormal" << normalLastAndCurrentQ.front() << "currentNormal" << normalLastAndCurrentQ.back();
            QVector3D aVec{normal.x()*normal.y()*9.81f,((normal.y()*normal.y())-1)*9.81f,normal.y()*normal.z()*9.81f};

            //aVec += x;

            return aVec;
        }
        if(xCoordInSquare > (zCoordInSquare))
        {
            QVector3D normal;

            // p1, p2, p4
            QVector3D v0 = p4 - p1;
            QVector3D v1 = p2 - p1;

            normal = QVector3D::crossProduct(v0,v1);
            normal.normalize();
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
            }

            QVector3D m = normalLastAndCurrentQ.front();
            QVector3D n = normalLastAndCurrentQ.back();
            QVector3D x{0,0,0};

            QVector3D mn = m+n;
            float mnLen = mn.length();
            float mnPow = mnLen*mnLen;
            // x = normalen på kolisjonsplanet
            x = {mn.x()/mnLen,mn.y()/mnLen,mn.z()/mnLen};


            qDebug() << "lastNormal" << normalLastAndCurrentQ.front() << "currentNormal" << normalLastAndCurrentQ.back();
            QVector3D aVec{normal.x()*normal.y()*9.81f,((normal.y()*normal.y())-1)*9.81f,normal.y()*normal.z()*9.81f};

            aVec += x;

            return aVec;
        }
    }else{
        qDebug() << "Ball is out of bounds!";
    }
    return QVector3D{0,0,0};
}
