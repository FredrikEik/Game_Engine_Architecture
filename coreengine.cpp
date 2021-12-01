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

    mGameCamera->pitch(20); //tilt the camera down
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
    playerObject = temp;
    playerObject->transform->mMatrix.scale(0.2f);
    temp->mesh->collisionsEnabled = false;
    mRenderWindow->addToGameObjects(playerObject); //57.f, -1.f, 6.f

    mGameCameraMesh = mResourceManager->CreateObject("camera.obj");
    mGameCameraMesh->transform->mMatrix.translate(gsl::Vector3D(57.f, .5f, 9.f));
    mGameCameraMesh->transform->mMatrix.rotateX(20);
    mGameCameraMesh->mesh->collisionsEnabled = false;
    mRenderWindow->addToGameObjects(mGameCameraMesh);

    temp = mResourceManager->CreateObject("arrow.obj");
    temp->transform->mMatrix.translate(9999,9999,9999);
    temp->transform->mMatrix.scale(0.3f);
    temp->mesh->collisionsEnabled = false;
    mRenderWindow->addToGameObjects(temp);

//    temp = mResourceManager->CreateObject("HamarHeightMap.bmp");
//    temp->transform->mMatrix.rotateX(90);
//    temp->transform->mMatrix.scale(2.5);
//    temp->transform->mMatrix.translate(0,0,-4);
//    hmMatrix = temp->transform->mMatrix;
//    mRenderWindow->addToGameObjects(temp);

    temp = mResourceManager->CreateObject("test_las.txt");
//    temp->transform->mMatrix.rotateX(180); //er terrenget flippet?
    temp->transform->mMatrix.translate(0,0,-150);
    temp->transform->mMatrix.scale(0.1);
//    temp->material->mShaderProgram = 0;
//    hmMatrix = &temp->transform->mMatrix;
    mRenderWindow->addToGameObjects(temp);

//    temp = mResourceManager->CreateObject("ContourLines");
//    temp->transform->mMatrix.translate(0,0,-150);
//    temp->transform->mMatrix.scale(0.1);
//    temp->material->mShaderProgram = 0;
//    temp->mesh->mDrawType = GL_POINTS;
//    mRenderWindow->addToGameObjects(temp);

    temp = mResourceManager->CreateObject("ball.obj");
    mRenderWindow->addToGameObjects(temp);


    float y = 0;
    for(int i = 0; i < 40; i++)
    {
        for(int j = 0; j < 40; j++)
        {
           int b = floor(rand() % 4 + 9);
           temp = mResourceManager->CreateObject(treeNames[b]);
           y = mResourceManager->getHeightMapHeight(gsl::Vector2D{3.f*i, -3.f*j}); // HVORFOR funker det ikke!
           temp->transform->mMatrix.translate(3.f*i, y, -3.f*j);
           temp->transform->mMatrix.scale(0.4);
           mRenderWindow->addToGameObjects(temp);
        }
    }

    //********************** Set up cameras **********************
    mGameCamera->setPosition(gsl::Vector3D(57.f, .5f, 9.f));

    mEditorCamera->setPosition(gsl::Vector3D(25.f, .5f, 4.f));
    mRenderWindow->setToCurrentCamera(mEditorCamera);


    mStereoSound = SoundManager::getInstance()->createSource("Stereo", gsl::Vector3D(0.0f, 0.0f, 0.0f),
                                                            "..\\GEA2021\\Assets\\Sounds\\stereo.wav", false, 1.0f);
}

void CoreEngine::resetScene()
{
    std::vector<GameObject*>* gameObjects = mRenderWindow->getGameObjectsPtr();

    gameObjects->clear(); //Removes and deletes all GameObjects

    SetUpScene(); //Scene setup: adds objects back in, with default parameters (e.g. Position reset)
}

void CoreEngine::startGameLoopTimer()
{
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
    temp->transform->mMatrix.setPosition(mEditorCamera->position().x, mEditorCamera->position().y, mEditorCamera->position().z -30);
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

void CoreEngine::shootBullet()
{
    gsl::Vector3D playerPos = playerObject->transform->mMatrix.getPosition();

     GameObject* temp = mResourceManager->CreateObject("bullet.obj");
     temp->transform->mMatrix.setPosition(playerPos.x, playerPos.y, playerPos.z);
     temp->transform->mMatrix.scale(0.1);
     mRenderWindow->addToGameObjects(temp);
}

bool CoreEngine::isPlaying()
{
    return bUsingGameCamera;
}

Camera *CoreEngine::getGameCamera()
{
    return mGameCamera;
}

//gsl::Vector3D CoreEngine::getHeightMapPosition()
//{
//    return mResourceManager->mTerrain->transform->mMatrix.getPosition();
//}

gsl::Matrix4x4 CoreEngine::getPlayerMatrix()
{
    return playerObject->transform->mMatrix;
}

//void CoreEngine::updateTerrainPos(float x, float y, float z)
//{
//    mTerrainC->transform->mMatrix.setPosition(GLfloat(x), GLfloat(y), GLfloat(z));
//}
