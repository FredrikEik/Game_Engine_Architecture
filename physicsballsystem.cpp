#include "physicsballsystem.h"

//PhysicsBallSystem* PhysicsBallSystem::mInstance = nullptr;

PhysicsBallSystem::PhysicsBallSystem()
{

}

void PhysicsBallSystem::update(GameObject& ballInn)
{
    gsl::Vector3D conv = ballInn.mTransformComp->mMatrix.getPosition();
    QVector2D xzBall {conv.x,conv.z};

    qDebug() << "y: " << getZbyBarycentricCoordinates(xzBall);
    gsl::Vector3D currentpos = ballInn.mTransformComp->mMatrix.getPosition();
    ballInn.mTransformComp->mMatrix.setPosition(currentpos.x,getZbyBarycentricCoordinates(xzBall)-0.99f, currentpos.z);
}

void PhysicsBallSystem::SetTerrainData(GameObject &TerrainInn)
{
    mTerrain = &TerrainInn;

    // --> TODO: Make a loop to open file and get all the heights and store them in an array
    // this way i can use them in the getZbyBarycentricCoordinates

    //qDebug() << "TarrainPos X: " << mTerrain->mTransformComp->mMatrix.getPosition().x << "Y: " << mTerrain->mTransformComp->mMatrix.getPosition().y;
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

float PhysicsBallSystem::getZbyBarycentricCoordinates(const QVector2D &ballPos)
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

        float p1y = -0.000000f;
        float p2y = 0.203227f;
        float p3y = 0.203227f;
        float p4y = -0.000000f;

//       float p1y = sArrayHeights[(gridYPos)*rows + (gridXPos)];
//        float p2y = sArrayHeights[(gridYPos+1)*rows + (gridXPos)];
//        float p3y = sArrayHeights[(gridYPos)*rows + (gridXPos+1)];
//        float p4y = sArrayHeights[(gridYPos+1)*rows + (gridXPos+1)];


        //Finne hvilken av de to trekantene spilleren står på
        if(xCoordInSquare <= (1-zCoordInSquare))
        {
            uvw = barycentricCoordinates(QVector2D{float(xCoordInSquare),float(zCoordInSquare)},p1,p2,p3);
            answer = p1y*uvw.x()+p2y*uvw.y()+p3y*uvw.z();
            return answer;
        }
        if(xCoordInSquare > (1-zCoordInSquare))
        {   QVector2D posInsSquare{xCoordInSquare,zCoordInSquare};
            uvw = barycentricCoordinates(posInsSquare,p2,p4,p3);
            answer = p2y*uvw.x()+p4y*uvw.y()+p3y*uvw.z();
            return answer;
        }
    }else{
        qDebug() << "Ball is out of bounds!";
    }
    return 0.f;
}
