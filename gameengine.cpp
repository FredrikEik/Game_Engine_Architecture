#include "gameengine.h"

#include <QKeyEvent>

#include "resourcemanager.h"
#include "gameobject.h"
#include "renderwindow.h"
#include "soundmanager.h"
#include "camera.h"
#include "physicsballsystem.h"
#include "mainwindow.h"
#include "collisionsystem.h"


// Make Singelton
GameEngine* GameEngine::mInstance = nullptr;

//TODO: endre slik at constructoren blir kalt
GameEngine::GameEngine()
{
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



    SetUpObjects();

    //TODO SetupTextures


    // Sound startup stuff
    mStereoSound = SoundManager::getInstance()->createSource(
                "Stereo", gsl::Vector3D(0.0f, 0.0f, 0.0f),
                gsl::SoundFilePath + "funnyhaha.wav", false, 1.0f);

    //mStereoSound->play();

    //Gameloop
    connect(mGameLoopRenderTimer, SIGNAL(timeout()), this, SLOT(GameLoop()));
    mGameLoopRenderTimer->start(16);

    mMainWindow->initList();
}



void GameEngine::SetUpObjects()
{

    mPlayer = mResourceManager->CreateObject(gsl::MeshFilePath + "player.obj");
    mPlayer->mTransformComp->mMatrix.translate(0,-1.8f,-15);
    mPlayer->mMaterialComp->mShaderProgram = 2;
    mRenderwindow->mGameObjects.push_back(mPlayer);



    //    for(int i{0}; i < 100; i++)
    //    {
    //        for(int j{0}; j < 100; j++)
    //        {
    //            tempGameObject = mResourceManager->CreateObject(gsl::MeshFilePath + "suzanne.obj", true);
    //            tempGameObject->mTransformComp->mMatrix.translate(2.f*(i), -3.f, -2.f*(j));
    //            mRenderwindow->mGameObjects.push_back(tempGameObject);
    //        }
    //    }

//    tempGameObject = mResourceManager->CreateObject(gsl::MeshFilePath + "cube.obj");
//    tempGameObject->mMaterialComp->mShaderProgram = 1;
//    tempGameObject->mMaterialComp->mTextureUnit = 1;
//    tempGameObject->mTransformComp->mMatrix.scale(0.2f);
//    mRenderwindow->mGameObjects.push_back(tempGameObject);



//    tempGameObject = mResourceManager->CreateObject(gsl::MeshFilePath + "suzanne.obj", true);
//    tempGameObject->mTransformComp->mMatrix.translate(4,0,-2);
//    mRenderwindow->mGameObjects.push_back(tempGameObject);

//    tempGameObject = mResourceManager->CreateObject(gsl::MeshFilePath + "suzanne.obj", true);
//    tempGameObject->mTransformComp->mMatrix.translate(2,0,-10);
//    // tempGameObject->mMeshComp->mDrawType=GL_LINES;
//    mRenderwindow->mGameObjects.push_back(tempGameObject);

    tempGameObject = mResourceManager->CreateObject(gsl::MeshFilePath + "cube.obj");
    tempGameObject->mTransformComp->mMatrix.translate(-1,-1,-5);
    tempGameObject->mMaterialComp->mShaderProgram = 2;
    mRenderwindow->mGameObjects.push_back(tempGameObject);

    tempGameObject = mResourceManager->CreateObject(gsl::MeshFilePath + "sphere.obj", true);
    tempGameObject->mTransformComp->mMatrix.translate(14,7,-27);
    tempGameObject->mMaterialComp->mShaderProgram = 2;
    mRenderwindow->mGameObjects.push_back(tempGameObject);

    tempGameObject = mResourceManager->CreateObject(gsl::MeshFilePath + "pyramid.obj");
    tempGameObject->mTransformComp->mMatrix.translate(-1,0.5,-5);
    tempGameObject->mMaterialComp->mShaderProgram = 2;
    mRenderwindow->mGameObjects.push_back(tempGameObject);

    tempGameObject = mResourceManager->CreateObject(gsl::MeshFilePath + "sphere.obj");
    tempGameObject->mTransformComp->mMatrix.translate(-5,2.5,0);
    tempGameObject->mMaterialComp->mShaderProgram = 2;
    mRenderwindow->mGameObjects.push_back(tempGameObject);

    tempGameObject = mResourceManager->CreateObject(gsl::MeshFilePath + "suzanne.obj", true);
    tempGameObject->mTransformComp->mMatrix.translate(-12,0.5,-13);
    tempGameObject->mMaterialComp->mShaderProgram = 2;
    mRenderwindow->mGameObjects.push_back(tempGameObject);






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
}

