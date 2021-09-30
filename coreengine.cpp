#include "coreengine.h"

#include "resourcemanager.h"
#include "soundsystem.h"

#include "gameobject.h"
#include "rendersystem.h"
#include "camera.h"

CoreEngine* CoreEngine::mInstance = nullptr;    //static pointer to instance

CoreEngine::CoreEngine(RenderSystem *renderSystemIn) : mRenderSystem{renderSystemIn}
{
    mResourceManager = &ResourceManager::getInstance();
    mSoundSystem = SoundSystem::getInstance();
    mInstance = this;

    //Make the gameloop timer:
    mGameLoopTimer = new QTimer(this);
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
    //********************** Making the object to be drawn **********************

    //Axis
    GameObject *temp = mResourceManager->addObject("axis");
    mRenderSystem->mGameObjects.push_back(temp);

    //dog triangle
    player = mResourceManager->addObject("suzanne.obj");
    player->mMaterial->mShaderProgram = 0;
    player->mMaterial->mTextureUnit = 0; //mResourceManager->getTextureID()->;
    player->mTransform->mMatrix.rotateY(180.f);

    player->mTransform->mMatrix.translate(-2.f, -2.f, .5f);
    //Adds sound to player:
    //mResourceManager->addComponent("fin_sang_stereo.wav", temp);
    mResourceManager->addComponent("techno_stereo.wav", player);
    //Hack to test sound system
    player->mSoundComponent->shouldPlay = true;

    mRenderSystem->mGameObjects.push_back(player);

    //lager test objekter
    for(int i{0}; i < 3; i++)
    {
        for(int j{0}; j < 3; j++)
        {
              temp = mResourceManager->addObject("suzanne3.obj");
              temp->mTransform->mMatrix.translate(-2, 1.f,2.f);
              temp->mMaterial->mShaderProgram = 1;
              temp->mMaterial->mTextureUnit = 2; //mResourceManager->getTextureID()->;
              temp->mTransform->mMatrix.rotateY(180.f);
              temp->mTransform->mMatrix.translate(-2.f*i, 1.f, 2.f*j);
              temp->mTransform->mMatrix.scale(0.5f);
              mRenderSystem->mGameObjects.push_back(temp);
        }
    }

//    temp = mResourceManager->addObject("suzanne.obj");
//    temp->mTransform->mMatrix.translate(0.f, 1.f, 0.f);
//    temp->mTransform->mMatrix.scale(.5f);

//    mRenderSystem->mGameObjects.push_back(temp);

    mEditorCamera = new Camera();
    mEditorCamera->mPosition = gsl::Vector3D(1.f, .5f, 4.f);
    mRenderSystem->mCurrentCamera = mEditorCamera;

    mResourceManager->setUpAllTextures();

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
    float speed = 0.05f;
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
    else if(mInput.W)
    {

         player->move(0,0 , speed);
    }
    else if(mInput.S)
    {

         player->move(0,0 , -speed);
    }
     else if(mInput.A)
    {

         player->move(speed, 0, 0);
    }
     else if(mInput.D)
    {

         player->move(-speed, 0, 0);
    }
    else if(mInput.Q)
        player->move(0, speed, 0);
    else if(mInput.E)
        player->move(0, -speed, 0);

}

void CoreEngine::gameLoop()
{
    //Keyboard / mouse input
    handleInput();

    mEditorCamera->update();
    SoundSystem::getInstance()->update(mRenderSystem);

    mRenderSystem->render();
}

