#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include <vector>

#include "gameobject.h"
#include "soundmanager.h"
#include "input.h"


class Camera;
class RenderWindow;
class ResourceManager;
class QTimer;
class SoundSourceComponent;
class MainWindow;
class PhysicsBallSystem;
class CollisionSystem;

class GameEngine : public QObject
{
    Q_OBJECT

public:
    GameEngine();
    void SetUpScene();
    void SetUpObjects();
    void HandleInput();
    void togglePlay(bool bInIsPlaying);

    static GameEngine* getInstance();

    void setRenderPointer(RenderWindow* temp, MainWindow *mainW);

    void playMusic(bool bIsPlaying);
    void resetWorld();

    bool bIsPlaying{false};

    Input mInput;

    //could simplify the createObject function to include the pusback in the mGameObjects in renderwindow
    //this way i dont need these functions and can only call the Createfunciton in resourcemanager.
    void CreateCube();
    void CreatePyramid();
    void CreateSphere();
    void CreateSuzanne();

    GameObject* mPlayer{nullptr};
    gsl::Vector3D initPlayerPos{0,0,0};

    GameObject* mLight{nullptr};
private slots:
    void GameLoop();

private:
    //TODO InputHandler

    ResourceManager *mResourceManager{nullptr};
    PhysicsBallSystem *mPhysicsBallSystem{nullptr};
    CollisionSystem *mCollisionSystem{nullptr};

    //std::vector<GameObject*> mGameObjects;

    GameObject* tempGameObject{nullptr};
    GameObject* mTerrainObject{nullptr};
    GameObject* mPhysicsBall{nullptr};

    Camera *mGameCamera{nullptr};
    Camera *mEditorCamera{nullptr};

    SoundSource* mStereoSound{nullptr};

    static GameEngine* mInstance;
    RenderWindow* mRenderwindow{nullptr};
    MainWindow* mMainWindow{nullptr};

    QTimer *mGameLoopRenderTimer{nullptr};
    void UpdateGameCameraFollow();

    std::vector<GameObject*> objBeforePlaying;

};

#endif // GAMEENGINE_H
