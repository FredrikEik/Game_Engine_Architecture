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

    mGameCamera->pitch(20); //tilt the camera down 20 degrees
}
/**
 * GetInstance returns a pointer to CoreEngine which can be used in other classes to
 * call/use public functions and variables from CoreEngine.
 */
CoreEngine *CoreEngine::getInstance()
{
    return mInstance;
}

/**
 * CoreEngine::GameLoop Is the gameLoop itself.
 * Everything that should be called/updated per frame should be called here.
 */
void CoreEngine::GameLoop()
{
    HandleInput();

    mRenderWindow->getCurrentCamera()->update();

    mRenderWindow->render();
}

/**
 * CoreEngine::SetUpScene Is the function that sets up all objects in the scene.
 * The objects themselves are made in ResourceManager, but here we can decide position,
 * rotation, scale, what shaderProgram to use, what texture or sound to use etc..
 */
void CoreEngine::SetUpScene()
{
    gsl::Vector3D gameCameraPos{57, 10, 120};
    gsl::Vector3D editorCameraPos{25, 10, 90};

/* Player */
    GameObject *temp = mResourceManager->CreateMainCharacter("cube.obj");
    playerObject = temp;
    temp->transform->mMatrix.scale(0.2f);
    temp->mesh->collisionsEnabled = false;
    mRenderWindow->addToGameObjects(playerObject);

/* Skybox */
    temp = mResourceManager->CreateObject("skyboxCube.obj");
    temp->transform->mMatrix.scale(1000);
    temp->material->mShaderProgram = gsl::SKYBOXSHADER;
    temp->material->mTextureUnit = 3;
    temp->mesh->collisionsEnabled = false;
    mRenderWindow->addToGameObjects(temp);

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

/* Terrain from txt file */
    temp = mResourceManager->CreateObject("test_las.txt");
    temp->transform->mMatrix.scale(0.1f);
    mRenderWindow->addToGameObjects(temp);

/* Høydekurver (contour-lines) */
//    temp = mResourceManager->CreateObject("ContourLines");
//    temp->transform->mMatrix.scale(0.1f);
//    temp->material->mShaderProgram = 0;
//    temp->mesh->mDrawType = GL_LINES;
//    mRenderWindow->addToGameObjects(temp);

/* Spawn grass */
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

/* Set Camera Positions / set starting camera */
    mGameCamera->setPosition(gsl::Vector3D(gameCameraPos));
    mEditorCamera->setPosition(gsl::Vector3D(editorCameraPos));
    mRenderWindow->setToCurrentCamera(mEditorCamera);

/* Initialize sounds */
    mStereoSound = SoundManager::getInstance()->createSource("Stereo", gsl::Vector3D(0.0f, 0.0f, 0.0f), "..\\GEA2021\\Assets\\Sounds\\stereo.wav", false, 0.2f);
    mGunShot = SoundManager::getInstance()->createSource("Gunshot", gsl::Vector3D(0.0f, 0.0f, 0.0f),     "..\\GEA2021\\Assets\\Sounds\\gunshot.wav", false, 0.2f);
    mReloadGun = SoundManager::getInstance()->createSource("ReloadGun", gsl::Vector3D(0.0f, 0.0f, 0.0f),     "..\\GEA2021\\Assets\\Sounds\\reload.wav", false, 0.2f);

}

/**
 * CoreEngine::resetScene Deletes all objects from the gameObjects vector,
 * and calls SetUpScene to reset the scene back to default.
 */
void CoreEngine::resetScene()
{
    std::vector<GameObject*>* gameObjects = mRenderWindow->getGameObjectsPtr();

    gameObjects->clear(); //Removes and deletes all GameObjects

    SetUpScene(); //Scene setup: adds objects back in, with default parameters (e.g. Position reset)
}

/**
 * CoreEngine::startGameLoopTimer Starts the gameloop timer, and here you can
 * decide how often the loop should be updated. (How long between each frame).
 */
