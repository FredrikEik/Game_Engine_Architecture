#ifndef COREENGINE_H
#define COREENGINE_H

#include <QObject>
#include "input.h"
//Forward declarations
class GameObjectManager;
class SoundSystem;
class RenderSystem;
class Camera;
class QTimer;
class MainWindow;

//Made from QObject to use a Qt slots...
class CoreEngine : public QObject
{
    Q_OBJECT
public:
    CoreEngine(RenderSystem *renderSystemIn, MainWindow *mainWindowsIn);

    //TODO:
    //Read and load all assets at start
    //Could read in list of all assets present, and only load on demand <<--- do this
    //List will be used in Editor GUI

    //Handle input

    ///Not singleton now, but can use this function to get Engine instance
    static CoreEngine* getInstance();

    void togglePlayMode(bool shouldPlay);

    void setUpScene();

    void handleInput();

    GameObjectManager *mGameObjectManager{nullptr};
    SoundSystem *mSoundSystem{nullptr};
    RenderSystem *mRenderSystem{nullptr};

    MainWindow *mMainWindow{nullptr};

    Camera *mGameCamera{nullptr};
    Camera *mEditorCamera{nullptr};

    QTimer *mGameLoopTimer{nullptr};    //timer that drives the gameloop

    bool isPlaying{false};              //is the game playing?

    Input mInput;

private slots:
    void gameLoop();

private:
    static CoreEngine* mInstance;   //Not singleton, but can use static getInstance()

};

#endif // COREENGINE_H
