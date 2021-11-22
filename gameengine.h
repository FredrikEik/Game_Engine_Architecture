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

private slots:
    void GameLoop();
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
    bool bBallPhysicsIsPlaying{false};

    Input mInput;

    //could simplify the createObject function to include the pusback in the mGameObjects in renderwindow
    //this way i dont need these functions and can only call the Createfunciton in resourcemanager.
    void CreateCube();
    void CreatePyramid();
    void CreateSphere();
    void CreateSuzanne();
    void CreateObject(std::string filePath, bool bUsingLOD, std::string textureName);



    GameObject *mXYZaxis{nullptr};
    GameObject* mPlayer{nullptr};
    gsl::Vector3D initPlayerPos{0,0,0};

    GameObject* mLight{nullptr};
    GameObject* mLight2{nullptr};
    void rotateLight();
    
    std::vector<std::pair<QString,std::vector<GameObject*>>> mLevels;
    void saveScene(std::string &levelName);
    void loadScene(std::string &levelName);
    void updateAllLevels();

    void setRotationX(float value);

    GameObject* mPhysicsBall{nullptr};
    void ResetBallVelocity();

private:
    //TODO InputHandler

    ResourceManager *mResourceManager{nullptr};
    PhysicsBallSystem *mPhysicsBallSystem{nullptr};
    CollisionSystem *mCollisionSystem{nullptr};

    //std::vector<GameObject*> mGameObjects;

    GameObject* tempGameObject{nullptr};
    GameObject* mTerrainObject{nullptr};
    GameObject* mHeightCurveObject{nullptr};

    Camera *mGameCamera{nullptr};
    Camera *mEditorCamera{nullptr};

    SoundSource* mStereoSound{nullptr};
    SoundSource* m3DTestSound{nullptr};

    static GameEngine* mInstance;
    RenderWindow* mRenderwindow{nullptr};
    MainWindow* mMainWindow{nullptr};

    QTimer *mGameLoopRenderTimer{nullptr};
    void UpdateGameCameraFollow();

    std::vector<GameObject*> objBeforePlaying;

};

#endif // GAMEENGINE_H
