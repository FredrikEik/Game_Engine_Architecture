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
#include "texture.h"
#include "particles.h"

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
    void createShapes(string shapeID);
    void readJS();
    void winner();
    void checkCollision();
    void SoundHandler();
    void checkCoints();
    int cointAmount;

    Texture *mTexture;
    std::vector<NameComponent*> mNameComp;
    std::vector<TransformComponent*> mTransformComp;
    std::vector<VisualObject*> mVisualObjects;
    std::vector<Square*> mWall;
    std::vector<Enemy*> mEnemies;
    std::vector<Circle*> mTrophies;
    std::vector<SoundSource*> mSound;
    std::vector<Particles*> mParticles;

    void playSound(int SoundID);

    ShapeFactory mShapeFactory;
    FrustumSystem *mFrustumSystem;

    SoundSource *mDeathSound;
    SoundSource *mChompSound;
    Enemy *mEnemy;
    Player *mPlayer;
    XYZ *xyz;
    Light *mLight{nullptr};
   // Particles *mParticle;


    bool wallCheck(int z, int x);
    void moveEnemy(int randNum);
    void movePlayer();
    void moveParticles(gsl::Vector3D mColor);
    void resetGame();
    void GameLoop();
    void spawnParticle();

private:
    static const int DIM_X = 19;
    static const int DIM_Z = 22;
    static const int CENTER_Y = 0;
    static int GameBoard[DIM_Z][DIM_X];
    void DrawBoard();

    Script *script{nullptr};
    Camera* mCam;
    CollisionSystem* mColSystem;

    int mLives{3};
    bool isValidPos = false;
    int angle =0;
    int trophies{0};
};

#endif // LEVEL_H
