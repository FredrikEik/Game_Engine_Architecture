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

    //gsl::Vector3D getHeightMapPosition();

    gsl::Matrix4x4 getPlayerMatrix();

//    void updateTerrainPos(float x, float y, float z);

//    GameObject *mTerrainC{nullptr};


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
    GameObject *playerObject{nullptr};

    SoundSource* mStereoSound{nullptr};

    std::string treeNames[14] = {
        "1tree.obj", "2tree.obj", "3tree.obj", "4tree.obj", "5tree.obj", "6tree.obj",
        "1forestTree.obj", "2forestTree.obj", "3forestTree.obj", "4forestTree.obj", "5forestTree.obj", "6forestTree.obj",
        "1deadTree.obj", "2deadTree.obj",
    };
};

#endif // COREENGINE_H
