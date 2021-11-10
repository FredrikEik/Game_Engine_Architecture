#include "gameengine.h"

#include <QKeyEvent>
#include <QString>

#include "resourcemanager.h"
#include "gameobject.h"
#include "renderwindow.h"
#include "soundmanager.h"
#include "camera.h"
#include "physicsballsystem.h"
#include "mainwindow.h"
#include "collisionsystem.h"
#include "constants.h"


// Make Singelton
GameEngine* GameEngine::mInstance = nullptr;

//TODO: endre slik at constructoren blir kalt
GameEngine::GameEngine()
{
}

void GameEngine::GameLoop()
{
    //mPhysicsBallSystem->update(*mPhysicsBall);

    rotateLight();

    HandleInput();

    //mCurrentCameraUPDATE??
    mEditorCamera->update();
    mGameCamera->update();

    mRenderwindow->render();

}

void GameEngine::SetUpScene()
{


    mResourceManager = new class ResourceManager();
    mPhysicsBallSystem = new PhysicsBallSystem();
    mInstance = this;
    mGameLoopRenderTimer = new QTimer(this);

    mEditorCamera = new Camera();
    mEditorCamera->setPosition(gsl::Vector3D(7.f, -0.8f, 2.f));
    mEditorCamera->yaw(-45);
    mRenderwindow->mCurrentCamera = mEditorCamera;

    mGameCamera = new Camera();
    mGameCamera->setPosition(gsl::Vector3D(0,2,0));
    mGameCamera->yaw(180);

    mResourceManager->setUpAllTextures();




    SetUpObjects();

    objBeforePlaying = mRenderwindow->mGameObjects;
    //TODO SetupTextures


    // Sound startup stuff
    mStereoSound = SoundManager::getInstance()->createSource(
                "Stereo", gsl::Vector3D(0.0f, 0.0f, 0.0f),
                gsl::SoundFilePath + "rock_background.wav", false, 1.0f);

    //mStereoSound->play();

    //Gameloop
    connect(mGameLoopRenderTimer, SIGNAL(timeout()), this, SLOT(GameLoop()));
    mGameLoopRenderTimer->start(7);// original value 16, 7 is for 144 hz

    mMainWindow->initList();
}

