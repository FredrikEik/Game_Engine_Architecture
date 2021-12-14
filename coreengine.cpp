#include "coreengine.h"
#include "gameobjectmanager.h"
#include "soundsystem.h"
#include "gameobject.h"
#include "rendersystem.h"
#include "camera.h"
#include "mainwindow.h"
#include "physicshandler.h"
#include "gameplaymechanics.h"
#include "meshhandler.h"

#include <thread>

//for JavaScript functionality
#include <QtQml>
#include <QJSEngine>
#include <QFile>
#include <QDebug>

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
    mMeshHandler = &MeshHandler::getInstance();

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

void CoreEngine::togglePhysicsPlay(bool shouldSimulate)
{
    isSimulatingPhysics = shouldSimulate;
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


    //Read a variable value from the script file
    QJSValue mString = engine.evaluate("myVariable");
    qDebug() << mString.toString();

}

void CoreEngine::setUpScene()
{   
    //ask resource manager to set up all asstes
    mGameObjectManager->setUpAllSounds();
    mGameObjectManager->setUpAllMeshes();
    mGameObjectManager->setUpAllTextures();
    mGameObjectManager->setUpAllShaders();
    mGameObjectManager->setUpAllMaterials();

    //First GameObject to be added is not drawn, making it an empty one.
    GameObject *temp = mGameObjectManager->addObject(("EmptyGameObject"));
    temp->mName = "EmptyGameObject";
    mRenderSystem->mGameObjects.push_back(temp);


    //********************** Making the objects to be drawn *********************

    //Vis & Sim code
    {
    //readLasFile = HUGELY EXPENSIVE FUNCTION - TO BE USED WITH VIS & SIM
    //Create the gameobject LasGround
//    GameObject *lasGround = mGameObjectManager->addObject("Data"); //Via a couple of jumps, eventually calles the hugely expensive "readLasFile" function in MeshHandler
//    lasGround->mName = "LasGround";
//    lasGround->mTransform->mMatrix.translate(0.0f, 0.0f, 0.0f);
//    mRenderSystem->mGameObjects.push_back(lasGround);


    //615197.0, 6758309.0
//    temp = mGameObjectManager->addObject("Ball.obj");
//    temp->mTransform->mMatrix.translate((615197.0f - 615181.0625f), 5.0f, (6758309.0f - 6758294)); //Ball position minus xMin and zMin to see simulation
//    temp->mTransform->mMatrix.scale(0.5f);
//    temp->mName = "RulleBall";
//    mGameObjectManager->addComponent("PhysicsComponent", temp);
//    mRenderSystem->mGameObjects.push_back(temp);

    //Rollingball, vis & sim collision code integrated into GEA

//    //Create many balls to "simulate" rain.
//    for(int i = 0; i <= numberOfSimulatedBalls; i++)
//    {
//        temp = mGameObjectManager->addObject("Ball.obj");
//        //Place the balls randomly between these points.
//        temp->mTransform->mMatrix.translate(QRandomGenerator::global()->bounded(10, 101), //Include first value, exlude second value
//                                            QRandomGenerator::global()->bounded(15,  31),
//                                            QRandomGenerator::global()->bounded(10, 101));
//        temp->mTransform->mMatrix.scale(1.0f);
//        temp->mName = "RollingBall_" + std::to_string(i);
//        mGameObjectManager->addComponent("PhysicsComponent", temp);
//        mRenderSystem->mGameObjects.push_back(temp);
//    }

//    temp = mGameObjectManager->addObject("ball.obj");
//    temp->mTransform->mMatrix.translate(0.5f, 2.0f, -0.25f);
//    temp->mTransform->mMatrix.scale(0.15f);
//    temp->mName = "RulleBall";
//    mRenderSystem->mGameObjects.push_back(temp);

//    temp = mGameObjectManager->addObject("CrookedTriangleSurface.obj");
//    temp->mTransform->mMatrix.translate(0.0f, 0.0f, 0.0f);
//    temp->mTransform->mMatrix.scale(1.0f);
//    temp->mTransform->mMatrix.rotateY(0);
//    temp->mName = "CrookedTriangleSurface";
//    mRenderSystem->mGameObjects.push_back(temp);
    }

    //Axis
    temp = mGameObjectManager->addObject("axis");
    temp->mName = "Axis";
    mRenderSystem->mGameObjects.push_back(temp);

    //Ground
    temp = mGameObjectManager->addObject("Plane5x5.obj");
    temp->mName = "Ground";
    temp->mMaterial = mGameObjectManager->getMaterial("Phong");
    mRenderSystem->mGameObjects.push_back(temp);

    //Point Light
    temp = mGameObjectManager->addObject("EmptyGameObject");
    temp->mName = "PointLight";
    temp->mLightComponent = new PointLightComponent;
    temp->mTransform->mMatrix.translate(0.0f, 5.0f, 0.0f);
    mRenderSystem->mGameObjects.push_back(temp);

    for(auto gob : mRenderSystem->mGameObjects)
    {
        if(gob->mLightComponent)
        {
            mRenderSystem->mLight = gob->mLightComponent;
            mRenderSystem->mLightPosition = gob->mTransform;
        }
    }

    //Dog triangle
    temp = mGameObjectManager->addObject("triangle");
    temp->mName = "DogTriangle";
    temp->mMaterial = mGameObjectManager->getMaterial("Test");
    temp->mTransform->mMatrix.translate(0.0f, 2.0f, 0.0f);

    mGameObjectManager->addComponent("caravan_mono.wav", temp); //Adds sound to triangle:
    if(temp->mSoundComponent) //Hack to test sound system
        temp->mSoundComponent->shouldPlay = true;
    mRenderSystem->mGameObjects.push_back(temp);

    //Skybox
    temp = mGameObjectManager->addObject("CubeInverted.obj");
    temp->mName = "Skybox";
    temp->mMaterial = mGameObjectManager->getMaterial("skybox"); // Skybox-shader
    temp->mTransform->mMatrix.scale(30);
    mRenderSystem->mGameObjects.push_back(temp);


    //Suzannes:
    for(int i = 0; i <= 5; i++)
    {
        for(int j = 0; j < 2; j++)
        {
            temp = mGameObjectManager->addObject("suzanne.obj");
            temp->mTransform->mMatrix.translate((1.0f * (i * 2)), 0.0f, (-1.0f * (j* 2)));
            temp->mTransform->mMatrix.scale(0.5f);
            temp->mMesh->mColliderRadius *= 0.3f;
            temp->mTransform->mScale.setAlltoSame(0.5f);
            temp->mName = "Monkey " + std::to_string(i) + std::to_string(j);
            mRenderSystem->mGameObjects.push_back(temp);
        }
    }

//    temp = mGameObjectManager->addObject("suzanne.obj");
//    temp->mTransform->mMatrix.translate(0.f, 0.f, 0.f);
//    temp->mTransform->mMatrix.scale(0.5f);
//    mRenderSystem->mGameObjects.push_back(temp);

    //mEditorCamera = new Camera();
    mEditorCamera->mPosition = gsl::Vector3D(0.0f, 10.0f, 20.0f);
//    mEditorCamera->mForward = gsl::Vector3D(90.0f, 0.0f, 0.0f);
//    mEditorCamera->mYaw = 180.0f;
    mEditorCamera->mPitch = -25.0f;
    mRenderSystem->mEditorCamera = mEditorCamera;

    //mGameObjectManager->setUpAllTextures();

    mGameCamera->mPosition = gsl::Vector3D(0.0f, 10.0f, 20.0f);
//    mGameCamera->mYaw = 180.0f;
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
            mEditorCamera->updateheight(-mEditorCamera->mCameraSpeed);
        if(mInput.E)
            mEditorCamera->updateheight(mEditorCamera->mCameraSpeed);
    }
}

