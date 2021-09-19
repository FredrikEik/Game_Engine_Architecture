#include "coreengine.h"

#include "resourcemanager.h"
#include "soundsystem.h"

#include "gameobject.h"
#include "renderwindow.h"
#include "camera.h"

CoreEngine* CoreEngine::mInstance = nullptr;    //static pointer to instance

CoreEngine::CoreEngine(RenderWindow *renderWindowIn) : mRenderWindow{renderWindowIn}
{
    mResourceManager = &ResourceManager::getInstance();
    mSoundSystem = SoundSystem::getInstance();
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
    SoundSystem::getInstance()->togglePlaySounds(isPlaying);
    mRenderWindow->isPlaying = isPlaying;
}

void CoreEngine::setUpScene()
{
    //********************** Making the object to be drawn **********************

    //Axis
    GameObject *temp = mResourceManager->addObject("axis");
    mRenderWindow->mGameObjects.push_back(temp);

    //dog triangle
    temp = mResourceManager->addObject("triangle");
    temp->mMaterial->mShaderProgram = 1;
    temp->mMaterial->mTextureUnit = 1; //mResourceManager->getTextureID()->;

    temp->mTransform->mMatrix.translate(0.f, 0.f, .5f);
    //Adds sound to moving triangle:
    mResourceManager->addComponent("caravan_mono.wav", temp);

    //Hack to test sound system
    temp->mSoundComponent->shouldPlay = true;

    mRenderWindow->mGameObjects.push_back(temp);

    //Suzanne:
    temp = mResourceManager->addObject("suzanne.obj");
    temp->mTransform->mMatrix.translate(1.f, 1.f, -.5f);
    temp->mTransform->mMatrix.scale(0.5f);
    mRenderWindow->mGameObjects.push_back(temp);

    //Suzanne 2 to check if resource handelig is correct:
    temp = mResourceManager->addObject("suzanne.obj");
    temp->mTransform->mMatrix.translate(-1.f, 1.f, -.5f);
    temp->mTransform->mMatrix.scale(0.3f);
    mRenderWindow->mGameObjects.push_back(temp);

    mEditorCamera = new Camera();
    mEditorCamera->mPosition = gsl::Vector3D(1.f, .5f, 4.f);
    mRenderWindow->mCurrentCamera = mEditorCamera;

    mResourceManager->setUpAllTextures();

    //Connect the gameloop timer to the render function:
    //This makes our render loop
    connect(mGameLoopTimer, SIGNAL(timeout()), this, SLOT(gameLoop()));
    //This timer runs the actual MainLoop
    //16 means 16ms = 60 Frames pr second (should be 16.6666666 to be exact...)
    mGameLoopTimer->start(16);
}

void CoreEngine::handleInput()
{
    //Camera

    //TODO: Probably a cleaner way to do this!
    mEditorCamera->setSpeed(0.f);  //cancel last frame movement
    if(mInput.RMB)
    {
        if(mInput.W)
            mEditorCamera->setSpeed(-mEditorCamera->mCameraSpeed);
        if(mInput.S)
            mEditorCamera->setSpeed(mEditorCamera->mCameraSpeed);
        if(mInput.D)
            mEditorCamera->moveRight(mEditorCamera->mCameraSpeed);
        if(mInput.A)
            mEditorCamera->moveRight(-mEditorCamera->mCameraSpeed);
        if(mInput.Q)
            mEditorCamera->updateHeigth(-mEditorCamera->mCameraSpeed);
        if(mInput.E)
            mEditorCamera->updateHeigth(mEditorCamera->mCameraSpeed);
    }
}

void CoreEngine::gameLoop()
{
    //Keyboard / mouse input
    handleInput();

    mEditorCamera->update();
    SoundSystem::getInstance()->update(mRenderWindow);

    mRenderWindow->render();
}

