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

    GameObject *temp = mResourceManager->CreateMainCharacter("cube.obj");
    temp->transform->mMatrix.scale(0.2f);
    mRenderWindow->addToGameObjects(temp);

    temp = mResourceManager->CreateObject("suzanne.obj");
    temp->transform->mMatrix.translate(gsl::Vector3D(57.f, -1.f, 0.f));
    mRenderWindow->addToGameObjects(temp);


    for(int i{0}; i < 40; i++)
   {
       for(int j{0}; j < 40; j++)
       {
           temp = mResourceManager->CreateObject("suzanne.obj");
           temp->transform->mMatrix.translate(3.f*(i), -5.f, -3.f*(j));
           mRenderWindow->addToGameObjects(temp);
       }
   }

    //********************** Set up cameras **********************
    mGameCamera->setPosition(gsl::Vector3D(57.f, .5f, 9.f));
    mGameCamera->pitch(20); //tilt the camera down

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
    // currentCamera = mGameCamera
    if(isPlaying())
    {
        PlayerInput();
    }
    else // currentCamera = mEditorCamera
    {
        EditorCameraInput();
    }
}

void CoreEngine::PlayerInput()
{
    if(mRenderWindow->getInput().W || mRenderWindow->getInput().A || mRenderWindow->getInput().S || mRenderWindow->getInput().D){
        for( int i = 0; i < mRenderWindow->getGameObjects().size(); i++){ //probably not a good way to do this

            if(mRenderWindow->getGameObjects()[i]->mName == "cube.obj"){
                if(mRenderWindow->getInput().W){
                    mRenderWindow->getGameObjects()[i]->move(0, 0, -0.7);
                    mGameCamera->setPosition(mGameCamera->position() + gsl::Vector3D(0, 0, -0.14f));
                }
                if(mRenderWindow->getInput().A){
                    mRenderWindow->getGameObjects()[i]->move(-0.5f, 0, 0);
                    mGameCamera->setPosition(mGameCamera->position() + gsl::Vector3D(-0.1f, 0, 0));
                }
                if(mRenderWindow->getInput().S){
                    mRenderWindow->getGameObjects()[i]->move(0, 0, 0.7);
                    mGameCamera->setPosition(mGameCamera->position() + gsl::Vector3D(0, 0, 0.14f));
                }
                if(mRenderWindow->getInput().D){
                    mRenderWindow->getGameObjects()[i]->move(0.5f, 0, 0);
                    mGameCamera->setPosition(mGameCamera->position() + gsl::Vector3D(0.1f, 0, 0));
                }
            }
        }
    }
}

void CoreEngine::EditorCameraInput()
{
    mEditorCamera->setSpeed(0.f);  //cancel last frame movement
    if(mRenderWindow->getInput().RMB)
    {
        if(mRenderWindow->getInput().W)
            mEditorCamera->setSpeed(-mEditorCamera->mCameraSpeed);
        if(mRenderWindow->getInput().S)
            mEditorCamera->setSpeed(mEditorCamera->mCameraSpeed);
        if(mRenderWindow->getInput().D)
            mEditorCamera->moveRight(mEditorCamera->mCameraSpeed);
        if(mRenderWindow->getInput().A)
            mEditorCamera->moveRight(-mEditorCamera->mCameraSpeed);
        if(mRenderWindow->getInput().Q)
            mEditorCamera->updateHeigth(-mEditorCamera->mCameraSpeed);
        if(mRenderWindow->getInput().E)
            mEditorCamera->updateHeigth(mEditorCamera->mCameraSpeed);

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

Camera *CoreEngine::getGameCamera()
{
    return mGameCamera;
}
