#include "coreengine.h"

#include "ResourceManager/gameobjectmanager.h"
#include "soundsystem.h"
#include "gameobject.h"
#include "rendersystem.h"
#include "camera.h"
#include "mainwindow.h"

CoreEngine* CoreEngine::mInstance = nullptr;    //static pointer to instance

CoreEngine::CoreEngine(RenderSystem *renderSystemIn, MainWindow *mainWindowIn)
                    : mRenderSystem{renderSystemIn}, mMainWindow{mainWindowIn}
{
    mLogger = Logger::getInstance();


    mLogger->setMainWindow(mMainWindow);

    time_t now = time(0);

    char* dt = ctime(&now);
    mLogger->logText("Logger started " + std::string(dt), LColor::HIGHLIGHT);

    mGameObjectManager = &GameObjectManager::getInstance();
    mSoundSystem = SoundSystem::getInstance();

    mInstance = this;

    //Make the gameloop timer:
    mGameLoopTimer = new QTimer(this);


    mEditorCamera = new Camera(50.0f, 0.1f, 1000.f);
    mEditorCamera->mName = "Editor";
    mGameCamera = new Camera(50.0f, 0.1f, 300.f);
    mGameCamera->mName = "Game";
}

CoreEngine *CoreEngine::getInstance()
{
    return mInstance;
}

void CoreEngine::togglePlayMode(bool shouldPlay)
{
    isPlaying = shouldPlay;
    SoundSystem::getInstance()->togglePlaySounds(isPlaying);
    mRenderSystem->mIsPlaying = isPlaying;
}

void CoreEngine::setUpScene()
{
    //ask resource manager to set up all asstes
//    mGameObjectManager->setUpAllSounds();
//    mGameObjectManager->setUpAllMeshes();
//    mGameObjectManager->setUpAllTextures();
//    mGameObjectManager->setUpAllShaders();
//    mGameObjectManager->setUpAllMaterials();
    //********************** Making the object to be drawn **********************

    //Axis
    GameObject *temp = mGameObjectManager->addObject("axis");
    temp->mName = "Axis";
    mRenderSystem->mGameObjects.push_back(temp);

    //dog triangle
    temp = mGameObjectManager->addObject("triangle");
    temp->mName = "DogTriangle";
    //temp->mMaterial = mGameObjectManager->getMaterial("Texture");
    temp->mMaterial->mShaderProgram = 1;
    temp->mMaterial->mTextureUnit = 1; //mResourceManager->getTextureID()->;

    temp->mTransform->mMatrix.translate(-2.f, -2.f, .5f);
    //Adds sound to moving triangle:
    mGameObjectManager->addComponent("caravan_mono.wav", temp);

    //Hack to test sound system
    temp->mSoundComponent->shouldPlay = true;

    mRenderSystem->mGameObjects.push_back(temp);

    //Suzannes:
    for(int i{0}; i < 10; i++)
    {
        for(int j{0}; j < 10; j++)
        {
            temp = mGameObjectManager->addObject("suzanne.obj");
            temp->mTransform->mMatrix.translate(2.f*i, -3.f, -2.f*j);
            temp->mTransform->mMatrix.scale(0.5f);
            temp->mName = "Monkey " + std::to_string((i*10)+j+1);
            mRenderSystem->mGameObjects.push_back(temp);
        }
    }

//    temp = mGameObjectManager->addObject("suzanne.obj");
//    temp->mTransform->mMatrix.translate(0.f, 0.f, 0.f);
//    temp->mTransform->mMatrix.scale(0.5f);
//    mRenderSystem->mGameObjects.push_back(temp);

//Rollingball, getting vis & sim code integrated into GEA code
    //    temp = mGameObjectManager->addObject("Ball.obj");
    //    temp->mTransform->mMatrix.translate(0.0f, 0.0f, -10.0f);
    //    temp->mTransform->mMatrix.scale(1.0f);
    //    temp->mName = "Rullendeball";
    //    mGameObjectManager->addComponent("PhysicsComponent", temp);
    //    mRenderSystem->mGameObjects.push_back(temp);

    //    temp = mGameObjectManager->addObject("TriangleSurface.obj");
    //    temp->mTransform->mMatrix.translate(1.5f, -2.0f, -9.0f);
    //    temp->mTransform->mMatrix.scale(3.0f);
    //    temp->mTransform->mMatrix.rotateY(90);
    //    temp->mName = "TriangleSurface";
    //    mRenderSystem->mGameObjects.push_back(temp);

    //mEditorCamera = new Camera();
    mEditorCamera->mPosition = gsl::Vector3D(1.f, .5f, 4.f);
    mRenderSystem->mEditorCamera = mEditorCamera;

    mGameObjectManager->setUpAllTextures();

    mGameCamera->mPosition = gsl::Vector3D(0.0f, 1.0f, 0.0f);
    mRenderSystem->mGameCamera = mGameCamera;

    //Updates the hierarchy to show objects in it:
    mMainWindow->updateHierarchy(mRenderSystem->mGameObjects);

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
    SoundSystem::getInstance()->update(mRenderSystem);

    //Update PhysicsObjects in scene
    //PhysicsComponent pc;
    //pc.movePhysicsObject();

    mRenderSystem->render();
}
