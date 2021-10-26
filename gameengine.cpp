#include "gameengine.h"

#include "resourcemanager.h"
#include "gameobject.h"
#include "renderwindow.h"
#include "soundmanager.h"
#include "camera.h"
#include "physicsballsystem.h"
#include "mainwindow.h"


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
    mGameCamera->setPosition(gsl::Vector3D(0,0,0));



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

    tempGameObject = mResourceManager->CreateObject(gsl::MeshFilePath + "player.obj");
    tempGameObject->mTransformComp->mMatrix.translate(0,0,0);
    tempGameObject->mMaterialComp->mShaderProgram = 2;
    mRenderwindow->mGameObjects.push_back(tempGameObject);



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






//    tempGameObject = mResourceManager->CreateObject("xyz");
//    tempGameObject->mMeshComp->mDrawType = GL_LINES;
//    tempGameObject->mMaterialComp->mTextureUnit = 0;
//    tempGameObject->mMaterialComp->mShaderProgram = 0;
//    mRenderwindow->mGameObjects.push_back(tempGameObject);





    // * * * * * Visualisering og simulering * * * * *

    mPhysicsBall = mResourceManager->CreateObject(gsl::MeshFilePath + "sphere.obj");
    mPhysicsBall->mTransformComp->mMatrix.translate(4.8f,-0.77f,-0.8f);
    mPhysicsBall->mTransformComp->mMatrix.scale(0.1);
    mPhysicsBall->mMaterialComp->mShaderProgram = 2;
    mRenderwindow->mGameObjects.push_back(mPhysicsBall);

    mTerrainObject = mResourceManager->CreateObject(gsl::MeshFilePath + "toTriangles.obj");
    mTerrainObject->mTransformComp->mMatrix.translate(4,-1,-1);
    mTerrainObject->mMaterialComp->mShaderProgram = 2;
    //mTerrainObject->mMaterialComp->mTextureUnit = 2;
    mRenderwindow->mGameObjects.push_back(mTerrainObject);

    mPhysicsBallSystem->SetTerrainData(*mTerrainObject);





//    tempGameObject = mResourceManager->CreateObject(gsl::MeshFilePath + "HamarHeightMap.obj");
//    tempGameObject->mTransformComp->mMatrix.translate(0,-15,0);
//    tempGameObject->mMaterialComp->mShaderProgram = 2;
//    mRenderwindow->mGameObjects.push_back(tempGameObject);
}

void GameEngine::HandleInput()
{

}

void GameEngine::togglePlay(bool bInIsPlaying)
{
    playMusic(bInIsPlaying);
    mRenderwindow->bIsPlaying = bInIsPlaying;
    if(bInIsPlaying)
    {
        mRenderwindow->mCurrentCamera = mGameCamera;
    }else
         mRenderwindow->mCurrentCamera = mEditorCamera;
}

void GameEngine::GameLoop()
{
    mPhysicsBallSystem->update(*mPhysicsBall);

    HandleInput();

    mEditorCamera->update();

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
