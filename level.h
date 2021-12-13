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
    void createShapes(string shapeID);
    void readJS();
    void checkCollision();
    void moveEnemy(double randNr);
    void movePlayer();
    void resetGame();
    void setupSound();

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
    Skybox *mSkyBox;
    Light * mLight{nullptr};
private:
    static const int DIM_Z = 22;
    static const int DIM_X = 19;
    static const int CENTER_Y = 0;
    static int GameBoard[DIM_Z][DIM_X];
    void DrawBoard();
    bool wallCheck(int x, int z);
    Script *script{nullptr};
    Camera* mCam;
    CollisionSystem* mColSystem;
    std::vector<VisualObject*> hearts;
    int mLives{3};
    int trophies{0};
    bool hit{false};
};

#endif // LEVEL_H
