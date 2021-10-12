#include "coreengine.h"

#include "resourcemanager.h"
#include "soundsystem.h"

#include "gameobject.h"
#include "rendersystem.h"
#include "camera.h"
#include "mainwindow.h"
#include "logger.h"

#include <ctime>

CoreEngine* CoreEngine::mInstance = nullptr;    //static pointer to instance

CoreEngine::CoreEngine(RenderSystem *renderSystemIn, MainWindow *mainWindowIn)
    : mRenderSystem{renderSystemIn}, mMainWindow{mainWindowIn}
{
    //Make logger
    mLogger = Logger::getInstance();
    //feed in MainWindow - have to be done, else logger will crash program
    mLogger->setMainWindow(mMainWindow);
    // current date/time based on current system
    time_t now = time(0);
    // convert now to string form
    char* dt = ctime(&now);
    mLogger->logText("Logger started " + std::string(dt), LColor::HIGHLIGHT);

    mResourceManager = &ResourceManager::getInstance();
    mSoundSystem = SoundSystem::getInstance();
    mInstance = this;

    //Make the gameloop timer:
    mGameLoopTimer = new QTimer(this);

    //Make EditorCamera:
    mEditorCamera = new Camera(45.f, 0.1f, 1000.f);
    mGameCamera = new Camera(45.f, 0.1f, 300.f);
}

CoreEngine *CoreEngine::getInstance()
{
    return mInstance;
}

void CoreEngine::togglePlayMode(bool shouldPlay)
{
    isPlaying = shouldPlay;
    SoundSystem::getInstance()->togglePlaySounds(isPlaying);
    mRenderSystem->isPlaying = isPlaying;
}

void CoreEngine::setUpScene()
{
    //ask resource manager to set up all assets
    mResourceManager->setUpAllTextures();
    mResourceManager->setUpAllSounds();
    mResourceManager->setUpAllMeshes();

    //********************** Making the object to be drawn **********************

    //Axis
    GameObject *temp = mResourceManager->addObject("axis");
    temp->mName = "Axis";
    mRenderSystem->mGameObjects.push_back(temp);

    //dog triangle
    temp = mResourceManager->addObject("triangle");
    temp->mName = "DogTriangle";
    temp->mMaterial->mShaderProgram = 1;
    temp->mMaterial->mTextureUnit = 1; //mResourceManager->getTextureID()->;

    temp->mTransform->mMatrix.translate(-2.f, -2.f, .5f);
    //Adds sound to moving triangle:
    mResourceManager->addComponent("caravan_mono.wav", temp);

    //Hack to test sound system
    if(temp->mSoundComponent)
        temp->mSoundComponent->shouldPlay = true;

    mRenderSystem->mGameObjects.push_back(temp);

    //Suzannes:
    for(int i{0}; i < 100; i++)
    {
        for(int j{0}; j < 10; j++)
        {
            temp = mResourceManager->addObject("suzanne.obj");
            temp->mTransform->mMatrix.translate(1.f*i, 0.f, -2.f*j);
            //TODO: Scaling have to be made easier and more automatic than this!
            temp->mTransform->mMatrix.scale(0.3f);
            temp->mMesh->mColliderRadius *= 0.3f;   //this should be done automatically
            temp->mTransform->mScale.setAlltoSame(0.3f);
            temp->mName = "monkey " + std::to_string((i*10)+j+1);
            mRenderSystem->mGameObjects.push_back(temp);
        }
    }

//    temp = mResourceManager->addObject("suzanne.obj");
//    temp->mTransform->mMatrix.translate(0.f, 0.f, 0.f);
//    temp->mTransform->mMatrix.scale(0.5f);
//    mRenderSystem->mGameObjects.push_back(temp);

    mEditorCamera->mPosition = gsl::Vector3D(30.f, 4.f, 4.f);
//    mEditorCamera->pitch(-20);
    mRenderSystem->mEditorCamera = mEditorCamera;

    mGameCamera->mPosition = gsl::Vector3D(30.f, 1.f, 0.f);
    mRenderSystem->mGameCamera = mGameCamera;

    //update SceneOutliner to show all objects:
    mMainWindow->updateOutliner(mRenderSystem->mGameObjects);

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

    //move camera to selected object
    if(mInput.F && mRenderSystem->mIndexToPickedObject >-1)
    {
        int tempIndex = mRenderSystem->mIndexToPickedObject;
        mEditorCamera->mPosition = mRenderSystem->mGameObjects.at(tempIndex)->mTransform->mMatrix.getPosition();
        //dynamic distance to object
        mEditorCamera->mPosition.z += (mRenderSystem->mGameObjects.at(tempIndex)->mMesh->mColliderRadius)*7;
        //dynamic height placement
        mEditorCamera->mPosition.y += (mRenderSystem->mGameObjects.at(tempIndex)->mMesh->mColliderRadius*4);
        mEditorCamera->mPitch = -30.f;
        mEditorCamera->mYaw = 0.f;
        mEditorCamera->update();
        mInput.F = false;
    }

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
//    SoundSystem::getInstance()->update(mRenderSystem);

    mRenderSystem->render();
}

