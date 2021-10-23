#ifndef PHYSICSBALLSYSTEM_H
#define PHYSICSBALLSYSTEM_H

#include "QVector3D"
#include "gameobject.h"
#include "queue"
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

private:
    float xPosOnTarrain;
    float zPosOnTarrain;
    float gridSquareSize;
    int gridXPos;
    int gridZPos;
    float xzScale{1};
    int mRows{1};
    int mCols{1};

    bool firstTriangle{true};
    QVector2D startPoint{0,0};
    bool setup{true};
    bool bOnBotLeft{true};

    int lastTirangleID{0};
    int currentTirangleID{0};

    //hardcoded for now should change when spawning on different square
    //make a init bool to get and set the curret pos;
    QVector2D lastGridXZPos{0,0};
    QVector2D currentGridXZPos{0,0};

    std::queue<QVector2D> lastAndCurrentQ;

    bool normalSetup{true};
    QVector3D lastNormal{0,0,0};
    QVector3D currentNormal{0,0,0};
    std::queue<QVector3D> normalLastAndCurrentQ;

//    int lastSquareID{0};
//    int currentSquareID{0};

//    QVector3D lastNormal{9,9,9};
//    QVector3D BotLeftNormal{0,0,0};
//    QVector3D TopRightNormal{0,0,0};

    QVector3D mVelocity{0,0,0};
    QVector3D mAcceleration{0,0,0};
    QVector3D mPosition{0,0,0};
//    static PhysicsBallSystem* mInstance;
    GameObject* mTerrain;
};

#endif // PHYSICSBALLSYSTEM_H