void GameEngine::SetUpObjects()
{
//    mResourceManager->loadScene(mRenderwindow->mGameObjects, mPlayer,mLight);
                                          //(gsl::MeshFilePath + "player.obj", gsl::TextureFilePath + "playerTexture")
    mPlayer = mResourceManager->CreateObject(gsl::MeshFilePath + "player.obj",false,"hund.bmp");

    mPlayer->mTransformComp->mMatrix.setPosition(0,-1.8f,-8);
    initPlayerPos = {0,-1.8f,-8};
    mPlayer->mMaterialComp->mShaderProgram = 2;
//    mPlayer->mMaterialComp->mTextureUnit = 1;
    //mPlayer->mMaterialComp->mShaderProgram = gsl::COLORSHADER;

    qDebug() << "PLAYER filepath: " << mPlayer->filepath.c_str();

    mRenderwindow->mGameObjects.push_back(mPlayer);



    //    for(int i{0}; i < 100; i++)
    //    {
    //        for(int j{0}; j < 100; j++)
    //        {
    //            tempGameObject = mResourceManager->CreateObject(gsl::MeshFilePath + "suzanne.obj", true);
    //            tempGameObject->mTransformComp->mMatrix.setPosition(2.f*(i), -3.f, -2.f*(j));
    //            mRenderwindow->mGameObjects.push_back(tempGameObject);
    //        }
    //    }

//    tempGameObject = mResourceManager->CreateObject(gsl::MeshFilePath + "cube.obj", false, "hund.bmp");
//    mRenderwindow->mGameObjects.push_back(tempGameObject);



//    tempGameObject = mResourceManager->CreateObject(gsl::MeshFilePath + "suzanne.obj", true, "hund.bmp");
//    tempGameObject->mTransformComp->mMatrix.setPosition(4,0,-2);
//    mRenderwindow->mGameObjects.push_back(tempGameObject);

//    tempGameObject = mResourceManager->CreateObject(gsl::MeshFilePath + "suzanne.obj", true);
//    tempGameObject->mTransformComp->mMatrix.setPosition(2,0,-10);
//    // tempGameObject->mMeshComp->mDrawType=GL_LINES;
//    mRenderwindow->mGameObjects.push_back(tempGameObject);

    mLight = mResourceManager->CreateObject(gsl::MeshFilePath + "light.obj",false, "cocademon3.bmp");
    mLight->mTransformComp->mMatrix.setPosition(-20,3,-10);
    mLight->mMaterialComp->mShaderProgram = 2;
//    mLight->mMaterialComp->mTextureUnit = 1;
    mRenderwindow->mGameObjects.push_back(mLight);

//    mLight2 = mResourceManager->CreateObject(gsl::MeshFilePath + "light.obj");
//    mLight2->mTransformComp->mMatrix.setPosition(-1,3,-5);
//    mLight2->mMaterialComp->mShaderProgram = gsl::COLORSHADER;
//    mRenderwindow->mGameObjects.push_back(mLight2);

//    tempGameObject = mResourceManager->CreateObject(gsl::MeshFilePath + "cacodemon2.obj", false, "cocademon3.bmp");
//    tempGameObject->mTransformComp->mMatrix.setPosition(5,0,-7);

//    mRenderwindow->mGameObjects.push_back(tempGameObject);


    tempGameObject = mResourceManager->CreateObject(gsl::MeshFilePath + "cube.obj",false, "orange.bmp");
    tempGameObject->mTransformComp->mMatrix.setPosition(-1,-1,-5);
    tempGameObject->mMaterialComp->mShaderProgram = 2;
//    tempGameObject->mMaterialComp->mTextureUnit = 3;
    mRenderwindow->mGameObjects.push_back(tempGameObject);


//    tempGameObject = mResourceManager->CreateObject(gsl::MeshFilePath + "cube.obj");
//    tempGameObject->mTransformComp->mMatrix.setPosition(-3,-1,-5);
//    tempGameObject->mMaterialComp->mShaderProgram = gsl::COLORSHADER;
//    mRenderwindow->mGameObjects.push_back(tempGameObject);

//    tempGameObject = mResourceManager->CreateObject(gsl::MeshFilePath + "sphere.obj", true);
//    tempGameObject->mTransformComp->mMatrix.setPosition(14,7,-27);
//    tempGameObject->mMaterialComp->mShaderProgram = gsl::COLORSHADER;
//    mRenderwindow->mGameObjects.push_back(tempGameObject);

//    tempGameObject = mResourceManager->CreateObject(gsl::MeshFilePath + "pyramid.obj");
//    tempGameObject->mTransformComp->mMatrix.setPosition(-1,0.5,-5);
//    tempGameObject->mMaterialComp->mShaderProgram = gsl::COLORSHADER;
//    mRenderwindow->mGameObjects.push_back(tempGameObject);


    // TEST
//    tempGameObject = mResourceManager->CreateObject(gsl::MeshFilePath + "sphere.obj",false, gsl::AssetFilePath + "texture");
//    tempGameObject->mTransformComp->mMatrix.setPosition(-1,0.5,-5);
//    tempGameObject->mMaterialComp->mShaderProgram = gsl::COLORSHADER;
//    mRenderwindow->mGameObjects.push_back(tempGameObject);

//    tempGameObject = mResourceManager->CreateObject(gsl::MeshFilePath + "sphere.obj");
//    tempGameObject->mTransformComp->mMatrix.setPosition(-5,2.5,0);
//    tempGameObject->mMaterialComp->mShaderProgram = gsl::PHONGSHADER;
//    mRenderwindow->mGameObjects.push_back(tempGameObject);

//    tempGameObject = mResourceManager->CreateObject(gsl::MeshFilePath + "suzanne.obj", true);
//    tempGameObject->mTransformComp->mMatrix.setPosition(-12,0.5,-13);
//    tempGameObject->mMaterialComp->mShaderProgram = gsl::PHONGSHADER;
//    mRenderwindow->mGameObjects.push_back(tempGameObject);

//    tempGameObject = mResourceManager->CreateObject(gsl::MeshFilePath + "suzanne.obj", true);
//    tempGameObject->mTransformComp->mMatrix.setPosition(3,-0.5f,-3);
//    tempGameObject->mMaterialComp->mShaderProgram = gsl::PHONGSHADER;
//    mRenderwindow->mGameObjects.push_back(tempGameObject);



    //mResourceManager->saveScene(mRenderwindow->mGameObjects);



//    tempGameObject = mResourceManager->CreateObject("xyz");
//    tempGameObject->mMeshComp->mDrawType = GL_LINES;
//    tempGameObject->mMaterialComp->mTextureUnit = 0;
//    tempGameObject->mMaterialComp->mShaderProgram = 0;
//    mRenderwindow->mGameObjects.push_back(tempGameObject);





    // * * * * * Visualisering og simulering * * * * *

//    mPhysicsBall = mResourceManager->CreateObject(gsl::MeshFilePath + "sphere.obj");
//    mPhysicsBall->mTransformComp->mMatrix.translate(4.8f,-0.77f,-0.8f);
//    mPhysicsBall->mTransformComp->mMatrix.scale(0.1);
//    mPhysicsBall->mMaterialComp->mShaderProgram = 2;
//    mRenderwindow->mGameObjects.push_back(mPhysicsBall);

//    mTerrainObject = mResourceManager->CreateObject(gsl::MeshFilePath + "toTriangles.obj");
//    mTerrainObject->mTransformComp->mMatrix.translate(4,-1,-1);
//    mTerrainObject->mMaterialComp->mShaderProgram = 2;
//    //mTerrainObject->mMaterialComp->mTextureUnit = 2;
//    mRenderwindow->mGameObjects.push_back(mTerrainObject);

//    mPhysicsBallSystem->SetTerrainData(*mTerrainObject);





//    tempGameObject = mResourceManager->CreateObject(gsl::MeshFilePath + "HamarHeightMap.obj");
//    tempGameObject->mTransformComp->mMatrix.translate(0,-15,0);
//    tempGameObject->mMaterialComp->mShaderProgram = 2;
//    mRenderwindow->mGameObjects.push_back(tempGameObject);

    //saveGame();
}


