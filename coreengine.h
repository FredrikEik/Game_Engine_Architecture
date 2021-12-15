#ifndef COREENGINE_H
#define COREENGINE_H

#include <QObject>
#include <vector>
#include "input.h"
#include "gameobject.h"
#include "entity.h"
//Forward declarations
class ResourceManager;
class SoundSystem;
class RenderSystem;
class Camera;
class QTimer;
class GameObject;
//coreengine
//Made from QObject to use a Qt slots...
class CoreEngine : public QObject
{
    Q_OBJECT
public:
    CoreEngine(RenderSystem *renderSystemIn);

    //TODO:
    //Read and load all assets at start
    //Could read in list of all assets present, and only load on demand <<--- do this
    //List will be used in Editor GUI

    //Handle input

    //! getInstance to be able to use getInstance function
    static CoreEngine* getInstance();
    //! player - pointer to player
    GameObject * player;
    //! @fn togglePlayMode - toggles on and off play mode
    void togglePlayMode(bool shouldPlay);
    //! @fn setUpScene - making the scene that the objects will be drawn in
    void setUpScene();
    //! mScriptsystem - pointer to script system
    class ScriptSystem *mScriptSystem{nullptr};
    //! @fn handleInput - takes in and handles the players input
    void handleInput();
    //! mResourceManager - pointer to resource manager
    ResourceManager *mResourceManager{nullptr};
    //! mSoundSystem - pointer to sound system
    SoundSystem *mSoundSystem{nullptr};
    //! mRendersystem - pointer to render system
    RenderSystem *mRenderSystem{nullptr};
    //! mGameObject - pointer to game object
    GameObject *mGameObject{nullptr};
    //! @var mPlayerID - varible for the players id
    MAX_ENTITIES_TYPE mPlayerID;
    //! @var mEnemyID - variable for the pointer array for the enemys id
    std::vector<MAX_ENTITIES_TYPE> mEnemyID;
    //! @var mEntity - pointer array for the entities
    std::vector<Entity> mEntity;

    //! mGameCamer - pointer for the camera
    Camera *mGameCamera{nullptr};
    //! mEditorCamer - pointer for the editor camera
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
