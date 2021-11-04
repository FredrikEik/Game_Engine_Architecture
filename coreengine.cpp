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
    mGameCameraMesh = new GameObject();
    frustum = new Frustum();
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

    mGameCameraMesh = mResourceManager->CreateObject("camera.obj");
    mGameCameraMesh->transform->mMatrix.translate(gsl::Vector3D(57.f, .5f, 9.f));
    mGameCameraMesh->transform->mMatrix.rotateX(20);
    mGameCameraMesh->mesh->collisionsEnabled = false;
    mRenderWindow->addToGameObjects(mGameCameraMesh);

    temp = mResourceManager->CreateObject("suzanne.obj");
    temp->transform->mMatrix.translate(gsl::Vector3D(57.f, -1.f, 0.f));
    mRenderWindow->addToGameObjects(temp);

    temp = mResourceManager->CreateObject("arrow.obj");
    temp->transform->mMatrix.translate(9999,9999,9999);
    temp->transform->mMatrix.scale(0.3f);
    temp->mesh->collisionsEnabled = false;
    mRenderWindow->addToGameObjects(temp);




    for(int i{0}; i < 40; i++)
   {
       for(int j{0}; j < 40; j++)
       {
           temp = mResourceManager->CreateObject("suzanne.obj");
           temp->transform->mMatrix.translate(3.f*i, -5.f, -3.f*j);
           mRenderWindow->addToGameObjects(temp);
       }
   }

    //********************** Set up cameras **********************
    mGameCamera->setPosition(gsl::Vector3D(57.f, .5f, 9.f));
    mGameCamera->pitch(20); //tilt the camera down

    mEditorCamera->setPosition(gsl::Vector3D(25.f, .5f, 4.f));
    mRenderWindow->setToCurrentCamera(mEditorCamera);


    mStereoSound = SoundManager::getInstance()->createSource("Stereo", gsl::Vector3D(0.0f, 0.0f, 0.0f),
                                                            "..\\GEA2021\\Assets\\Sounds\\stereo.wav", false, 1.0f);
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
    gsl::Vector3D move{0,0,0};
    if(mRenderWindow->getInput().W){
        move.z -= 0.7f;
    }
    if(mRenderWindow->getInput().A){
        move.x -= 0.5f;
    }
    if(mRenderWindow->getInput().S){
        move.z += 0.7f;
    }
    if(mRenderWindow->getInput().D){
        move.x += 0.5f;
    }
    mRenderWindow->setPlayerMovement(move.x, move.y, move.z);
    mGameCamera->setPosition(mGameCamera->position() + gsl::Vector3D(move.x/5, move.y/5, move.z/5));

    move = (move/5) + mGameCameraMesh->transform->mMatrix.getPosition();
    mGameCameraMesh->transform->mMatrix.setPosition(move.x, move.y, move.z);
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

void CoreEngine::CreateObjectButton(std::string objName)
{
    GameObject *temp = mResourceManager->CreateObject(objName);
    temp->transform->mMatrix.scale(2);
    temp->transform->mMatrix.setPosition(0, 0, -10.f);
    mRenderWindow->addToGameObjects(temp);
}

void CoreEngine::MoveSelectionArrow(gsl::Vector3D pos)
{
    for(auto i : mRenderWindow->getGameObjects())
    {
        if(i->mName == "arrow.obj")
        {
            if(mRenderWindow->getIndexToPickedObject() == -1)
            {
                i->transform->mMatrix.setPosition(9999, 9999, 9999);
                return;
            }
            i->transform->mMatrix.setPosition(pos.x, pos.y + i->mesh->sphereRadius/3, pos.z);
        }

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
    mEditorCamera->mProjectionMatrix.perspective(frustum->FOV, mAspectRatio, frustum->nearPlane, frustum->farPlane);
    mGameCamera->mProjectionMatrix.perspective(frustum->FOV, mAspectRatio, frustum->nearPlane, frustum->farPlane);
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
