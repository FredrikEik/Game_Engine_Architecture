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
    if(bBallPhysicsIsPlaying && mPhysicsBall)
        mPhysicsBallSystem->update(*mPhysicsBall);

    rotateLight();
    //m3DTestSound->setPosition(mRenderwindow->mGameObjects[2]->mTransformComp->mMatrix.getPosition());
    mStereoSound->setPosition(gsl::Vector3D(0,0,0));
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
    SoundManager::getInstance()->init();
    mInstance = this;
    mGameLoopRenderTimer = new QTimer(this);

    mEditorCamera = new Camera();
    mEditorCamera->setPosition(gsl::Vector3D(7.f, 7.f, 7.f));
//        mEditorCamera->setPosition(gsl::Vector3D(450.f, 70.f, 680.f));
//            mEditorCamera->setPosition(gsl::Vector3D(500.f, 200.f, 500.f));
   mEditorCamera->yaw(120);
    mEditorCamera->pitch(30);
    mRenderwindow->mCurrentCamera = mEditorCamera;

    mGameCamera = new Camera();
    mGameCamera->setPosition(gsl::Vector3D(0,2,0));
    mGameCamera->yaw(180);

    mResourceManager->setUpAllTextures();
    mResourceManager->getAllMeshNames();
    mResourceManager->getAllLevelNames();


    //mPhysicsBallSystem->readFile(gsl::ProjectFolderName + "test_las.txt");


    SetUpObjects();

//    std::string levelName("DefaultLevel");
//    loadScene(levelName);

    // Sound startup stuff
    mStereoSound = SoundManager::getInstance()->createSource(
                "dog", gsl::Vector3D(0.0f, 0.0f, 0.0f),
                gsl::SoundFilePath + "rock_background.wav", false, 1.0f);

    m3DTestSound = SoundManager::getInstance()->createSource(
                "Stereo", gsl::Vector3D(0.0f, 0.0f, 0.0f),
                gsl::SoundFilePath + "rock_background_mono.wav", false, 1.0f);

    //m3DTestSound->play();
//    mStereoSound->play();

    //Gameloop
    connect(mGameLoopRenderTimer, SIGNAL(timeout()), this, SLOT(GameLoop()));
    mGameLoopRenderTimer->start(7);// original value 16, 7 is for 144 hz

    mMainWindow->initList();
    mMainWindow->initComboboxTexture(mResourceManager->mTextureNames);
    mMainWindow->initComboboxMeshes(mResourceManager->mMeshNames);
    mMainWindow->initComboboxLevels(mResourceManager->mLevelNames);

}

