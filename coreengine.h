#ifndef COREENGINE_H
#define COREENGINE_H

#include <QObject>
#include "input.h"

#include "ResourceManager.h"
#include "soundmanager.h"

#include "GameObject.h"
#include "renderwindow.h"
#include "camera.h"

class QTimer;
class SoundManager;
class ResourceManager;
class RenderWindow;
class Camera;

/**
 * CoreEngine is a class that should have all simple main functionalities, like
 * for example the game-loop itself, Filling the scene with objects, Keyboard/mouse input handling.
 */
class CoreEngine : public QObject
{
    Q_OBJECT
public:
    CoreEngine(RenderWindow *renderWindow);

    static CoreEngine *getInstance();

    void SetUpScene();
    void resetScene();

    void startGameLoopTimer();

    void HandleInput();
    void PlayerInput();
    void EditorCameraInput();

    void CreateObjectButton(std::string objName);
    void MoveSelectionArrow(gsl::Vector3D pos);

    void swapCurrentCamera();
    void initCameraProjectionMatrixes(float AspectRatio);

    void playStartGameSound();

    void shootBullet();

    Input mInput;

    bool isPlaying();

    Camera* getGameCamera();

    gsl::Matrix4x4 getPlayerMatrix();

    GameObject *playerObject{nullptr};

    SoundSource* mReloadGun{nullptr};
    SoundSource* mStereoSound{nullptr};
    SoundSource* mGunShot{nullptr};

private slots:
    void GameLoop();


private:

    bool bUsingGameCamera{false}; //When true, GameCamera is used. When false, EditorCamera is used

    static CoreEngine* mInstance;

    QTimer *mGameLoopTimer{nullptr};
    SoundManager *mSoundManager{nullptr};
    ResourceManager* mResourceManager{nullptr};
    RenderWindow *mRenderWindow{nullptr};
    Frustum *frustum{nullptr};

    Camera *mEditorCamera{nullptr};
    Camera *mGameCamera{nullptr};

    GameObject *mGameCameraMesh{nullptr};

    /**
     * String array that holds names of all tree objects. I was supposed to spawn random trees out on the
     * terrain, but there were errors with the objects that created a huge collisionSphere, so it was not
     * possible to use mousePicking on a tree.
     */
    std::string treeNames[14] = {
        "1tree.obj", "2tree.obj", "3tree.obj", "4tree.obj", "5tree.obj", "6tree.obj",
        "1forestTree.obj", "2forestTree.obj", "3forestTree.obj", "4forestTree.obj", "5forestTree.obj",
        "1deadTree.obj", "2deadTree.obj"
    };
};

#endif // COREENGINE_H
