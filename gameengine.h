#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include <vector>

#include "gameobject.h"
class Camera;
class RenderWindow;
class ResourceManager;
class QTimer;

class GameEngine : public QObject
{
    Q_OBJECT

public:
    GameEngine();
    void SetUpScene();
    void HandleInput();

    static GameEngine* getInstance();

    void setRenderPointer(RenderWindow* temp);

private slots:
    void GameLoop();

private:
    //TODO InputHandler

     ResourceManager *mResourceManager{nullptr};
    //std::vector<GameObject*> mGameObjects;

    Camera *mGameCamera{nullptr};
    Camera *mEditorCamera{nullptr};

    static GameEngine* mInstance;
    RenderWindow* mRenderwindow{nullptr};

    QTimer *mGameLoopRenderTimer{nullptr};
};

#endif // GAMEENGINE_H
