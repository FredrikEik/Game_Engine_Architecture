#ifndef COREENGINE_H
#define COREENGINE_H

#include "ECS/ECScoordinator.h"

//Forward declarations
class RenderWindow;
class Camera;
class QTimer;
class MainWindow;

//Made from QObject to use a Qt slots...
class CoreEngine : public QObject
{
    Q_OBJECT
public:
    CoreEngine(RenderWindow *renderSystemIn, MainWindow *mainWindowIn);

    //Not singleton now, but can use this function to get Engine instance.
    static CoreEngine* getInstance();

    void togglePlayMode(bool shouldPlay);

    RenderWindow *mRenderSystem{nullptr};

    MainWindow *mMainWindow{nullptr};

    //Two separate cameras to be able to render the frustum, etc for the game camera while in editor modus
    Camera *mGameCamera{nullptr};
    Camera *mEditorCamera{nullptr};

    QTimer *mGameLoopTimer{nullptr};    //timer that drives the gameloop

    bool isPlaying{false};              //is the game playing?
    bool bWiremode{false};
    bool bKeyReleased{true};

    float m_cameraSpeed{0.05f};

    Input m_input;

private slots:
    void gameLoop();

private:
    static CoreEngine* mInstance;   //Not singleton, but can use static getInstance()

    //BOSS MAN - This is supposed to oversee most things!
    std::unique_ptr<ECScoordinator> ECScord{nullptr};

};

#endif // COREENGINE_H
