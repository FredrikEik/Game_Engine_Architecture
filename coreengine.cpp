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


    //readLasFile = HUGELY EXPENSIVE FUNCTION - TO BE USED WITH VIS & SIM
    // https://www.tutorialcup.com/cplusplus/multithreading.htm#Initializing_thread_with_an_object
    // https://stackoverflow.com/questions/10673585/start-thread-with-member-function
    MeshHandler HeightMapMeshHandler;
//    std::thread t1(&MeshHandler::HeightMap, HeightMap.readLasFile());
//    t1(HeightMap.readLasFile());
//    t1.detach();

    //Get the data from the las file.
    MeshData HeightMesh = HeightMapMeshHandler.readLasFile();

    //Create a new mesh for the Las ground
    int meshIndex = HeightMapMeshHandler.makeMesh("LasGroundMesh");
    MeshComponent* currentMesh = new MeshComponent();

    //Fill the mesh with relevant data from heightMesh
    std::copy(HeightMapMeshHandler.mMeshes.at(meshIndex).mVAO,
              HeightMapMeshHandler.mMeshes.at(meshIndex).mVAO+3, currentMesh->mVAO);

    std::copy(HeightMapMeshHandler.mMeshes.at(meshIndex).mVertexCount,
              HeightMapMeshHandler.mMeshes.at(meshIndex).mVertexCount+3, currentMesh->mVertexCount);

    std::copy(HeightMapMeshHandler.mMeshes.at(meshIndex).mIndexCount,
              HeightMapMeshHandler.mMeshes.at(meshIndex).mIndexCount+3, currentMesh->mIndexCount);

//    glPointSize(5.0f)
    currentMesh->mDrawType = {GL_POINTS};
    currentMesh->mColliderRadius = 0.0f;

    //Create the gameobject LasGround
    GameObject *LasGround = mGameObjectManager->addObject("LasGround");
    LasGround->mMesh = currentMesh;
    LasGround->mName = "LasGround";
    LasGround->mTransform->mMatrix.translate(0.0f, 0.0f, 0.0f);
    mRenderSystem->mGameObjects.push_back(LasGround);

//---------------------End of Vis & Sim code----------------------------------------

    //Axis
    temp = mGameObjectManager->addObject("axis");
    temp->mName = "Axis";
    mRenderSystem->mGameObjects.push_back(temp);

    //dog triangle
    temp = mGameObjectManager->addObject("triangle");
    temp->mName = "DogTriangle";
    temp->mMaterial = mGameObjectManager->getMaterial("Texture");

    temp->mTransform->mMatrix.translate(-2.f, -2.f, .5f);
    //Adds sound to moving triangle:
    mGameObjectManager->addComponent("caravan_mono.wav", temp);

//    //groundObject
//    temp = mGameObjectManager->addObject("Ground");
//    temp->mName = "Ground";
//    mRenderSystem->mGameObjects.push_back(temp);

    //Hack to test sound system
    if(temp->mSoundComponent)
        temp->mSoundComponent->shouldPlay = true;

    mRenderSystem->mGameObjects.push_back(temp);

    //Suzannes:
//    for(int i{0}; i < 10; i++)
//    {
//        for(int j{0}; j < 1; j++)
//        {
//            temp = mGameObjectManager->addObject("suzanne.obj");
//            temp->mTransform->mMatrix.translate(3.f*i, -4.f, -3.f*j);
//            temp->mTransform->mMatrix.scale(0.5f);
//            temp->mMesh->mColliderRadius *= 0.3f;   //this should be done automatically
//            temp->mTransform->mScale.setAlltoSame(0.5f);
//            temp->mName = "Monkey " + std::to_string(i+j);
//            mRenderSystem->mGameObjects.push_back(temp);
//        }
//    }

//    temp = mGameObjectManager->addObject("suzanne.obj");
//    temp->mTransform->mMatrix.translate(0.f, 0.f, 0.f);
//    temp->mTransform->mMatrix.scale(0.5f);
//    mRenderSystem->mGameObjects.push_back(temp);

//Rollingball, getting vis & sim collision code integrated into GEA
    temp = mGameObjectManager->addObject("Ball.obj");
    temp->mTransform->mMatrix.translate(0.5f, 1.5f, 0.0f);
    temp->mTransform->mMatrix.scale(0.1f);
    temp->mName = "RollingBall";
    mGameObjectManager->addComponent("PhysicsComponent", temp);
    mRenderSystem->mGameObjects.push_back(temp);

    temp = mGameObjectManager->addObject("CrookedTriangleSurface.obj");
    temp->mTransform->mMatrix.translate(0.0f, 0.0f, 0.0f);
    temp->mTransform->mMatrix.scale(1.0f);
    temp->mTransform->mMatrix.rotateY(0); //If rotation is applied, vertices does not match their actual locations in world
    temp->mName = "CrookedTriangleSurface";
    mRenderSystem->mGameObjects.push_back(temp);

    //mEditorCamera = new Camera();
    mEditorCamera->mPosition = gsl::Vector3D(0.0f, 1.0f, 10.0f);
    mRenderSystem->mEditorCamera = mEditorCamera;

    //mGameObjectManager->setUpAllTextures();

    mGameCamera->mPosition = gsl::Vector3D(0.0f, 1.0f, 10.0f);
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

    //Update PhysicsObjects in scene
    std::vector<GameObject*> mGameObjects = mRenderSystem->getAllGameObjects();
    PhysicsHandler ph(mRenderSystem);
    ph.movePhysicsObject(mGameObjects);

//    //Initializing values for the gameplay
//    GamePlayMechanics tm(mRenderSystem);
//    int tetrominoNr;
////    std::vector<GameObject*> gameBlock;

//    while(mGameObjects.size() < 40) //this line never s
//    {
////        qDebug() << "There are" << mGameObjects.size() << "objects in the scene";
//        tetrominoNr = tm.GetTetromino(); //Returning a random number between 1 & 7, used to pick which tetromino.

//        //This line compiles, but mGameObjects does not expand with the push backs from inside the tetrominomaker function
//        mGameObjects = tm.TetrominoMaker(tetrominoNr); //Returns four tetrominos as four gameobjects to be used in the scene.

//        //With these lines i get convert-type errors. c2440 From '_Ty' to '_objty'
////        mGameObjects.emplace_back(tm.TetrominoMaker(tetrominoNr));
////        mRenderSystem->mGameObjects.emplace_back(tm.TetrominoMaker(tetrominoNr));

//        //If there is a tetromino in scene, let player rotate and drop tetromino
//        for (int x = 0; x > mGameObjects.size(); x++)
//        {
//            for (int i = 0; i > 4; i++) //Need to loop through the four gameBlock vectors containing one square each
//            {
//                if(mGameObjects[x]->mName == "Tetromino")
//                {
//                    qDebug() << "Gameobject and subsecquent tetrominoes found";
//                    qDebug() << QString::fromStdString(mGameObjects[i]->mName);
//                }
//            }
//        }
//    }
    //If there is not a tetromino in scene, draw another one.

    mRenderSystem->render();
}
