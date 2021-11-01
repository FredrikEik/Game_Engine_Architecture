#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include <vector>

#include "gameobject.h"
#include "soundmanager.h"


class Camera;
class RenderWindow;
class ResourceManager;
class QTimer;
class SoundSourceComponent;
class MainWindow;
class PhysicsBallSystem;

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

    //could simplify the createObject function to include the pusback in the mGameObjects in renderwindow
    //this way i dont need these functions and can only call the Createfunciton in resourcemanager.
    void CreateCube();
    void CreatePyramid();
    void CreateSphere();
    void CreateSuzanne();

    GameObject* mPlayer{nullptr};
private slots:
    void GameLoop();

private:
    //TODO InputHandler

    ResourceManager *mResourceManager{nullptr};
    PhysicsBallSystem *mPhysicsBallSystem{nullptr};
    //std::vector<GameObject*> mGameObjects;

    GameObject* tempGameObject{nullptr};
    GameObject* mTerrainObject{nullptr};
    GameObject* mPhysicsBall{nullptr};

    Camera *mGameCamera{nullptr};
    Camera *mEditorCamera{nullptr};

    SoundSource* mStereoSound{nullptr};

    //bool isPlaying{false};

    static GameEngine* mInstance;
    RenderWindow* mRenderwindow{nullptr};
    MainWindow* mMainWindow{nullptr};

    QTimer *mGameLoopRenderTimer{nullptr};
    void UpdateGameCameraFollow();
};

#endif // GAMEENGINE_H
