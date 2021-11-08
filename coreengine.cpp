﻿#include "coreengine.h"

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
    //isPlaying = shouldPlay;
    if (shouldPlay == true)
    {
        SoundSystem::getInstance()->togglePlaySounds(true);
        mRenderSystem->isPlaying = true;
        isPlaying = true;
        //mRenderSystem->mCurrentCamera = mGameCamera;
    }
    else
    {
        SoundSystem::getInstance()->togglePlaySounds(false);
        isPlaying = false;
        mRenderSystem->isPlaying = false;
        mRenderSystem->mCurrentCamera = mEditorCamera;
        mEditorCamera->mPosition = gsl::Vector3D(1.f, .5f, 4.f);
    }

}

void CoreEngine::setUpScene()
{
    //********************** Making the object to be drawn **********************

    //Axis
    axis = mResourceManager->addObject("axis");
    axis->objName = "Axis";
    //scene->setupScene1();
    //scene->setupScene1();

    //PLAYER
    player = mResourceManager->addObject("suzanne.obj");
    player->mMaterial->mShaderProgram = 0;
    player->mMaterial->mTextureUnit = 0; //mResourceManager->getTextureID()->;
    player->mTransform->mMatrix.rotateY(180.f);
    player->mTransform->mMatrix.scale(0.5f);
    player->mTransform->mMatrix.translate(0.f, 0, -6);
    mResourceManager->addCollider("sphere", player);
    //Adds sound to player:
    mResourceManager->addComponent("caravan_mono.wav", player);

    //Hack to test sound system
    player->mSoundComponent->shouldPlay = true;
    player->objName = "Player";
    //mRenderSystem->mGameObjects.push_back(player);

    //ENEMY
    enemy = mResourceManager->addObject("suzanne3.obj");
    enemy->mTransform->mMatrix.translate(-2, 1.f,2.f);
    enemy->mMaterial->mShaderProgram = 1;
    enemy->mMaterial->mTextureUnit = 2;
    enemy->mTransform->mMatrix.rotateY(180.f);
    enemy->mTransform->mMatrix.translate(-2.f/**i*/, -1.f, 1.f/**j*/);
    enemy->mTransform->mMatrix.scale(0.5f);
    mResourceManager->addCollider("sphere", enemy);
    mResourceManager->addComponent("roblox_stereo.wav", enemy);
    enemy->mSoundComponent->shouldPlay = false;
    enemy->objName = "enemy";
    //mRenderSystem->mGameObjects.push_back(enemy);

    //BOSS
    boss = mResourceManager->addObject("suzanne3.obj");
    boss->mTransform->mMatrix.translate(-2, 1.f,2.f);
    boss->mMaterial->mShaderProgram = 1;
    boss->mMaterial->mTextureUnit = 2;
    boss->mTransform->mMatrix.rotateY(180.f);
    boss->mTransform->mMatrix.translate(-2.f/**i*/, -1.f, 5.f/**j*/);
    boss->mTransform->mMatrix.scale(1.5f);
    mResourceManager->addCollider("sphere", boss);
    mResourceManager->addComponent("run_stereo.wav", boss);
    boss->mSoundComponent->shouldPlay = false;
    //looping fungerer ikke
    boss->mSoundComponent->looping = false;
    boss->objName = "boss";
    //mRenderSystem->mGameObjects.push_back(boss);

    mTerrain = mResourceManager->addObject("terrain");
    mTerrain->mTransform->mMatrix.translateZ(-100.f);
    mRenderSystem->mGameObjects.push_back(mTerrain);
    //setup camera

    for(float i = 0; i < 4; i += 0.1)
    {

        mParticles = mResourceManager->addObject("particle");
        mParticles->mMaterial->mShaderProgram = 0;
        mParticles->mMaterial->mTextureUnit = 0;
        mParticles->mTransform->mMatrix.setPosition(1.f *i, 1.f, 1.f*i);
        mRenderSystem->mGameObjects.push_back(mParticles);
    }



    mGameCamera = new Camera();
    mEditorCamera = new Camera();
    mGameCamera->mViewMatrix.rotateY(30.f);
    mGameCamera->mViewMatrix.rotateX(90.f);
    mEditorCamera->mPosition = gsl::Vector3D(1.f, .5f, 4.f);

    mGameCamera->mPosition = player->mTransform->mMatrix.getPosition();

    mRenderSystem->mCurrentCamera = mEditorCamera;



    mResourceManager->setUpAllTextures();

    //Connect the gameloop timer to the render function:
    //This makes our render loop
    connect(mGameLoopTimer, SIGNAL(timeout()), this, SLOT(gameLoop()));
    //This timer runs the actual MainLoop
    //16 means 16ms = 60 Frames pr second (should be 16.6666666 to be exact...)
    mGameLoopTimer->start(16);
}

void CoreEngine::updateCamera()
{
    if (isPlaying)
    {

        // MÅ ORDNE ROTATION RIKTIG
        gsl::Vector3D playerpos = player->mTransform->mMatrix.getPosition();
        mRenderSystem->mCurrentCamera->mPosition = playerpos;
        mRenderSystem->mCurrentCamera->mPosition.setZ(playerpos.getZ()+2.f);
        mRenderSystem->mCurrentCamera->mPosition.setY(playerpos.getY()+1.5);

    }


}

void CoreEngine::updateScene()
{
    //TODO:

    updateCamera();

   // mResourceManager->update(mRenderSystem->dt);

    if(getInstance()->mResourceManager->checkCollision(
    player, enemy))
    {
        qDebug() << "collided !";
        enemy->mTransform->mMatrix.rotateX(90);
        enemy->mCollider->objectsHasCollided = true;
        enemy->mSoundComponent->shouldPlay = true;

    }
    else
        enemy->mSoundComponent->shouldPlay = false;


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

        if(isPlaying)
         player->move(0,0 , speed);


    }
    else if(mInput.S)
    {
if(isPlaying)
         player->move(0,0 , -speed);
    }
     else if(mInput.A)
    {
if(isPlaying)
         player->move(speed, 0, 0);
    }
     else if(mInput.D)
    {
if(isPlaying)
         player->move(-speed, 0, 0);
    }
    else if(mInput.Q)
    {

        if(isPlaying)
        player->move(0, speed, 0);
    }
    else if(mInput.E)
    {
            if(isPlaying)
        player->move(0, -speed, 0);
    }

}

void CoreEngine::playSound(std::string assetName)
{
    mResourceManager->addComponent(assetName, player);
}

void CoreEngine::gameLoop()
{
    //Keyboard / mouse input
    handleInput();

    mRenderSystem->mCurrentCamera->update();
    SoundSystem::getInstance()->update(mRenderSystem);

    mRenderSystem->render();
}