void GameEngine::HandleInput()
{
    mRenderwindow->mCurrentCamera->setSpeed(0.f);  //cancel last frame movement
    float cameraSpeed = mRenderwindow->getCameraSpeed();
    Camera *currentCamera = mRenderwindow->mCurrentCamera;
    float camSpeedMultiplyer{1.2f};

    UpdateGameCameraFollow();

    if(mInput.F && !bIsPlaying)
    {
        int tempListIndex = mMainWindow->getObjectListIndex();
        gsl::Vector3D movePos {0,
                    mRenderwindow->mGameObjects.at(tempListIndex)->mCollisionComp->mRaidus*3,
                    mRenderwindow->mGameObjects.at(tempListIndex)->mCollisionComp->mRaidus*3};
        mEditorCamera->setPosition(mRenderwindow->mGameObjects.at(tempListIndex)->mTransformComp->mMatrix.getPosition()+movePos);
        mEditorCamera->setYaw(0);
        mEditorCamera->setPitch(-45.f);
        mEditorCamera->update();
        mInput.F = false;
    }

    if(mInput.RMB)
    {
        if(mInput.W)
            currentCamera->setSpeed(-cameraSpeed);
        if(mInput.S)
            currentCamera->setSpeed(cameraSpeed);
        if(mInput.D)
            currentCamera->moveRight(cameraSpeed);
        if(mInput.A)
            currentCamera->moveRight(-cameraSpeed);
        if(mInput.Q)
            currentCamera->updateHeigth(-cameraSpeed);
        if(mInput.E)
            currentCamera->updateHeigth(cameraSpeed);
    }
    if(bIsPlaying)
    {
        //qDebug() << "nr of obj: " << mRenderwindow->mGameObjects.size();
        QVector3D currentPos{mPlayer->mTransformComp->mMatrix.getPosition().x,
                    mPlayer->mTransformComp->mMatrix.getPosition().y,
                    mPlayer->mTransformComp->mMatrix.getPosition().z};

        if(mInput.W)
        {
            QVector3D nextPos{currentPos.x() + currentCamera->getFowrardVector().x*cameraSpeed*camSpeedMultiplyer,
                        0,
                        currentPos.z() + currentCamera->getFowrardVector().z*cameraSpeed*camSpeedMultiplyer};

            QVector3D nextVec = nextPos - currentPos;

            if(CollisionSystem::isColliding(mRenderwindow->mGameObjects, nextPos))
            {
                if(CollisionSystem::isColliding(mRenderwindow->mGameObjects, currentPos))
                {
                    mPlayer->mTransformComp->mMatrix.translateZ(nextVec.z());
                }else
                {
                    mPlayer->mTransformComp->mMatrix.translateX(nextVec.x());
                }
            }else
            {
                mPlayer->mTransformComp->mMatrix.translateX(currentCamera->getFowrardVector().x*cameraSpeed*camSpeedMultiplyer);
                mPlayer->mTransformComp->mMatrix.translateZ(currentCamera->getFowrardVector().z*cameraSpeed*camSpeedMultiplyer);
            }
        }
        if(mInput.S)
        {
            QVector3D nextPos{currentPos.x() - currentCamera->getFowrardVector().x*cameraSpeed*camSpeedMultiplyer,
                        0,
                        currentPos.z() - currentCamera->getFowrardVector().z*cameraSpeed*camSpeedMultiplyer};

            QVector3D nextVec = nextPos - currentPos;

            if(CollisionSystem::isColliding(mRenderwindow->mGameObjects, nextPos))
            {
                if(CollisionSystem::isColliding(mRenderwindow->mGameObjects, currentPos))
                {
                    mPlayer->mTransformComp->mMatrix.translateZ(nextVec.z());
                }else
                {
                    mPlayer->mTransformComp->mMatrix.translateX(nextVec.x());
                }
            }else
            {
                mPlayer->mTransformComp->mMatrix.translateX(-currentCamera->getFowrardVector().x*cameraSpeed*camSpeedMultiplyer);
                mPlayer->mTransformComp->mMatrix.translateZ(-currentCamera->getFowrardVector().z*cameraSpeed*camSpeedMultiplyer);
            }
        }
        if(mInput.D)
        {
            QVector3D nextPos{currentPos.x() + currentCamera->getRightVector().x*cameraSpeed*camSpeedMultiplyer,
                        0,
                        currentPos.z() + currentCamera->getRightVector().z*cameraSpeed*camSpeedMultiplyer};

            QVector3D nextVec = nextPos - currentPos;

            if(CollisionSystem::isColliding(mRenderwindow->mGameObjects, nextPos))
            {
                if(CollisionSystem::isColliding(mRenderwindow->mGameObjects, currentPos))
                {
                    mPlayer->mTransformComp->mMatrix.translateZ(nextVec.z());
                }else
                {
                    mPlayer->mTransformComp->mMatrix.translateX(nextVec.x());
                }
            }else
            {
                mPlayer->mTransformComp->mMatrix.translateX(currentCamera->getRightVector().x*cameraSpeed*camSpeedMultiplyer);
                mPlayer->mTransformComp->mMatrix.translateZ(currentCamera->getRightVector().z*cameraSpeed*camSpeedMultiplyer);
            }
        }
        if(mInput.A)
        {
            QVector3D nextPos{currentPos.x() - currentCamera->getRightVector().x*cameraSpeed*camSpeedMultiplyer,
                        0,
                        currentPos.z() - currentCamera->getRightVector().z*cameraSpeed*camSpeedMultiplyer};

            QVector3D nextVec = nextPos - currentPos;

            if(CollisionSystem::isColliding(mRenderwindow->mGameObjects, nextPos))
            {
                if(CollisionSystem::isColliding(mRenderwindow->mGameObjects, currentPos))
                {
                    mPlayer->mTransformComp->mMatrix.translateZ(nextVec.z());
                }else
                {
                    mPlayer->mTransformComp->mMatrix.translateX(nextVec.x());
                }
            }else
            {
                mPlayer->mTransformComp->mMatrix.translateX(-currentCamera->getRightVector().x*cameraSpeed*camSpeedMultiplyer);
                mPlayer->mTransformComp->mMatrix.translateZ(-currentCamera->getRightVector().z*cameraSpeed*camSpeedMultiplyer);
            }
        }
    }
}

