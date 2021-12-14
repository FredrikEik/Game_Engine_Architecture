#include "coreengine.h"

#include "resourcemanager.h"
#include "soundsystem.h"
#include <qfile.h>
#include <qjsondocument.h>
#include <qjsonobject.h>
#include <qjsonarray.h>
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

    //loadBoss("boss.json");
    mResourceManager->setUpAllTextures();

    //Axis
    axis = mResourceManager->addObject("axis");
    axis->objName = "Axis";
    mRenderSystem->mGameObjects.push_back(axis);
    //scene->setupScene1();
    //scene->setupScene1();

    //PLAYER
    player = mResourceManager->addObject("suzanne.obj");
    player->mMaterial->mShaderProgram = 0;
    player->mMaterial->mTextureUnit = 0;
    player->mTransform->mMatrix.rotateY(180.f);
    player->mTransform->mMatrix.scale(0.5f);
    player->mTransform->mMatrix.translate(0.f, 0, -20);
    mResourceManager->addCollider("sphere", player);
    //Adds sound to player:
    mResourceManager->addComponent("neon_stereo.wav", player);

    //Hack to test sound system
    player->mSoundComponent->shouldPlay = true;
    player->objName = "Player";
    //mRenderSystem->mGameObjects.push_back(player);

    //BOSS
//    boss = mResourceManager->addObject("suzanne3.obj");
//    boss->mTransform->mMatrix.translate(-2, 1.f,2.f);
//    boss->mMaterial->mShaderProgram = 1;
//    boss->mMaterial->mTextureUnit = 2;
//    boss->mTransform->mMatrix.rotateY(180.f);
//    boss->mTransform->mMatrix.translate(-2.f/**i*/, -1.f, 5.f/**j*/);
//    boss->mTransform->mMatrix.scale(1.5f);
//    mResourceManager->addCollider("sphere", boss);
//    mResourceManager->addComponent("run_stereo.wav", boss);
//    boss->mSoundComponent->shouldPlay = false;
//    //looping fungerer ikke
//    boss->mSoundComponent->looping = false;
//    boss->objName = "boss";
    //mRenderSystem->mGameObjects.push_back(boss);

    skybox = mResourceManager->addObject("skybox");
    skybox->mMaterial->mShaderProgram = 2;
    skybox->mMaterial->mTextureUnit = 1;
    skybox->mTransform->mMatrix.scale(40.f);
    mRenderSystem->mGameObjects.push_back(skybox);

    mGameCamera = new Camera();
    mEditorCamera = new Camera();
    mGameCamera->mViewMatrix.rotateY(30.f);
    mGameCamera->mViewMatrix.rotateX(90.f);
    mEditorCamera->mPosition = gsl::Vector3D(0.f, 20.f, 13.f);

    mGameCamera->mPosition = player->mTransform->mMatrix.getPosition();

    mRenderSystem->mCurrentCamera = mEditorCamera;

    Las = mResourceManager->addObject("LAS");
    Las->mMaterial->mShaderProgram =3;
    Las->mMaterial->mTextureUnit = 0;
    Las->mTransform->mMatrix.setPosition(3,-5,0);
    mRenderSystem->mGameObjects.push_back(Las);

    //Connect the gameloop timer to the render function:
    //This makes our render loop
    connect(mGameLoopTimer, SIGNAL(timeout()), this, SLOT(gameLoop()));
    //This timer runs the actual MainLoop
    //16 means 16ms = 60 Frames pr second (should be 16.6666666 to be exact...)
    mGameLoopTimer->start(16);
    for(unsigned int i{0}; i < mRenderSystem->mGameObjects.size(); i++)
    {
        mRenderSystem->mGameObjects[i]->startPos =
                mRenderSystem->mGameObjects[i]->mTransform->mMatrix.getPosition();
    }






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

        enemy->objName = "enemy";

        qDebug() << "enemy " << i;


        enemies.push_back(enemy);


    }

    projectile = mResourceManager->addObject("projectile");
    projectile->mTransform->mMatrix.rotateY(180.f);
    projectile->mTransform->mMatrix.translate(0.f, 0, -2.5);
    mResourceManager->addComponent("splat_stereo.wav", projectile);
    projectile->mSoundComponent->shouldPlay = false;
    mResourceManager->addCollider("sphere", projectile);
    mRenderSystem->mGameObjects.push_back(player);
    playerSpawned = true;
    enemySpawned = true;

    for(unsigned int i{0}; i < mRenderSystem->mGameObjects.size(); i++)
    {
        mRenderSystem->mGameObjects[i]->startPos =
                mRenderSystem->mGameObjects[i]->mTransform->mMatrix.getPosition();
    }


}


