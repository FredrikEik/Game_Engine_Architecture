#ifndef LEVEL_H
#define LEVEL_H
#include <QFile>        //Reading from file
#include <QJSEngine>    //The script engine itself!
#include <QObject>
#include <vector>
#include "camera.h"
#include "enemy.h"
#include "particlesystem.h"
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
    void createShapes(string shapeID);
    void readJS();
    void checkCollision();
    void moveEnemy(int randNr);
    void movePlayer();
    void resetGame();
    void setupSound();
    void update(Camera* dc, Input di);

    std::vector<NameComponent*> mNameComps;
    std::vector<TransformComponent*> mTransComps;
    std::vector<VisualObject*> mVisualObjects;
    std::vector<Square*> mWall;
    std::vector<Enemy*> mEnemies;
    std::vector<Circle*> mTrophies;
    std::vector<SoundSource*> mSounds;
    ShapeFactory mShapeFactory;
    FrustumSystem* mFrustumSystem;
    Enemy* mEnemy;
    Player* mPlayer;
    Light * mLight{nullptr};
    void spawnParticle();
    void moveParticles(gsl::Vector3D color);
    std::vector<ParticleSystem*> mParticles;
    CollisionSystem* mCollisionSystem;
    MovementSystem *mMovementSystem;
    int trophies{0};
    int mLives{3};
private:
    static const int DIM_Z = 22;
    static const int DIM_X = 19;
    static const int CENTER_Y = 0;
    static int GameBoard[DIM_Z][DIM_X];
    void DrawBoard();
    bool wallCheck(int x, int z);
    Script *script{nullptr};
    Camera* mCam;


    std::vector<VisualObject*> hearts;


    bool hit{false};
};

#endif // LEVEL_H
