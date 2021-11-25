#ifndef LEVEL_H
#define LEVEL_H
#include <QFile>        //Reading from file
#include <QJSEngine>    //The script engine itself!
#include <QObject>
#include <vector>
#include "camera.h"
#include "enemy.h"
#include "shapefactory.h"
#include <chrono>   //for sleep_for
#include <thread>   //for sleep_for

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
    std::string createShapes(string shapeID);
    void readJS();
    void winner();
    void checkCollision();
    void SoundHandler();
    void checkCoints();
    int cointAmount;

    std::vector<NameComponent*> mNameComp;
    std::vector<TransformComponent*> mTransformComp;
    std::vector<VisualObject*> mVisualObjects;
    std::vector<Square*> mWall;
    std::vector<Enemy*> mEnemies;
    std::vector<Circle*> mTrophies;
    ShapeFactory mShapeFactory;
    FrustumSystem* mFrustumSystem;
    SoundSource* mSound{};
    Enemy* mEnemy;
    Player* mPlayer;
    Skybox *mSkyBox;
    XYZ *xyz;
    Light * mLight{nullptr};
    int trophies{0};
    bool wallCheck(int z, int x);
    void moveEnemy();
    void CheckRotation();
    void resetGame();
private:
    static const int DIM_X = 19;
    static const int DIM_Z = 22;
    bool checkEnemyRotFront = false;
    bool checkEnemyRotBack = false;
    bool checkEnemyRotLeft = false;
    bool checkEnemyRotRight = false;
    static const int CENTER_Y = 0;
    static int GameBoard[DIM_Z][DIM_X];
    void DrawBoard();
    Script *script{nullptr};
    Camera* mCam;
    CollisionSystem* mColSystem;
    int mLives{2};

};

#endif // LEVEL_H
