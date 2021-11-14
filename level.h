#ifndef LEVEL_H
#define LEVEL_H
#include <QFile>        //Reading from file
#include <QJSEngine>    //The script engine itself!
#include <QObject>
#include <vector>
#include "camera.h"
#include "enemy.h"
#include "shapefactory.h"

class Script : public QObject
{
    //  for å kalle funksjoner fra javascript
    Q_OBJECT
public:
    Script(QObject *parent = nullptr);
    void sendSignal();
    std::vector<gsl::Vector3D> trophyPos;
signals:
    void signalOne();
public slots:
    //  en public slot kan kalles fra javascript, uten Q_INVOKABLE
    void scriptFunction(float x, float y, float z);
};
class Level : public QObject
{
public:
    Level(Camera* cam);
    ~Level();
    void initObjects();
    std::string createShapes(string shapeID);
    void readJS();
    void winner();
    void checkCollision();

    std::vector<NameComponent*> mNameComps;
    std::vector<TransformComponent*> mTransComps;
    std::vector<VisualObject*> mVisualObjects;
    std::vector<Square*> mWall;
    std::vector<Enemy*> mEnemies;
    std::vector<Circle*> mTrophies;
    ShapeFactory mShapeFactory;
    FrustumSystem* mFrustumSystem;
    SoundSource* mLaserSound{};
    Enemy* mEnemy;
    Player* mPlayer;
    Skybox *mSkyBox;
    Light * mLight{nullptr};
    int trophies{0};
private:
    static const int DIM_Z = 19;
    static const int DIM_X = 22;
//    static const int CENTER_X = DIM_X / 2;
//    static const int CENTER_Z = (DIM_Z + 1) / 2;
    static const int CENTER_Y = 0;
    static int GameBoard[DIM_Z][DIM_X];
    void DrawBoard();
    Script *script{nullptr};
    Camera* mCam;
    CollisionSystem* mColSystem;
};

#endif // LEVEL_H
