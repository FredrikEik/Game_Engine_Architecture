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
    mRenderWindow->addToGameObjects(playerObject); // pos = 57.f, 8.5f, 118

    gsl::Vector3D gameCameraPos{57, 10, 120};
    gsl::Vector3D editorCameraPos{25, 10, 90};

    mGameCameraMesh = mResourceManager->CreateObject("camera.obj");
    mGameCameraMesh->transform->mMatrix.translate(gameCameraPos);
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

/** Terrain from txt file */
    temp = mResourceManager->CreateObject("test_las.txt");
    temp->transform->mMatrix.scale(0.1f);
    mRenderWindow->addToGameObjects(temp);

/** Høydekurver (contour-lines) */
//    temp = mResourceManager->CreateObject("ContourLines");
//    temp->transform->mMatrix.scale(0.1f);
//    temp->material->mShaderProgram = 0;
//    temp->mesh->mDrawType = GL_LINES;
//    mRenderWindow->addToGameObjects(temp);

/** Spawn grass */
    float y = 0;
    float b = 0;
    for(int i = 1; i < 99; i+= 2)
    {
        for(int j = 1; j < 99; j+=2)
        {
           b = floor(rand() % 360);
           temp = mResourceManager->CreateObject("grass.obj");
           y = mResourceManager->getHeightMapHeight(gsl::Vector2D{float(i), float(j)});
           temp->transform->mMatrix.translate(i, y-0.2f, j*mResourceManager->scale);
           temp->transform->mMatrix.rotateY(b);
           temp->material->mShaderProgram = gsl::TEXTURESHADER;
           temp->material->mTextureUnit = 2;
           mRenderWindow->addToGameObjects(temp);
        }
    }

//********************** Set up cameras **********************
    mGameCamera->setPosition(gsl::Vector3D(gameCameraPos));

    mEditorCamera->setPosition(gsl::Vector3D(editorCameraPos));
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
// Player
    mRenderWindow->setPlayerMovement(move.x, move.y, move.z); //Movement happens later, this just decides where we move.
// GameCamera
    mGameCamera->setPosition(mGameCamera->position() + gsl::Vector3D(move.x/5, move.y/5, move.z/5));
// GameCamera - Mesh
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
            int index = mRenderWindow->getIndexToPickedObject();
            if(index == -1)
            {
                i->transform->mMatrix.setPosition(9999, 9999, 9999);
                return;
            }
            i->transform->mMatrix.setPosition(pos.x, pos.y + mRenderWindow->getGameObjects()[index]->mesh->sphereRadius/3, pos.z);

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
