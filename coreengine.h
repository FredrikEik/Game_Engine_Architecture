#ifndef COREENGINE_H
#define COREENGINE_H

#include <QObject>
#include "input.h"
#include "gameobject.h"
#include "collisionhandler.h"
#include "meshhandler.h"
#include "mainwindow.h"
//Forward declarations
class ResourceManager;
class SoundSystem;
class RenderSystem;
class MeshHandler;
class Camera;
class QTimer;

class CoreEngine : public QObject
{
    Q_OBJECT
public:
    CoreEngine(RenderSystem *renderSystemIn);

    ///Not singleton now, but can use this function to get Engine instance
    static CoreEngine* getInstance();

    ///liste over spillobjekter
    GameObject * player;
    GameObject *axis;
    GameObject *enemy;
    GameObject *boss;
    GameObject* mTerrain;
    GameObject* Particles;
    GameObject* projectile;
    GameObject* skybox;
    GameObject* Las;
    GameObject* ball;
    std::vector<GameObject *> enemies;
    std::vector<GameObject*> Rain;

    bool playerSpawned = false;
    bool particlesSpawned = false;
    bool enemySpawned = false;
    bool goatDead = false;
    bool particleTimer = false;
    bool startTimer = false;
    bool isPlaying{false};              //is the game playing?
    bool spawnBoss = false;

    float tempPosX{0};
    float tempPosY{0};
    float tempPosZ{0};

    int score = 0;

    MeshData linebox;
    MeshData linebox2;
    MeshData circle1;
    MeshData circle2;    
    Input mInput;

    ///coreengine sine funksjoner
    void togglePlayMode(bool shouldPlay);
    void setUpScene();
    void testScene();
    void RollingBallSimulation();
    void updateCamera();
    void updateScene();
    void UpdateSimulation();
    void reset();
    void loadBoss(std::string scene);
    void spawnParticles(GameObject *temp);
    void spawnProjectile();
    void handleInput();
    void playSound(std::string assetName);

    Camera *mGameCamera{nullptr};
    Camera *mEditorCamera{nullptr};
    ResourceManager *mResourceManager{nullptr};
    SoundSystem *mSoundSystem{nullptr};
    RenderSystem *mRenderSystem{nullptr};
    MeshHandler *mMeshHandler{nullptr};
    QTimer *mGameLoopTimer{nullptr};    ///timer that drives the gameloop

private slots:
    void gameLoop();

private:
    static CoreEngine* mInstance;


    MainWindow *mMainWindow;
};

#endif // COREENGINE_H