void GameEngine::togglePlay(bool bInIsPlaying)
{
    playMusic(bInIsPlaying);
    bIsPlaying = bInIsPlaying;
    if(bInIsPlaying)
    {
        initPlayerPos = mPlayer->mTransformComp->mMatrix.getPosition();
        mRenderwindow->mCurrentCamera = mGameCamera;
    }else{

        mRenderwindow->mCurrentCamera = mEditorCamera;
        mPlayer->mTransformComp->mMatrix.setPosition(initPlayerPos.x,initPlayerPos.y,initPlayerPos.z);

//        mRenderwindow->mGameObjects.clear();
//        mMainWindow->clean();
//        SetUpScene();
    }
}

void GameEngine::UpdateGameCameraFollow()
{
    mGameCamera->setPosition(mPlayer->mTransformComp->mMatrix.getPosition() + gsl::Vector3D(0,2,0));
}

void GameEngine::rotateLight()
{
    static float rotate{0.f};
    mRenderwindow->mGameObjects[1]->mTransformComp->mMatrix.translate(sin(rotate)/6, 0, cos(rotate)/6);
    rotate+=0.01f;
}

void GameEngine::saveScene()
{
    mResourceManager->saveScene(mRenderwindow->mGameObjects);
}

void GameEngine::loadScene()
{
    mResourceManager->loadScene(mRenderwindow->mGameObjects,mPlayer, mLight);
}

