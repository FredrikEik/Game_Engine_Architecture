#include "coreengine.h"


CoreEngine* CoreEngine::mInstance = nullptr;

CoreEngine::CoreEngine(RenderWindow *renderWindow)
{
    mRenderWindow = renderWindow;

    mResourceManager = &ResourceManager::getInstance();
    mSoundManager = SoundManager::getInstance();

    mInstance = this;

    mGameLoopTimer = new QTimer(this);
    mEditorCamera = new Camera();
    mGameCamera = new Camera();
}

CoreEngine *CoreEngine::getInstance()
{
    return mInstance;
}

void CoreEngine::GameLoop()
{
    HandleInput();

    mRenderWindow->getCurrentCamera()->update();

    mRenderWindow->render();
}

void CoreEngine::SetUpScene()
{

    GameObject *temp;/* = mResourceManager->CreateObject("suzanne.obj");
    mRenderWindow->mGameObjects.push_back(temp);*/

    for(int i{0}; i < 40; i++)
   {
       for(int j{0}; j < 40; j++)
       {
           temp = mResourceManager->CreateObject("suzanne.obj");
           temp->transform->mMatrix.translate(3.f*(i), -3.f, -3.f*(j));
           mRenderWindow->addToGameObjects(temp);
       }
   }

    //********************** Set up cameras **********************
    mGameCamera->setPosition(gsl::Vector3D(57.f, .5f, -9.f));

    mEditorCamera->setPosition(gsl::Vector3D(1.f, .5f, 4.f));
    mRenderWindow->setToCurrentCamera(mEditorCamera);


    mStereoSound = SoundManager::getInstance()->createSource("Stereo", gsl::Vector3D(0.0f, 0.0f, 0.0f),
                                                            "..\\GEA2021\\Assets\\Sounds\\stereo.wav", false, 1.0f);

//    playStartGameSound();

    //Connect the gameloop timer to the render function:
    connect(mGameLoopTimer, SIGNAL(timeout()), this, SLOT(GameLoop()));
    //This timer runs the actual MainLoop
    //16 means 16ms = 60 Frames pr second
    mGameLoopTimer->start(16);
}

void CoreEngine::HandleInput() //TODO: fix input so that it works from CoreEngine
{
    //mEditorCamera
    if(mRenderWindow->getCurrentCamera() == mEditorCamera){
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
    else // currentCamera = mGameCamera
    {

    }
}

void CoreEngine::swapCurrentCamera()
{
    if(mRenderWindow->getCurrentCamera() == mEditorCamera)
    {
        bUsingGameCamera = true;
        mRenderWindow->setToCurrentCamera(mGameCamera);
    }
    else{
        bUsingGameCamera = false;
        mRenderWindow->setToCurrentCamera(mEditorCamera);
    }
}

void CoreEngine::initCameraProjectionMatrixes(float mAspectRatio)
{
    mEditorCamera->mProjectionMatrix.perspective(45.f, mAspectRatio, 0.1f, 100.f);
    mGameCamera->mProjectionMatrix.perspective(45.f, mAspectRatio, 0.1f, 100.f);
}

void CoreEngine::playStartGameSound()
{
    mStereoSound->play();
}

bool CoreEngine::isPlaying()
{
    return bUsingGameCamera;
}
