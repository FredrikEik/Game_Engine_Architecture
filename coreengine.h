#ifndef COREENGINE_H
#define COREENGINE_H

//Forward declarations
class GameObjectManager;
class SoundSystem;
class RenderSystem;
class Camera;
class QTimer;
class MainWindow;

/**
   CoreEngine er en samling av mange systemer.
   Her finner en objekter av ressurshåndteringsklassene og game-loopen for logikk og rendering-kallet.
 */

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
    void togglePhysicsPlay(bool shouldSimulate);

    void setUpScene();
    void handleInput();

    GameObjectManager *mGameObjectManager{nullptr};
    SoundSystem *mSoundSystem{nullptr};
    RenderSystem *mRenderSystem{nullptr};
    MainWindow *mMainWindow{nullptr};
    //Two separate cameras to be able to render the frustum, etc for the game camera while in editor modus
    Camera *mGameCamera{nullptr};
    Camera *mEditorCamera{nullptr};
    QTimer *mGameLoopTimer{nullptr};    //timer that drives the gameloop
    MeshHandler *mMeshHandler{nullptr};

    bool isPlaying{false};           //is the game playing?
    bool isSimulatingPhysics{false}; //is physics simulating?

    Input mInput;

    //Reads info from JavaScript about all objects in the scene
    void readJSScene();


private slots:
    void gameLoop();

private:
    static CoreEngine* mInstance;   //Not singleton, but can use static getInstance()

    class Logger *mLogger{nullptr};
};

#endif // COREENGINE_H
