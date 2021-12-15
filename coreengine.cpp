#include "coreengine.h"
#include "gameobject.h"
#include "camera.h"
#include "mainwindow.h"
#include "renderwindow.h"

CoreEngine* CoreEngine::mInstance = nullptr;    //static pointer to instance

CoreEngine::CoreEngine(RenderWindow *renderSystemIn, MainWindow *mainWindowIn)
    : mRenderSystem{renderSystemIn}, mMainWindow{mainWindowIn}
{
    // current date/time based on current system
    time_t now = time(0);
    // convert now to string form
    char* dt = ctime(&now);

    mInstance = this;

    //Make the gameloop timer:
    mGameLoopTimer = new QTimer(this);
}

CoreEngine *CoreEngine::getInstance()
{
    return mInstance;
}

void CoreEngine::togglePlayMode(bool shouldPlay)
{
    isPlaying = shouldPlay;
//    mRenderSystem->mIsPlaying = isPlaying;
}

void CoreEngine::gameLoop()
{

}