void GameEngine::setRenderPointer(RenderWindow *temp, MainWindow *mainW)
{
    mRenderwindow = temp;
    mMainWindow = mainW;
}

void GameEngine::playMusic(bool bIsPlaying)
{
    if(bIsPlaying)
        mStereoSound->play();
    else
        mStereoSound->stop();
}

void GameEngine::resetWorld()
{
    //hmmm?
}

// For the four funcitons under: temporary set the shader to 2 for mouspicking testing
void GameEngine::CreateCube()
{
    tempGameObject = mResourceManager->CreateObject(gsl::MeshFilePath + "cube.obj");
    tempGameObject->mMaterialComp->mShaderProgram = 2;
    mRenderwindow->mGameObjects.push_back(tempGameObject);
}

void GameEngine::CreatePyramid()
{
    tempGameObject = mResourceManager->CreateObject(gsl::MeshFilePath + "pyramid.obj");
    tempGameObject->mMaterialComp->mShaderProgram = 2;
    mRenderwindow->mGameObjects.push_back(tempGameObject);
}

void GameEngine::CreateSphere()
{
    tempGameObject = mResourceManager->CreateObject(gsl::MeshFilePath + "sphere.obj", true);
    tempGameObject->mMaterialComp->mShaderProgram = 2;
    mRenderwindow->mGameObjects.push_back(tempGameObject);
}

void GameEngine::CreateSuzanne()
{
    tempGameObject = mResourceManager->CreateObject(gsl::MeshFilePath + "suzanne.obj", true);
    tempGameObject->mMaterialComp->mShaderProgram = 2;
    mRenderwindow->mGameObjects.push_back(tempGameObject);
}

GameEngine * GameEngine::getInstance()
{
    if(!mInstance)
    {
        mInstance = new GameEngine();
    }
    return mInstance;
}
