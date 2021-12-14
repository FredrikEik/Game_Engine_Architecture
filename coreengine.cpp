#include "coreengine.h"

#include "gameobject.h"
#include "rendersystem.h"
#include "camera.h"
#include "mainwindow.h"
#include "qtimer.h"

CoreEngine* CoreEngine::mInstance = nullptr;    //static pointer to instance

CoreEngine::CoreEngine(RenderSystem *renderSystemIn, MainWindow *mainWindowIn)
    : mRenderSystem{renderSystemIn}, mMainWindow{mainWindowIn}
{
    // current date/time based on current system
    time_t now = time(0);
    // convert now to string form
    char* dt = ctime(&now);

    mInstance = this;

    //Make the gameloop timer:
    mGameLoopTimer = new QTimer(this);

    //Make EditorCamera:
    mEditorCamera = new Camera();
    mEditorCamera->setName("Editor Camera");
    mEditorCamera->pitch(10.f);
    mEditorCamera->perspective(60.f, 4.f/3.f, 0.1f, 1000.f);
    mGameCamera = new Camera();
    mGameCamera->setName("Game Camera");
}

CoreEngine *CoreEngine::getInstance()
{
    return mInstance;
}

void CoreEngine::togglePlayMode(bool shouldPlay)
{
    isPlaying = shouldPlay;
    mRenderSystem->mIsPlaying = isPlaying;
}

//void CoreEngine::setUpScene()
//{
//    //ask resource manager to set up all assets
//    mResourceManager->setUpAllSounds();
//    mResourceManager->setUpAllMeshes();
//    mResourceManager->setUpAllTextures();
//    mResourceManager->setUpAllShaders();
//    mResourceManager->setUpAllMaterials();  //materials uses textures and shaders!

//    //********************** Making the object to be drawn **********************

//    //Axis
//    GameObject *temp = mResourceManager->addObject("axis");
//    temp->mName = "Axis";
//    mRenderSystem->mGameObjects.push_back(temp);

//    //Grid
//    temp = mResourceManager->addObject("editorgrid");
//    temp->mName = "EditorGrid";
//    mRenderSystem->mGameObjects.push_back(temp);

//    //dog triangle
//    temp = mResourceManager->addObject("triangle");
//    temp->mName = "DogTriangle";
//    temp->mMaterial = mResourceManager->getMaterial("Texture");

//    temp->mTransform->mMatrix.translate(-2.f, -2.f, .5f);
//    //Adds sound to moving triangle:
//    mResourceManager->addComponent("caravan_mono.wav", temp);

//    //Hack to test sound system
//    if(temp->mSoundComponent)
//        temp->mSoundComponent->shouldPlay = true;

//    mRenderSystem->mGameObjects.push_back(temp);

//    //Suzannes - using default material:
////    for(int i{-50}; i < 100; i++)
////    {
////        for(int j{0}; j < 10; j++)
////        {
//            temp = mResourceManager->addObject("suzanne.obj");
////            temp->mTransform->mMatrix.translate(1.f*i, 0.f, -2.f*j);
//            //TODO: Scaling have to be made easier and more automatic than this!
//            temp->mTransform->mMatrix.scale(0.3f);
//            temp->mMesh->mColliderRadius *= 0.3f;   //this should be done automatically
//            temp->mTransform->mScale.setAlltoSame(0.3f);
//            temp->mName = "monkey "; // + std::to_string((i*10)+j+1);
//            temp->mMaterial = mResourceManager->getMaterial("Instanced");
//            mRenderSystem->mGameObjects.push_back(temp);
////        }
////    }

//    //Instance rendering of Susannes:
//    mRenderSystem->instancing(temp->mMesh->mVAO[0]);

////    temp = mResourceManager->addObject("suzanne.obj");
////    temp->mTransform->mMatrix.translate(0.f, 0.f, 0.f);
////    temp->mTransform->mMatrix.scale(0.5f);
////    mRenderSystem->mGameObjects.push_back(temp);

//    mEditorCamera->mPosition = gsl::Vector3D(0.f, 1.f, 4.f);
////    mEditorCamera->pitch(-20);
//    mRenderSystem->mEditorCamera = mEditorCamera;

//    mGameCamera->mPosition = gsl::Vector3D(0.f, 1.f, 0.f);
//    mRenderSystem->mGameCamera = mGameCamera;

//    //update SceneOutliner to show all objects:
//    mMainWindow->updateOutliner(mRenderSystem->mGameObjects);

//    //Connect the gameloop timer to the render function:
//    //This makes our render loop
//    connect(mGameLoopTimer, SIGNAL(timeout()), this, SLOT(gameLoop()));
//    //This timer runs the actual MainLoop
//    //16 means 16ms = 60 Frames pr second (should be 16.6666666 to be exact...)
//    mGameLoopTimer->start(20);
//}

void CoreEngine::handleInput()
{
    //Camera

    //move camera to selected object
//    if(mInput.F && mRenderSystem->mIndexToPickedObject >-1)
//    {
//        int tempIndex = mRenderSystem->mIndexToPickedObject;
//        mEditorCamera->mPosition = mRenderSystem->mGameObjects.at(tempIndex)->mTransform->mMatrix.getPosition();
//        //dynamic distance to object
//        mEditorCamera->mPosition.z += (mRenderSystem->mGameObjects.at(tempIndex)->mMesh->mColliderRadius)*7;
//        //dynamic height placement
//        mEditorCamera->mPosition.y += (mRenderSystem->mGameObjects.at(tempIndex)->mMesh->mColliderRadius*4);
//        mEditorCamera->mPitch = -30.f;
//        mEditorCamera->mYaw = 0.f;
//        mEditorCamera->update();
//        mInput.F = false;
//    }

    //Camera Movement
    if(m_input.RMB)
    {
        if(m_input.W)
            mEditorCamera->moveForward(m_cameraSpeed);
        if(m_input.S)
            mEditorCamera->moveForward(-m_cameraSpeed);
        if(m_input.A)
            mEditorCamera->moveRight(-m_cameraSpeed);
        if(m_input.D)
            mEditorCamera->moveRight(m_cameraSpeed);
        if(m_input.E)
            mEditorCamera->moveUp(m_cameraSpeed);
        if(m_input.Q)
            mEditorCamera->moveUp(-m_cameraSpeed);
    }

    if(mEditorCamera->editorCamera())
    {
        if(m_input.C && bKeyReleased)
        {
            if(!bWiremode)
            {
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                bWiremode = true;
                bKeyReleased = false;
            }
            else
            {
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                bWiremode = false;
                bKeyReleased = false;
            }
        }
    }
}

void CoreEngine::gameLoop()
{
    //Keyboard / mouse input
    handleInput();

    mEditorCamera->update();

    mRenderSystem->render();
}

