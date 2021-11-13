#include "coreengine.h"

#include "resourcemanager.h"
#include "soundsystem.h"

#include "gameobject.h"
#include "rendersystem.h"
#include "camera.h"
#include <random>
#include <iostream>
#include <cstdlib>
#include <ctime>

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
    player->mTransform->mMatrix.translate(0.f, 0, -20);
    mResourceManager->addCollider("sphere", player);
    //Adds sound to player:
    mResourceManager->addComponent("caravan_mono.wav", player);

    //Hack to test sound system
    player->mSoundComponent->shouldPlay = true;
    player->objName = "Player";
    //mRenderSystem->mGameObjects.push_back(player);

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

//    mTerrain = mResourceManager->addObject("terrain");
//    mTerrain->mTransform->mMatrix.translateZ(-100.f);
//    mRenderSystem->mGameObjects.push_back(mTerrain)

    projectile = mResourceManager->addObject("projectile");
    projectile->mTransform->mMatrix.rotateY(180.f);
    projectile->mTransform->mMatrix.translate(0.f, 0, -2.5);
    mResourceManager->addCollider("sphere", projectile);
    mResourceManager->addComponent("splat_stereo.wav", projectile);
    projectile->mSoundComponent->shouldPlay = false;

    //mRenderSystem->mGameObjects.push_back(projectile);

    skybox = mResourceManager->addObject("skybox");
    skybox->mMaterial->mShaderProgram = 1;
    skybox->mMaterial->mTextureUnit = 2;
    skybox->mTransform->mMatrix.scale(40.f);
    //mRenderSystem->mGameObjects.push_back(skybox);

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

void CoreEngine::testScene()
{
    for(int i = 0; i < 10; i++)
    {

        enemy = mResourceManager->addObject("suzanne3.obj");
        enemy->mTransform->mMatrix.translate(1*i, 0,0.f);
        enemy->mMaterial->mShaderProgram = 1;
        enemy->mMaterial->mTextureUnit = 2;
        enemy->mTransform->mMatrix.rotateY(180.f);
        enemy->mTransform->mMatrix.scale(0.5f);

        mResourceManager->addCollider("sphere", enemy);
        mResourceManager->addComponent("roblox_stereo.wav", enemy);
        enemy->mSoundComponent->shouldPlay = false;
        enemy->mSoundComponent->looping = false;

        mRenderSystem->mGameObjects.push_back(enemy);

        enemy->mName = "enemy " + std::to_string(enemies.size());

        qDebug() << "enemy " << i;

        enemies.push_back(enemy);

    }

        mRenderSystem->mGameObjects.push_back(player);
        mRenderSystem->mGameObjects.push_back(axis);
        playerSpawned = true;
        enemySpawned = true;
}

void CoreEngine::spawnParticles(GameObject * temp)
{
    srand( (unsigned)time( NULL ) );

    tempPosX = temp->mTransform->mMatrix.getPosition().getX();
    tempPosY = temp->mTransform->mMatrix.getPosition().getY();
    tempPosZ = temp->mTransform->mMatrix.getPosition().getZ();

    for(float i = 0; i < .5; i += 0.002)
    {

        Particles = mResourceManager->addObject("particle");
        Particles->mMaterial->mShaderProgram = 0;
        Particles->mMaterial->mTextureUnit = 0;
        Particles->mTransform->mMatrix.scale(.02);
        Particles->mTransform->mMatrix.setPosition(tempPosX,tempPosY,tempPosZ);
        mRenderSystem->mParticles.push_back(Particles);
    }
    particlesSpawned = true;
    //Particles->isAlive = true;
}

void CoreEngine::spawnProjectile()
{
    tempPosX = player->mTransform->mMatrix.getPosition().getX();
    tempPosY = player->mTransform->mMatrix.getPosition().getY();
    tempPosZ = player->mTransform->mMatrix.getPosition().getZ();

    projectile->mTransform->mMatrix.setPosition(tempPosX, tempPosY, tempPosZ);
    ProjectileSpawned = true;
    mRenderSystem->mGameObjects.push_back(projectile);


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
    //mParticles->move(0,1,0);
    updateCamera();

    //projectile->mTransform->mMatrix.setPosition();

    //prøve få particles til å spawne på enemy

    if(ProjectileSpawned)
    projectile->mTransform->mMatrix.translateZ(.01f);

//    if(enemySpawned && !goatDead)
//        enemies->mTransform->mMatrix.translateZ(-.01);
   // mResourceManager->update(mRenderSystem->dt);

    for(unsigned int i = 0; i< enemies.size(); i++)
    {
        if(enemies[i]->isAlive)
        enemies[i]->mTransform->mMatrix.translateZ(-.001);

        //test collision
        if(!enemies[i]->mCollider->objectsHasCollided)
        {
            if(getInstance()->mResourceManager->checkCollision(
            projectile, enemies[i]))
            {
                qDebug() << "collided !";
                enemies[i]->mTransform->mMatrix.rotateZ(90);
                enemies[i]->mCollider->objectsHasCollided = true;
                enemies[i]->mSoundComponent->shouldPlay = true;
                enemies[i]->isAlive = false;

                //spawnParticles(enemies[i]);

                projectile->mSoundComponent->shouldPlay = true;

                //velger om man skal drawe et object eller ikke
                goatDead = true;

                qDebug() << "enemy " << i << " hit";


                //projectile->isAlive = false;
                //enemy->isAlive = false;

                //enemy->mSoundComponent->shouldPlay = false;
            }


        }

        if(player->isAlive && enemies[i]->isAlive)
        {
            if(getInstance()->mResourceManager->checkCollision(
            player, enemies[i]))
            {
                qDebug() << "You died!";
                player->mTransform->mMatrix.rotateZ(90);
                //enemy->mCollider->objectsHasCollided = true;
                enemies[i]->mSoundComponent->shouldPlay = true;
                //enemy->mSoundComponent->looping = false;
                spawnParticles(player);
                projectile->mSoundComponent->shouldPlay = true;

                player->isAlive = false;

            }

        }
    }


}

void CoreEngine::handleInput()
{
    //Camera
    float speed = 0.2f;
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
    else if(mInput.LMB)
    {
        if(isPlaying && playerSpawned)
        {

            spawnProjectile();
        }
    }
    else if(mInput.W)
    {

        if(isPlaying)
         player->move(0,0 , speed /2);


    }
    else if(mInput.S)
    {
if(isPlaying)
         player->move(0,0 , -speed /2);
    }
     else if(mInput.A)
    {
        if(isPlaying)
        {
            player->move(speed, 0, 0);
            //testDelete = true;
        }

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

