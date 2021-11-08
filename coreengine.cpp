#include "coreengine.h"
#include "gameobjectmanager.h"
#include "soundsystem.h"
#include "gameobject.h"
#include "rendersystem.h"
#include "camera.h"
#include "mainwindow.h"
#include "physicshandler.h"

//for JavaScript functionality
#include <QtQml>
#include <QJSEngine>
#include <QFile>

CoreEngine* CoreEngine::mInstance = nullptr;    //static pointer to instance

CoreEngine::CoreEngine(RenderSystem *renderSystemIn, MainWindow *mainWindowIn)
                    : mRenderSystem{renderSystemIn}, mMainWindow{mainWindowIn}
{
    //Create and send logger to main
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

    //Make the cameras for the scene
    mEditorCamera = new Camera(30.0f, 0.1f, 1000.f);
    mEditorCamera->mName = "Editor";
    mGameCamera = new Camera(30.0f, 0.1f, 300.f);
    mGameCamera->mName = "Game";

    CoreEngine::readJSScene();
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

void CoreEngine::readJSScene()
{
    //Create script engine
    QJSEngine engine;

    //Make variable to this path
    QString fileName = "../GEA2021/SceneScript.js";

    //Make a QFile for the variable
    QFile scriptFile(fileName);

    //Try to open file and give error if something is wrong
    if (!scriptFile.open(QIODevice::ReadOnly))
        qDebug() << "Error - No file here: " << fileName;

    //Read the file
    QTextStream stream(&scriptFile);
    QString contents = stream.readAll();

    //Close the file
    scriptFile.close();

    //Load the script into script engine, filename is used to report bugs in the file
    engine.evaluate(contents, fileName);
}

void CoreEngine::setUpScene()
{
    //ask resource manager to set up all asstes
    mGameObjectManager->setUpAllSounds();
    mGameObjectManager->setUpAllMeshes();
    mGameObjectManager->setUpAllTextures();
    mGameObjectManager->setUpAllShaders();
    mGameObjectManager->setUpAllMaterials();
    //********************** Making the object to be drawn **********************

    //Axis
    GameObject *temp = mGameObjectManager->addObject("axis");
    temp->mName = "Axis";
    mRenderSystem->mGameObjects.push_back(temp);

    //dog triangle
    temp = mGameObjectManager->addObject("triangle");
    temp->mName = "DogTriangle";
    temp->mMaterial = mGameObjectManager->getMaterial("Texture");

    temp->mTransform->mMatrix.translate(-2.f, -2.f, .5f);
    //Adds sound to moving triangle:
    mGameObjectManager->addComponent("caravan_mono.wav", temp);

    //Hack to test sound system
    if(temp->mSoundComponent)
        temp->mSoundComponent->shouldPlay = true;

    mRenderSystem->mGameObjects.push_back(temp);

    //Suzannes:
    for(int i{-50}; i < 100; i++)
    {
        for(int j{0}; j < 10; j++)
        {
            temp = mGameObjectManager->addObject("suzanne.obj");
            temp->mTransform->mMatrix.translate(3.f*i, -4.f, -3.f*j);
            temp->mTransform->mMatrix.scale(0.5f);
            temp->mMesh->mColliderRadius *= 0.3f;   //this should be done automatically
            temp->mTransform->mScale.setAlltoSame(0.5f);
            temp->mName = "Monkey " + std::to_string((i*10)+j+1);
            mRenderSystem->mGameObjects.push_back(temp);
        }
    }

//    temp = mGameObjectManager->addObject("suzanne.obj");
//    temp->mTransform->mMatrix.translate(0.f, 0.f, 0.f);
//    temp->mTransform->mMatrix.scale(0.5f);
//    mRenderSystem->mGameObjects.push_back(temp);

//Rollingball, getting vis & sim collision code integrated into GEA
        temp = mGameObjectManager->addObject("Ball.obj");
        temp->mTransform->mMatrix.translate(0.0f, 0.0f, -10.0f);
        temp->mTransform->mMatrix.scale(0.5f);
        temp->mName = "RollingBall";
        mGameObjectManager->addComponent("PhysicsComponent", temp);
        mRenderSystem->mGameObjects.push_back(temp);

        temp = mGameObjectManager->addObject("TriangleSurface.obj");
        temp->mTransform->mMatrix.translate(1.5f, -2.0f, -9.0f);
        temp->mTransform->mMatrix.scale(3.0f);
        temp->mTransform->mMatrix.rotateY(90);
        temp->mName = "TriangleSurface";
        mRenderSystem->mGameObjects.push_back(temp);

    //mEditorCamera = new Camera();
    mEditorCamera->mPosition = gsl::Vector3D(0.0f, 0.0f, 20.0f);
    mRenderSystem->mEditorCamera = mEditorCamera;

    //mGameObjectManager->setUpAllTextures();

    mGameCamera->mPosition = gsl::Vector3D(0.0f, 0.0f, 20.0f);
    mRenderSystem->mGameCamera = mGameCamera;

    //Updates the hierarchy to show objects in it:
    mMainWindow->updateHierarchy(mRenderSystem->mGameObjects);

    //Connect the gameloop timer to the render function:
    //This makes our render loop
    connect(mGameLoopTimer, SIGNAL(timeout()), this, SLOT(gameLoop()));
    //This timer runs the actual MainLoop
    //16 means 16ms = 60 Frames pr second (should be 16.6666666 to be exact...)
    mGameLoopTimer->start(7); //Edited to 7 ms, this gives an fps of about 143fps
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
    //SoundSystem::getInstance()->update(mRenderSystem);

    //Update PhysicsObjects in scene
    std::vector<GameObject*> mGameObjects = mRenderSystem->getAllGameObjects();
    PhysicsHandler ph(mRenderSystem);
    ph.movePhysicsObject(mGameObjects);

    mRenderSystem->render();
}
