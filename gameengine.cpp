#include "gameengine.h"

#include "resourcemanager.h"
#include "gameobject.h"
#include "renderwindow.h"
#include "soundmanager.h"
#include "camera.h"


// Make Singelton
GameEngine* GameEngine::mInstance = nullptr;

//TODO: endre slik at constructoren blir kalt
GameEngine::GameEngine()
{
}

void GameEngine::SetUpScene()
{
    mResourceManager = new class ResourceManager();
    mInstance = this;
    mGameLoopRenderTimer = new QTimer(this);


    GameObject* tempGameObject = mResourceManager->CreateObject(gsl::MeshFilePath + "Suzanne.obj");
    mRenderwindow->mGameObjects.push_back(tempGameObject);

    tempGameObject = mResourceManager->CreateObject(gsl::MeshFilePath + "Suzanne.obj");
    tempGameObject->mTransformComp->mMatrix.translate(2,0,0);
    mRenderwindow->mGameObjects.push_back(tempGameObject);

    tempGameObject = mResourceManager->CreateObject(gsl::MeshFilePath + "Suzanne.obj");
    tempGameObject->mTransformComp->mMatrix.translate(4,0,0);
    mRenderwindow->mGameObjects.push_back(tempGameObject);

    tempGameObject = mResourceManager->CreateObject(gsl::MeshFilePath + "cube.obj");
    tempGameObject->mTransformComp->mMatrix.translate(-2,0,0);
    mRenderwindow->mGameObjects.push_back(tempGameObject);

    tempGameObject = mResourceManager->CreateObject(gsl::MeshFilePath + "sphere.obj");
    tempGameObject->mTransformComp->mMatrix.translate(-6,0,0);
    mRenderwindow->mGameObjects.push_back(tempGameObject);

    tempGameObject = mResourceManager->CreateObject(gsl::MeshFilePath + "pyramid.obj");
    tempGameObject->mTransformComp->mMatrix.translate(-9,0,0);
    mRenderwindow->mGameObjects.push_back(tempGameObject);

    //TODO SetupTextures


    // Sound startup stuff
    SoundSource* mStereoSound = SoundManager::getInstance()->createSource(
                "Stereo", gsl::Vector3D(0.0f, 0.0f, 0.0f),
                gsl::SoundFilePath + "funnyhaha.wav", false, 1.0f);

    //mStereoSound->play();

    //Gameloop
    connect(mGameLoopRenderTimer, SIGNAL(timeout()), this, SLOT(gameLoop()));
    mGameLoopRenderTimer->start(16);
}

void GameEngine::HandleInput()
{

}

void GameEngine::GameLoop()
{
    HandleInput();

    mEditorCamera->update();

    mRenderwindow->render();
}

void GameEngine::setRenderPointer(RenderWindow *temp)
{
    mRenderwindow = temp;
}

GameEngine * GameEngine::getInstance()
{
    if(!mInstance)
    {
        mInstance = new GameEngine();
    }
    return mInstance;
}
