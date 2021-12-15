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

/**
   Her har vi klassen for Scripting hvor vi har brukt
   javascript for å lagre trofè-posisjoner.
   Vi bruker nå andre metoder for å plassere trofeer.
 */

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

/**
 * Level lager og holder på mapet og alle objekter i scenen vår.
 * Level inneholder også alle systemene våre, og det er her vi kaller
 * på de fleste funksjonene fra disse systemene. Blant annet kaller vi
 * på kollisjonstester fra CollisionSystem, og flytter på objekter ved
 * hjelp av MovementSystem.
 */

class Level : public QObject
{
public:
    Level(Camera* cam);
    ~Level();
    void initObjects();
    void createShapes(string shapeID);
    void readJS();
    void checkCollision();
    void updateEnemy(int randNr);
    void updatePlayer();
    void resetGame();
    void setupSound();
    void update(Camera* dc, InputComponent di, int randO, gsl::Vector3D col, int fC);
    void spawnParticle();
    void updateParticles(gsl::Vector3D color);

    std::vector<NameComponent*> mNameComps;
    std::vector<TransformComponent*> mTransComps;
    std::vector<VisualObject*> mVisualObjects;
    std::vector<Square*> mWall;
    std::vector<Enemy*> mEnemies;
    std::vector<Circle*> mTrophies;
    std::vector<SoundSource*> mSounds;
    std::vector<Particle*> mParticles;

    FrustumSystem* mFrustumSystem{nullptr};
    CollisionSystem* mCollisionSystem{nullptr};
    MovementSystem* mMovementSystem{nullptr};
    ParticleSystem* mParticleSystem{nullptr};

    Enemy* mEnemy{nullptr};
    Player* mPlayer{nullptr};
    Light* mLight{nullptr};
    ShapeFactory mShapeFactory;

    int trophies{0};
    int mLives{3};
    bool winner = false;
    bool playM = false;
private:
    static const int DIM_Z = 22;
    static const int DIM_X = 19;
    static const int CENTER_Y = 0;
    static int GameBoard[DIM_Z][DIM_X];
    void DrawBoard();
    bool wallCheck(int x, int z);
    Script* script{nullptr};
    Camera* mCam{nullptr};
    std::vector<VisualObject*> hearts;
};

#endif // LEVEL_H