void CoreEngine::gameLoop()
{
    //Keyboard / mouse input
    handleInput();

    mEditorCamera->update();
    //SoundSystem::getInstance()->update(mRenderSystem);

    //Vis & Sim Code
    {
//    std::vector<GameObject*> mGameObjects = mRenderSystem->getAllGameObjects();
//    PhysicsHandler ph;

//    ph.movePhysicsObject(mGameObjects, isSimulatingPhysics, numberOfSimulatedBalls);
    }

    std::vector<GameObject*> mGameObjects = mRenderSystem->getAllGameObjects();

    //Initializing values for the gameplay
    GamePlayMechanics tm(mRenderSystem);
    int tetrominoNr;

//    while(mGameObjects.size() < 40)
//    {
//        qDebug() << "There are" << mGameObjects.size() << "objects in the scene";
//        tetrominoNr = tm.GetTetromino(); //Returning a random number between 1 & 7, used to pick which tetromino.

//        tm.TetrominoMaker(tetrominoNr); //Returns the tetromino to be used in the scene.

//        //If there is a tetromino in scene, let player rotate and drop tetromino
//        for (int x = 0; x > mGameObjects.size(); x++)
//        {
//            for (int i = 0; i > 4; i++) //Need to loop through the four gameBlock vectors containing one square each
//            {
//                if(mGameObjects[x]->mName == "Tetromino")
//                {
//                    qDebug() << "Gameobject and tetromino found";
//                    qDebug() << QString::fromStdString(mGameObjects[i]->mName);
//                }
//            }
//        }
//    }
    //If there is not a tetromino in scene, draw another one.

    mRenderSystem->render();
}
