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
}

CoreEngine *CoreEngine::getInstance()
{
    return mInstance;
}

void CoreEngine::GameLoop()
{
    HandleInput();

    mEditorCamera->update();

    mRenderWindow->render();
}

void CoreEngine::SetUpScene()
{

    GameObject *temp = mResourceManager->CreateObject("Cube");
    mRenderWindow->mGameObjects.push_back(temp);

    for(int i{0}; i < 40; i++)
   {
       for(int j{0}; j < 40; j++)
       {
           temp = mResourceManager->CreateObject("suzanne.obj");
           temp->transform->mMatrix.translate(2.f*(i), -3.f, -2.f*(j));
           mRenderWindow->mGameObjects.push_back(temp);
       }
   }

//    temp = mResourceManager->CreateObject("suzanne.obj");
//    temp->transform->mMatrix.translate(2.0f, 0, -4.0f);
//    mRenderWindow->mGameObjects.push_back(temp);

/** Å rendre 2 av samme objekt funker ikke og gir en opengl error message */ // Array object is not active (1282) "HighSeverity"

//    temp = mObjectManager.CreateObject("suzanne.obj");
//    temp->transform->mMatrix.translate(0, 0, 0);
//    mGameObjects.push_back(temp);


    //********************** Set up camera **********************
    mEditorCamera->setPosition(gsl::Vector3D(1.f, .5f, 4.f));
    mRenderWindow->setToCurrentCamera(mEditorCamera);


    SoundSource* mStereoSound = SoundManager::getInstance()->createSource("Stereo", gsl::Vector3D(0.0f, 0.0f, 0.0f),
                                                            "..\\GEA2021\\Assets\\Sounds\\stereo.wav", false, 1.0f);
    mStereoSound->play();


    //Connect the gameloop timer to the render function:
    connect(mGameLoopTimer, SIGNAL(timeout()), this, SLOT(GameLoop()));
    //This timer runs the actual MainLoop
    //16 means 16ms = 60 Frames pr second (should be 16.6666666 to be exact...)
    mGameLoopTimer->start(16);
}

void CoreEngine::HandleInput() //TODO: fix input so that it works from CoreEngine
{
    //mEditorCamera
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
