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
    //spill modus
    if (shouldPlay == true)
    {
        SoundSystem::getInstance()->togglePlaySounds(true);
        mRenderSystem->isPlaying = true;
        isPlaying = true;
    }
    //editor modus
    else
    {
        SoundSystem::getInstance()->togglePlaySounds(false);
        isPlaying = false;
        mRenderSystem->isPlaying = false;
        mRenderSystem->mCurrentCamera = mEditorCamera;
        mEditorCamera->mPosition = gsl::Vector3D(1.f, .5f, 4.f);
    }

}

///setter opp default scene for editoren
void CoreEngine::setUpScene()
{
    //setter opp alle textures
    mResourceManager->setUpAllTextures();
    //lager xyz akse for editormodus
    axis = mResourceManager->addObject("axis");
    axis->objName = "Axis";
    mRenderSystem->mGameObjects.push_back(axis);

    //Setter opp skybox
    skybox = mResourceManager->addObject("skybox");
    skybox->mMaterial->mShaderProgram = 4;
    skybox->mMaterial->mTextureUnit = 6;
    skybox->mTransform->mMatrix.scale(40.f);
    skybox->mTransform->mMatrix.rotateZ(180);
    skybox->useFrustum = false;
    mRenderSystem->mGameObjects.push_back(skybox);

    //setter opp begge kameraene
    mGameCamera = new Camera();
    mEditorCamera = new Camera();
    mGameCamera->mViewMatrix.rotateY(30.f);
    mGameCamera->mViewMatrix.rotateX(90.f);
    mEditorCamera->mPosition = gsl::Vector3D(0.f, 10.f, 13.f);


    mRenderSystem->mCurrentCamera = mEditorCamera;

    //Setter opp terrenget hentet fra las data
    Las = mResourceManager->addObject("LAS");
    Las->mMaterial->mShaderProgram = 1;
    Las->mMaterial->mTextureUnit = 1;
    Las->mTransform->mMatrix.setPosition(3,-5,0);
    Las->useFrustum = false;
    mRenderSystem->mGameObjects.push_back(Las);


    //game/render loopen som skal oppdateres 60 ganger i sekundet
    connect(mGameLoopTimer, SIGNAL(timeout()), this, SLOT(gameLoop()));
    mGameLoopTimer->start(16);

}

void CoreEngine::testScene()
{

    //spawner 10 fiender på rekke
    for(int i = 0; i < 10; i++)
    {

        enemy = mResourceManager->addObject("suzanne3.obj");
        enemy->mTransform->mMatrix.translate(1*i, 0,0.f);
        enemy->mMaterial->mShaderProgram = 3;
        enemy->mMaterial->mTextureUnit = 2;
        enemy->mTransform->mMatrix.rotateY(180.f);
        enemy->mTransform->mMatrix.scale(0.5f);
        //legger til sphere kollisjon og lyd
        mResourceManager->addCollider("sphere", enemy);
        mResourceManager->addComponent("roblox_stereo.wav", enemy);
        enemy->mSoundComponent->shouldPlay = false;
        enemy->mSoundComponent->looping = false;
        mRenderSystem->mGameObjects.push_back(enemy);
        enemy->objName = "enemy";
        enemies.push_back(enemy);

    }

    //legger til prosjektil
    projectile = mResourceManager->addObject("projectile");
    projectile->mTransform->mMatrix.rotateY(180.f);
    projectile->mTransform->mMatrix.translate(0.f, 0, -25);
    mResourceManager->addComponent("splat_stereo.wav", projectile);
    projectile->mSoundComponent->shouldPlay = false;
    mResourceManager->addCollider("sphere", projectile);



    //legger til en spiller
    player = mResourceManager->addObject("suzanne.obj");
    player->mMaterial->mShaderProgram = 0;
    player->mMaterial->mTextureUnit = 0;
    player->mTransform->mMatrix.rotateY(180.f);
    player->mTransform->mMatrix.scale(0.5f);
    player->mTransform->mMatrix.translate(0.f, 0, -20);
    //legger til sphere kollisjon
    mResourceManager->addCollider("sphere", player);
    //legger til musikk til spilleren:
    mResourceManager->addComponent("neon_stereo.wav", player);
    player->mSoundComponent->shouldPlay = true;
    player->objName = "Player";
    mGameCamera->mPosition = player->mTransform->mMatrix.getPosition();


    mRenderSystem->mGameObjects.push_back(player);
    playerSpawned = true;
    enemySpawned = true;

    //setter startpoisjon til objektene så de kan resettes senere
    for(unsigned int i{0}; i < mRenderSystem->mGameObjects.size(); i++)
    {
        mRenderSystem->mGameObjects[i]->startPos =
                mRenderSystem->mGameObjects[i]->mTransform->mMatrix.getPosition();
    }


}


void CoreEngine::spawnParticles(GameObject * temp)
{

    /// Spawner partikler på posisjonen til objektet
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

}

