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
   @class Script - Her lager vi scriptet vårt, dette blir ikke brukt, men i en tidligere versjon av filen så plasserte vi ut trophy med det.
 */

class Script : public QObject
{
    //  for å kalle funksjoner fra javascript.
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
    /**
    @class Level - Denne klassen lager mappet og holder generelt styr på alt av gameloop, script og nødvendige kolisjonstester.
    @fn createShapes - Lager objekter slik at vi kan bruke knapp for å spawne.
    @param shapeID - sender inn ID til objektet.
    @fn checkCollision - Bruker kollisjonsklassens funksjoner til å sjekke kollisjon.
    @fn SoundHandler - tilrettelegger lyden vi skal inkludere.
    @fn wallCheck - gå igjennom mappet og sjkker om det er vegg der.
    @param x - sender inn x for å kunne sende inn x posisjon til objekt.
    @param z - sender inn z for å kunne sende inn z posisjon til objekt.
    @fn updateEnemy - Oppdaterer enemy ihenhold med movement.
    @fn updatePlayer - Oppdaterer spiller ihenhold med movement.
    @fn updateParticles - Oppdaterer particle ihenhold med movement.
    @param mColor - sender inn color så vi kan få forskjellige farger på particle.
    @fn resetGame - resetter spillerens, enemy, trophy's posisjon, resetter mLives, winCondition.
    @fn spawnParticle - lager en particle.
    @fn update - oppdaterer spiller, enemy og particle movement og spawner particler.
    @param dc - sender inn camera peker.
    @param di - Sender inn input.
    @param randO - sender inn en int så vi kan sende inn et random nummer.
    @param col - sender in col for å sende inn farge.
    @param fc - sender inn framcount.
    @fn playSound - spiller lyd
    @param soundID - sender inn ID til lyd
    */

    Level(Camera* cam);
    ~Level();
    void createShapes(string shapeID);
    void readJS();
    void checkCollision();
    void SoundHandler();
    bool wallCheck(int z, int x);
    void updateEnemy(int randNum);
    void updatePlayer();
    void updateParticles(gsl::Vector3D mColor);
    void resetGame();
    void spawnParticle();
    void update(Camera* dc, Input di, int randO, gsl::Vector3D col, int fC);
    void playSound(int SoundID);

    Texture *mTexture{nullptr};
    MovementSystem *mMoveSys{nullptr};
    CollisionSystem *mCollisionSystem{nullptr};
    std::vector<NameComponent*> mNameComp;
    std::vector<TransformComponent*> mTransformComp;
    std::vector<VisualObject*> mVisualObjects;
    std::vector<Square*> mWall;
    std::vector<Enemy*> mEnemies;
    std::vector<Circle*> mTrophies;
    std::vector<SoundSource*> mSound;
    std::vector<Particle*> mParticles;
    ParticleSystem *mParticleSystem;
    ShapeFactory mShapeFactory;
    FrustumSystem *mFrustumSystem{nullptr};
    SoundSource *mDeathSound;
    SoundSource *mChompSound;
    Enemy *mEnemy{nullptr};
    Player *mPlayer{nullptr};
    Light *mLight{nullptr};



    bool playM = false;
    int trophies{0};
    int mLives{3};
    bool winCondition = false;
private:
    static const int DIM_X = 19;
    static const int DIM_Z = 22;
    static const int CENTER_Y = 0;
    static int GameBoard[DIM_Z][DIM_X];

    void DrawBoard();
    Script *script{nullptr};
    Camera* mCam;
    CollisionSystem* mColSystem{nullptr};
    std::vector<VisualObject*> hearts;


};

#endif // LEVEL_H