void CoreEngine::startGameLoopTimer()
{
    //Connect the gameloop timer to the render function:
    connect(mGameLoopTimer, SIGNAL(timeout()), this, SLOT(GameLoop()));
    //This timer runs the actual MainLoop
    //16 means 16ms = 60 Frames pr second
    mGameLoopTimer->start(16);
}

/**
 * CoreEngine::HandleInput Checks if the program is in editorMode or GameMode.
 * And decides based on that, what input function should be called.
 */
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

/**
 * CoreEngine::PlayerInput checks what buttons are currently being clicked/pressed.
 * Based on that info it sets the next positions of the playerObject, and sets the
 * next position of the gameCamera and it's mesh.
 */
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

/**
 * CoreEngine::EditorCameraInput checks what buttons are currently being clicked/pressed.
 * Based on that info it sets the speed of the EditorCamera. Which later updates the position
 * of EditorCamera based on the value from mCameraSpeed.
 */
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

/**
 * CoreEngine::CreateObjectButton is the function that gets called when the 'Create Object' button is pressed.
 * The function creates a new object and spawns it in front of the EditorCamera.
 */
void CoreEngine::CreateObjectButton(std::string objName)
{
    GameObject *temp = mResourceManager->CreateObject(objName);
    temp->transform->mMatrix.setPosition(mEditorCamera->position().x, mEditorCamera->position().y, mEditorCamera->position().z -5);
    mRenderWindow->addToGameObjects(temp);
}

/**
 * CoreEngine::MoveSelectionArrow Moves the arrow that shows the currently selected object
 * above the currently selected object. Position of curretly selected object is passd in
 * to the function as a 'gsl::Vector3D' parameter 'pos'.
 */
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

/**
 * CoreEngine::swapCurrentCamera is a function that checks what camera is currently being used,
 * and switches over to using the other camera. The program calls this function when the user clicks
 * 'Start Game' button in the UI.
 */
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

/**
 * CoreEngine::initCameraProjectionMatrixes
 * Initializes both cameras projectionMatrixes. Sets FOV, aspectRatio, nearplane, and farplane.
 * Gets some of these values from the Frustum struct in Components.h.
 */
void CoreEngine::initCameraProjectionMatrixes(float mAspectRatio)
{
    mEditorCamera->mProjectionMatrix.perspective(frustum->FOV, mAspectRatio, frustum->nearPlane, frustum->farPlane);
    mGameCamera->mProjectionMatrix.perspective(frustum->FOV, mAspectRatio, frustum->nearPlane, frustum->farPlane);
}

/**
 * CoreEngine::playStartGameSound
 * Plays 'start game' sound.
 */
void CoreEngine::playStartGameSound()
{
    mStereoSound->play();
}

/**
 * CoreEngine::shootBullet is called when the program is in 'game mode', if the player clicks the
 * left mouse button, and if there is ammo left in the clip. If no ammo is left, click 'R'.
 * The function spawns a bullet at playerObject's position, and plays a sound called 'mGunShot'.
 */
void CoreEngine::shootBullet()
{
    gsl::Vector3D playerPos = playerObject->transform->mMatrix.getPosition();

     GameObject* temp = mResourceManager->CreateObject("bullet.obj");
     temp->transform->mMatrix.setPosition(playerPos.x, playerPos.y, playerPos.z);
     temp->transform->mMatrix.scale(0.1);
     mRenderWindow->addToGameObjects(temp);

     mGunShot->play();
}

/**
 * CoreEngine::isPlaying
 * Returns a bool that decides if we are in 'game mode' or 'editor mode'.
 */
bool CoreEngine::isPlaying()
{
    return bUsingGameCamera;
}

/**
 * CoreEngine::getGameCamera
 * Returns a pointer to the game-camera.
 */
Camera *CoreEngine::getGameCamera()
{
    return mGameCamera;
}

/**
 * CoreEngine::getPlayerMatrix
 *  Returns the modelMatrix of 'playerObject' (Cube).
 */
gsl::Matrix4x4 CoreEngine::getPlayerMatrix()
{
    return playerObject->transform->mMatrix;
}