void CoreEngine::spawnProjectile()
{
    /// spawner prosjektilene ut fra spilleren
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

        /// oppdaterer 3.person kamera
        gsl::Vector3D playerpos = player->mTransform->mMatrix.getPosition();
        mRenderSystem->mCurrentCamera->mPosition = playerpos;
        mRenderSystem->mCurrentCamera->mPosition.setZ(playerpos.getZ()+2.f);
        mRenderSystem->mCurrentCamera->mPosition.setY(playerpos.getY()+1.5);

    }


}

void CoreEngine::updateScene()
{

    updateCamera();

    ///bestemmer hva som skal rendres i editor eller play mode
    if(isPlaying)
        axis->RenderInPlaymode = false;
    else
        axis->RenderInPlaymode = true;

    if(spawnBoss)
    {
        loadBoss("Boss.json");
        spawnBoss = false;
    }

    ///beveger prosjektilen
    if(projectile->ProjectileSpawned)
    projectile->mTransform->mMatrix.translateZ(.01f);

    for(unsigned int i = 0; i< enemies.size(); i++)
    {
        ///beveger fiender
        if(enemies[i]->isAlive)
        enemies[i]->mTransform->mMatrix.translateZ(-.002);


        if(!enemies[i]->mCollider->objectsHasCollided)
        {
            ///sjekker om fiende blir truffet av prosjektilen
            if(getInstance()->mResourceManager->checkCollision(
            projectile, enemies[i]))
            {
                qDebug() << "collided !";
                ///effekter får å se at fienden er død
                enemies[i]->mTransform->mMatrix.rotateZ(90);

                enemies[i]->mCollider->objectsHasCollided = true;
                //en del bugs med lyden, kan uncommente for å se at det funker
                //enemies[i]->mSoundComponent->shouldPlay = true;
                enemies[i]->isAlive = false;

                qDebug() << "enemy " << i << " hit";

                ///oppdaterer score
                score += 10;


            }


        }

        if(player->isAlive && enemies[i]->isAlive)
        {
            ///Sjekker om fienden treffer spilleren
            if(getInstance()->mResourceManager->checkCollision(
            player, enemies[i]))
            {
                qDebug() << "You died!";
                player->mTransform->mMatrix.rotateZ(90);
                enemies[i]->mSoundComponent->shouldPlay = true;
                ///spawner blodpartikler
                spawnParticles(player);
                projectile->mSoundComponent->shouldPlay = true;
                particleTimer = true;
                player->isAlive = false;

            }

        }
    }




}
//simulerings innlevering
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
        ///resetter posisjonen til objektene i scenen
        mRenderSystem->mGameObjects[i]->mTransform->mMatrix.setPosition(
        mRenderSystem->mGameObjects[i]->startPos.x,
        mRenderSystem->mGameObjects[i]->startPos.y,
        mRenderSystem->mGameObjects[i]->startPos.z);
    }

}

void CoreEngine::loadBoss(std::string file)
{
    ///henter fil fra mappe
    QFile loadFile(QString((gsl::AssetFilePath + file).c_str()));
    if (!loadFile.open(QIODevice::ReadOnly)) {
        qDebug() << "error reading JSON file";
        return;
    }

    QByteArray saveData = loadFile.readAll();   ///leser json fil
    QJsonDocument loadDoc(QJsonDocument::fromJson(saveData));   ///konverterer til json dokument
    QJsonObject jsonObject = loadDoc.object();  ///leser objekt data

    if (jsonObject.contains("Boss") && jsonObject["Boss"].isArray())
    {
        QJsonArray entityArray = jsonObject["Boss"].toArray();
        ///går gjennom alle entetiene i filen, for øyeblikket bare en

        for ( int entityIndex{0}; entityIndex < entityArray.size(); ++entityIndex) {
            GameObject *tempObj{nullptr};
            QString name;

            QJsonObject entityObject = entityArray[entityIndex].toObject();

            ///leser mesh navnet så man lager en entity utifra det
            if (entityObject.contains("mesh") && entityObject["mesh"].isString())
            {
                QString mesh = entityObject["mesh"].toString();
                tempObj = mResourceManager->addObject(mesh.toStdString());

            }
            ///henter posisjonen fra filen og setter posisjonen til bossen
            if (entityObject.contains("position") && entityObject["position"].isArray())
            {
                QJsonArray positionArray = entityObject["position"].toArray();
                if (positionArray[0].isDouble() && positionArray[0].isDouble() && positionArray[0].isDouble())
                    tempObj->mTransform->mMatrix.setPosition(positionArray[0].toDouble(),
                            positionArray[1].toDouble(), positionArray[2].toDouble());


            }
            ///henter skaleringen og setter riktig størrelse til bossen
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


}

void CoreEngine::handleInput()
{
    ///Camera
    float speed = .4f;
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
        if(isPlaying && playerSpawned && player->isAlive)
        {
            ///skyter prosjektiler
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

void CoreEngine::addEntity(std::string assetName)
{
    GameObject* temp = mResourceManager->addObject(assetName);
    mRenderSystem->mGameObjects.push_back(temp);
}

void CoreEngine::gameLoop()
{
    ///Keyboard / mouse input
    handleInput();

    mRenderSystem->mCurrentCamera->update();
    SoundSystem::getInstance()->update(mRenderSystem);

    mRenderSystem->render();
}