void GameEngine::SetUpObjects()
{
    mXYZaxis = mResourceManager->CreateObject("xyz",false,"plain");
    mRenderwindow->mGameObjects.push_back(mXYZaxis);



    mPlayer = mResourceManager->CreateObject(gsl::MeshFilePath + "player.obj",false,"plain");
    mPlayer->mTransformComp->mMatrix.setPosition(0,0,-8);
    initPlayerPos = {0,0.f,-8};
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


    mLight = mResourceManager->CreateObject(gsl::MeshFilePath + "light.obj",false);
        mLight->mTransformComp->mMatrix.setPosition(50,30,111);
                //mLight->mTransformComp->mMatrix.setPosition(5,5,5);
//    mLight->mTransformComp->mMatrix.setPosition(-20,3,-10);
//    mLight->mTransformComp->mMatrix.setPosition(500,300,500);
    mLight->mMaterialComp->mShaderProgram = 2;
//    mLight->mMaterialComp->mTextureUnit = 1;
    mRenderwindow->mGameObjects.push_back(mLight);




    // TERRAIN:
    mTerrainObject = mResourceManager->CreateObject(gsl::ProjectFolderName + "test_las.txt",false,"plain");
//    mTerrainObject->mTransformComp->mMatrix.setPosition(-500.f,-60.f,-500.f);
    mTerrainObject->mMaterialComp->mShaderProgram = 2;
    mTerrainObject->mMaterialComp->mTextureUnit = 0;
//    mTerrainObject->mTransformComp->mMatrix.scale(1,0.5f,1);
    mRenderwindow->mGameObjects.push_back(mTerrainObject);

    mHeightCurveObject = mResourceManager->CreateObject("HeightCurve",false,"plain");
    mHeightCurveObject->mMaterialComp->mShaderProgram = 0;
    mHeightCurveObject->mMaterialComp->mTextureUnit = 0;
//    mHeightCurveObject->mTransformComp->mMatrix.scale(1,0.5f,1);
    mHeightCurveObject->mTransformComp->mMatrix.translateY(0.f);
    mRenderwindow->mGameObjects.push_back(mHeightCurveObject);

    // Ball-Physics:

    mPhysicsBall = mResourceManager->CreateObject(gsl::MeshFilePath + "sphere.obj", false, "blue.bmp");
    mPhysicsBall->mTransformComp->mMatrix.translate(552.f,23.f,601.f);
    mPhysicsBall->mTransformComp->mMatrix.scale(1);
    mPhysicsBall->mMaterialComp->mShaderProgram = 2;
    mRenderwindow->mGameObjects.push_back(mPhysicsBall);

    mPhysicsBallSystem->SetTerrainData(*mTerrainObject);

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

            if(CollisionSystem::isColliding(mPlayer, mRenderwindow->mGameObjects, nextPos))
            {
                if(CollisionSystem::isColliding(mPlayer, mRenderwindow->mGameObjects, currentPos))
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

            if(CollisionSystem::isColliding(mPlayer, mRenderwindow->mGameObjects, nextPos))
            {
                if(CollisionSystem::isColliding(mPlayer, mRenderwindow->mGameObjects, currentPos))
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

            if(CollisionSystem::isColliding(mPlayer, mRenderwindow->mGameObjects, nextPos))
            {
                if(CollisionSystem::isColliding(mPlayer, mRenderwindow->mGameObjects, currentPos))
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

            if(CollisionSystem::isColliding(mPlayer, mRenderwindow->mGameObjects, nextPos))
            {
                if(CollisionSystem::isColliding(mPlayer, mRenderwindow->mGameObjects, currentPos))
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
//    mRenderwindow->mGameObjects[2]->mTransformComp->mMatrix.translate(sin(rotate)/6, 0, cos(rotate)/6);

    //mRenderwindow->mGameObjects[2]->mTransformComp->mMatrix.translate(sin(rotate)*10, 0, cos(rotate)*10);
    rotate+=0.01f;
}

void GameEngine::saveScene(std::string &levelName)
{
    mResourceManager->saveScene(mRenderwindow->mGameObjects, levelName);
}

void GameEngine::loadScene(std::string &levelName)
{
    mResourceManager->loadScene(mRenderwindow->mGameObjects,mPlayer, mLight,levelName);
}

void GameEngine::updateAllLevels()
{
    mResourceManager->getAllLevelNames();
    mMainWindow->updateComboboxLevels(mResourceManager->mLevelNames);
}

void GameEngine::ResetBallVelocity()
{
    mPhysicsBallSystem->resetBall();
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
    tempGameObject = mResourceManager->CreateObject(gsl::MeshFilePath + "cube.obj", false);
    tempGameObject->mMaterialComp->mShaderProgram = 2;
    mRenderwindow->mGameObjects.push_back(tempGameObject);
}

void GameEngine::CreatePyramid()
{
    tempGameObject = mResourceManager->CreateObject(gsl::MeshFilePath + "pyramid.obj",false);
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

void GameEngine::CreateObject(std::string filePath, bool bUsingLOD, std::string textureName)
{
    tempGameObject = mResourceManager->CreateObject(filePath, bUsingLOD, textureName);
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
