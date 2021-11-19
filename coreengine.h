#ifndef COREENGINE_H
#define COREENGINE_H

#include <QObject>
#include "input.h"
#include "gameobject.h"
#include "collisionhandler.h"
#include "meshhandler.h"
#include "scene.h"
#include "mainwindow.h"
//Forward declarations
class ResourceManager;
class SoundSystem;
class RenderSystem;
class scene;
class Camera;
class QTimer;

//Made from QObject to use a Qt slots...
class CoreEngine : public QObject
{
    Q_OBJECT
public:
    CoreEngine(RenderSystem *renderSystemIn);

    //TODO:
    //Read and load all assets at start
    //Could read in list of all assets present, and only load on demand <<--- do this
    //List will be used in Editor GUI

    //Handle input

    ///Not singleton now, but can use this function to get Engine instance
    static CoreEngine* getInstance();
    GameObject * player;
    GameObject *axis;
    GameObject *enemy;
    GameObject *boss;
    GameObject* mTerrain;
    GameObject* Particles;
    GameObject* projectile;
    GameObject* skybox;

    std::vector<GameObject *> enemies;

    bool playerSpawned = false;
    bool particlesSpawned = false;
    bool enemySpawned = false;
    bool goatDead = false;
    bool testDelete = false;

    bool startTimer = false;

    float tempPosX{0};
    float tempPosY{0};
    float tempPosZ{0};

    int score = 0;

    MeshData linebox;
    MeshData linebox2;

    MeshData circle1;
    MeshData circle2;

    scene *scene;

    void togglePlayMode(bool shouldPlay);

    void setUpScene();
    void testScene();
    void updateCamera();
    void updateScene();

    void spawnParticles(GameObject *temp);
    void spawnProjectile();

    void handleInput();
    ResourceManager *mResourceManager{nullptr};
    SoundSystem *mSoundSystem{nullptr};
    RenderSystem *mRenderSystem{nullptr};
    void playSound(std::string assetName);

    Camera *mGameCamera{nullptr};
    Camera *mEditorCamera{nullptr};

    QTimer *mGameLoopTimer{nullptr};    //timer that drives the gameloop

    bool isPlaying{false};              //is the game playing?

    Input mInput;

private slots:
    void gameLoop();

private:
    static CoreEngine* mInstance;   //Not singleton, but can use static getInstance()


    MainWindow *mMainWindow;
};

#endif // COREENGINE_H