void CoreEngine::spawnParticles(GameObject * temp)
{

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
    projectile->ProjectileSpawned = true;
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

    updateCamera();

    if(spawnBoss)
    {
        loadBoss("Boss.json");
        spawnBoss = false;
    }

    if(projectile->ProjectileSpawned)
    projectile->mTransform->mMatrix.translateZ(.01f);

    for(unsigned int i = 0; i< enemies.size(); i++)
    {
        if(enemies[i]->isAlive)
        enemies[i]->mTransform->mMatrix.translateZ(-.002);

        //test collision
        if(!enemies[i]->mCollider->objectsHasCollided)
        {
            if(getInstance()->mResourceManager->checkCollision(
            projectile, enemies[i]))
            {
                qDebug() << "collided !";
                enemies[i]->mTransform->mMatrix.rotateZ(90);
                enemies[i]->mCollider->objectsHasCollided = true;
                //enemies[i]->mSoundComponent->shouldPlay = true;
                enemies[i]->isAlive = false;

                //spawnParticles(enemies[i]);


                // INVALID NAME ERROR KOMMER AV FEIL MED LYDEN!

               // projectile->mSoundComponent->shouldPlay = true;

                //velger om man skal drawe et object eller ikke
                goatDead = true;

                qDebug() << "enemy " << i << " hit";

                //enemy->mSoundComponent->shouldPlay = false;

                score += 10;


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


                testDelete = true;

                player->isAlive = false;

            }

        }
    }



    //mMainWindow->UpdateScore(score);



}

void CoreEngine::UpdateSimulation()
{
    if(!Rain.empty()){
        for(auto i{0}; i<static_cast<int>(Rain.size()); i++)
        {
        getInstance()->mResourceManager->moveAlongSurface(0.0017, ball);
        }
    }

}

void CoreEngine::reset()
{
    for(unsigned int i{0}; i < mRenderSystem->mGameObjects.size(); i++)
    {

        mRenderSystem->mGameObjects[i]->mTransform->mMatrix.setPosition(
        mRenderSystem->mGameObjects[i]->startPos.x,
        mRenderSystem->mGameObjects[i]->startPos.y,
        mRenderSystem->mGameObjects[i]->startPos.z);
    }

}

void CoreEngine::loadBoss(std::string scene)
{
    QFile loadFile(QString((gsl::AssetFilePath + scene).c_str()));
    if (!loadFile.open(QIODevice::ReadOnly)) {
        qDebug() << "error reading JSON file";
        return;
    }
    qDebug() << "scene open";
    QByteArray saveData = loadFile.readAll();   //read whole file
    QJsonDocument loadDoc(QJsonDocument::fromJson(saveData));   //convert to json document
    QJsonObject jsonObject = loadDoc.object();  //read first object == whole thing

    if (jsonObject.contains("Boss") && jsonObject["Boss"].isArray())
    {
        QJsonArray entityArray = jsonObject["Boss"].toArray();
        //go through each entity in file
        int entityIndex{0};   //used to print log afterwards
        for ( ; entityIndex < entityArray.size(); ++entityIndex) {
            GameObject *tempObj{nullptr};
            QString name;   //this is pushed as the key to the mEntityMap later ???? - maybe not

            QJsonObject entityObject = entityArray[entityIndex].toObject();   //first entity as object

            if (entityObject.contains("name") && entityObject["name"].isString())
                name = entityObject["name"].toString();

            if (entityObject.contains("mesh") && entityObject["mesh"].isString())
            {
                QString mesh = entityObject["mesh"].toString();
                tempObj = mResourceManager->addObject(mesh.toStdString());

            }

            if (entityObject.contains("position") && entityObject["position"].isArray())
            {
                QJsonArray positionArray = entityObject["position"].toArray();
                if (positionArray[0].isDouble() && positionArray[0].isDouble() && positionArray[0].isDouble())
                    tempObj->mTransform->mMatrix.setPosition(positionArray[0].toDouble(),
                            positionArray[1].toDouble(), positionArray[2].toDouble());


            }
            if (entityObject.contains("scale") && entityObject["scale"].isArray())
            {

                QJsonArray scaleArray = entityObject["scale"].toArray();
                if (scaleArray[0].isDouble() && scaleArray[0].isDouble() && scaleArray[0].isDouble())
                tempObj->mTransform->mMatrix.scale(scaleArray[0].toDouble(), scaleArray[1].toDouble(),
                        scaleArray[2].toDouble());

            }
            mRenderSystem->mGameObjects.push_back(tempObj);

        }

    }
    else
        qDebug() << "somwthing not right";

}

void CoreEngine::handleInput()
{
    //Camera
    float speed = .5f;
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

        if(isPlaying && player->isAlive)
         player->move(0,0 , speed /2);


    }
    else if(mInput.S)
    {
    if(isPlaying && player->isAlive)
         player->move(0,0 , -speed /2);
    }
     else if(mInput.A)
    {
        if(isPlaying && player->isAlive)
        {
            player->move(speed, 0, 0);
            //testDelete = true;
        }

    }
     else if(mInput.D)
    {
    if(isPlaying && player->isAlive)
         player->move(-speed, 0, 0);
    }
    else if(mInput.Q)
    {

        if(isPlaying && player->isAlive)
        player->move(0, speed, 0);
    }
    else if(mInput.E)
    {
            if(isPlaying && player->isAlive)
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