void GameEngine::HandleInput()
{
    mRenderwindow->mCurrentCamera->setSpeed(0.f);  //cancel last frame movement
    float cameraSpeed = mRenderwindow->getCameraSpeed();
    Camera *currentCamera = mRenderwindow->mCurrentCamera;
    float camSpeedMultiplyer{2};
    mPlayer->mCollisionComp->mVelocity;

    UpdateGameCameraFollow();

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
//        if(mInput.W && !CollisionSystem::isColliding(mRenderwindow->mGameObjects))
//        {
//            mPlayer->mTransformComp->mMatrix.translateX(mRenderwindow->mCurrentCamera->getFowrardVector().x*cameraSpeed*camSpeedMultiplyer);
//            mPlayer->mTransformComp->mMatrix.translateZ(currentCamera->getFowrardVector().z*cameraSpeed*camSpeedMultiplyer);
//        }
//        if(mInput.S /*&& !CollisionSystem::isColliding(mRenderwindow->mGameObjects)*/)
//        {
//            mPlayer->mTransformComp->mMatrix.translateX(-currentCamera->getFowrardVector().x*cameraSpeed*camSpeedMultiplyer);
//            mPlayer->mTransformComp->mMatrix.translateZ(-currentCamera->getFowrardVector().z*cameraSpeed*camSpeedMultiplyer);
//        }
//        if(mInput.D && !CollisionSystem::isColliding(mRenderwindow->mGameObjects))
//        {
//            mPlayer->mTransformComp->mMatrix.translateX(currentCamera->getRightVector().x*cameraSpeed*camSpeedMultiplyer);
//            mPlayer->mTransformComp->mMatrix.translateZ(currentCamera->getRightVector().z*cameraSpeed*camSpeedMultiplyer);
//        }
//        if(mInput.A && !CollisionSystem::isColliding(mRenderwindow->mGameObjects))
//        {
//            mPlayer->mTransformComp->mMatrix.translateX(-currentCamera->getRightVector().x*cameraSpeed*camSpeedMultiplyer);
//            mPlayer->mTransformComp->mMatrix.translateZ(-currentCamera->getRightVector().z*cameraSpeed*camSpeedMultiplyer);
//        }


        QVector3D currentPos{mPlayer->mTransformComp->mMatrix.getPosition().x,
                    mPlayer->mTransformComp->mMatrix.getPosition().y,
                    mPlayer->mTransformComp->mMatrix.getPosition().z};
        if(mInput.W /*&& !CollisionSystem::isColliding(mRenderwindow->mGameObjects)*/)
        {
            QVector3D nextPos{currentPos.x() + currentCamera->getFowrardVector().x*cameraSpeed*camSpeedMultiplyer,
                        0,
                        currentPos.z() + currentCamera->getFowrardVector().z*cameraSpeed*camSpeedMultiplyer};
            qDebug() << "Curpos: " << currentPos;
            qDebug() << "NextPose: " << nextPos;
//            mPlayer->mTransformComp->mMatrix.translateX(mRenderwindow->mCurrentCamera->getFowrardVector().x*cameraSpeed*camSpeedMultiplyer);
//            mPlayer->mTransformComp->mMatrix.translateZ(currentCamera->getFowrardVector().z*cameraSpeed*camSpeedMultiplyer);

            if(CollisionSystem::isColliding(mRenderwindow->mGameObjects, nextPos))
            {
                mPlayer->mCollisionComp->mVelocity.setX(0);
                mPlayer->mCollisionComp->mVelocity.setZ(0);
                mPlayer->mTransformComp->mMatrix.translateX(mPlayer->mCollisionComp->mVelocity.x());
                mPlayer->mTransformComp->mMatrix.translateZ(mPlayer->mCollisionComp->mVelocity.z());
            }else
            {
                mPlayer->mCollisionComp->mVelocity.setX(currentCamera->getFowrardVector().x*cameraSpeed*camSpeedMultiplyer);
                mPlayer->mCollisionComp->mVelocity.setZ(currentCamera->getFowrardVector().z*cameraSpeed*camSpeedMultiplyer);
                mPlayer->mTransformComp->mMatrix.translateX(mPlayer->mCollisionComp->mVelocity.x());
                mPlayer->mTransformComp->mMatrix.translateZ(mPlayer->mCollisionComp->mVelocity.z());
            }
        }
        if(mInput.S)
        {
            QVector3D nextPos{currentPos.x() - currentCamera->getFowrardVector().x*cameraSpeed*camSpeedMultiplyer,
                        0,
                        currentPos.z() - currentCamera->getFowrardVector().z*cameraSpeed*camSpeedMultiplyer};
            if(CollisionSystem::isColliding(mRenderwindow->mGameObjects, nextPos))
            {
                mPlayer->mCollisionComp->mVelocity.setX(0);
                mPlayer->mCollisionComp->mVelocity.setZ(0);
                mPlayer->mTransformComp->mMatrix.translateX(mPlayer->mCollisionComp->mVelocity.x());
                mPlayer->mTransformComp->mMatrix.translateZ(mPlayer->mCollisionComp->mVelocity.z());
            }else
            {
                mPlayer->mCollisionComp->mVelocity.setX(-currentCamera->getFowrardVector().x*cameraSpeed*camSpeedMultiplyer);
                mPlayer->mCollisionComp->mVelocity.setZ(-currentCamera->getFowrardVector().z*cameraSpeed*camSpeedMultiplyer);
                mPlayer->mTransformComp->mMatrix.translateX(mPlayer->mCollisionComp->mVelocity.x());
                mPlayer->mTransformComp->mMatrix.translateZ(mPlayer->mCollisionComp->mVelocity.z());
            }
        }
        if(mInput.D /*&& !CollisionSystem::isColliding(mRenderwindow->mGameObjects)*/)
        {
            QVector3D nextPos{currentPos.x() + currentCamera->getRightVector().x*cameraSpeed*camSpeedMultiplyer,
                        0,
                        currentPos.z() + currentCamera->getRightVector().z*cameraSpeed*camSpeedMultiplyer};
            if(CollisionSystem::isColliding(mRenderwindow->mGameObjects, nextPos))
            {
                mPlayer->mCollisionComp->mVelocity.setX(0);
                mPlayer->mCollisionComp->mVelocity.setZ(0);
                mPlayer->mTransformComp->mMatrix.translateX(mPlayer->mCollisionComp->mVelocity.x());
                mPlayer->mTransformComp->mMatrix.translateZ(mPlayer->mCollisionComp->mVelocity.z());
            }else
            {
                mPlayer->mCollisionComp->mVelocity.setX(currentCamera->getRightVector().x*cameraSpeed*camSpeedMultiplyer);
                mPlayer->mCollisionComp->mVelocity.setZ(currentCamera->getRightVector().z*cameraSpeed*camSpeedMultiplyer);
                mPlayer->mTransformComp->mMatrix.translateX(mPlayer->mCollisionComp->mVelocity.x());
                mPlayer->mTransformComp->mMatrix.translateZ(mPlayer->mCollisionComp->mVelocity.z());
            }
        }
        if(mInput.A /*&& !CollisionSystem::isColliding(mRenderwindow->mGameObjects)*/)
        {
            QVector3D nextPos{currentPos.x() - currentCamera->getRightVector().x*cameraSpeed*camSpeedMultiplyer,
                        0,
                        currentPos.z() - currentCamera->getRightVector().z*cameraSpeed*camSpeedMultiplyer};
            if(CollisionSystem::isColliding(mRenderwindow->mGameObjects, nextPos))
            {
                mPlayer->mCollisionComp->mVelocity.setX(0);
                mPlayer->mCollisionComp->mVelocity.setZ(0);
                mPlayer->mTransformComp->mMatrix.translateX(mPlayer->mCollisionComp->mVelocity.x());
                mPlayer->mTransformComp->mMatrix.translateZ(mPlayer->mCollisionComp->mVelocity.z());
            }else
            {
                mPlayer->mCollisionComp->mVelocity.setX(-currentCamera->getRightVector().x*cameraSpeed*camSpeedMultiplyer);
                mPlayer->mCollisionComp->mVelocity.setZ(-currentCamera->getRightVector().z*cameraSpeed*camSpeedMultiplyer);
                mPlayer->mTransformComp->mMatrix.translateX(mPlayer->mCollisionComp->mVelocity.x());
                mPlayer->mTransformComp->mMatrix.translateZ(mPlayer->mCollisionComp->mVelocity.z());
            }
        }

    }
   // qDebug() << mPlayer->mCollisionComp->mVelocity;
}

void GameEngine::togglePlay(bool bInIsPlaying)
{
    playMusic(bInIsPlaying);
    bIsPlaying = bInIsPlaying;
    if(bInIsPlaying)
    {
        mRenderwindow->mCurrentCamera = mGameCamera;
    }else
         mRenderwindow->mCurrentCamera = mEditorCamera;
}

void GameEngine::UpdateGameCameraFollow()
{
    mGameCamera->setPosition(mPlayer->mTransformComp->mMatrix.getPosition() + gsl::Vector3D(0,2,0));
}

void GameEngine::GameLoop()
{
    //mPhysicsBallSystem->update(*mPhysicsBall);

    HandleInput();

    //mCurrentCameraUPDATE??
    mEditorCamera->update();
    mGameCamera->update();

    mRenderwindow->render();

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
