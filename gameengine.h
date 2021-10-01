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

class GameEngine : public QObject
{
    Q_OBJECT

public:
    GameEngine();
    void SetUpScene();
    void HandleInput();

    static GameEngine* getInstance();

    void setRenderPointer(RenderWindow* temp, MainWindow *mainW);

    void playMusic(bool bIsPlaying);

private slots:
    void GameLoop();

private:
    //TODO InputHandler

     ResourceManager *mResourceManager{nullptr};
    //std::vector<GameObject*> mGameObjects;

    GameObject* tempGameObject;

    Camera *mGameCamera{nullptr};
    Camera *mEditorCamera{nullptr};

    SoundSource* mStereoSound{nullptr};

    static GameEngine* mInstance;
    RenderWindow* mRenderwindow{nullptr};
    MainWindow* mMainWindow{nullptr};

    QTimer *mGameLoopRenderTimer{nullptr};
};

#endif // GAMEENGINE_H
