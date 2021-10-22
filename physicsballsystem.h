#ifndef PHYSICSBALLSYSTEM_H
#define PHYSICSBALLSYSTEM_H

#include "QVector3D"
#include "gameobject.h"
class PhysicsBallSystem
{
public:
    PhysicsBallSystem();

    void update(GameObject& ballInn);
    void SetTerrainData(GameObject& TerrainInn);
    void move(float dt);
    void StartRolling();


//    static PhysicsBallSystem* getInstance();

    QVector3D barycentricCoordinates(const QVector2D &dette, const QVector2D &p1, const QVector2D &p2, const QVector2D &p3);
    QVector3D getAkselerationVector(const QVector2D &ballPos);

    float xPosOnTarrain;
    float zPosOnTarrain;
    float gridSquareSize;
    int gridXPos;
    int gridZPos;
private:
    float xzScale{1};
    int mRows{1};
    int mCols{1};

    QVector3D mVelocity{0,0,0};
    QVector3D mAcceleration{0,0,0};
    QVector3D mPosition{0,0,0};
//    static PhysicsBallSystem* mInstance;
    GameObject* mTerrain;
};

#endif // PHYSICSBALLSYSTEM_H
