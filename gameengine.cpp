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

    mEditorCamera = new Camera();
    mEditorCamera->setPosition(gsl::Vector3D(1.f, .5f, 4.f));
    mRenderwindow->mCurrentCamera = mEditorCamera;

//    for(int i{0}; i < 10; i++)
//    {
//        for(int j{0}; j < 10; j++)
//        {
//            tempGameObject = mResourceManager->CreateObject(gsl::MeshFilePath + "suzanne.obj", true);
//            tempGameObject->mTransformComp->mMatrix.translate(2.f*(i*2), -3.f, -2.f*(j*2));
//            mRenderwindow->mGameObjects.push_back(tempGameObject);
//        }
//    }

    tempGameObject = mResourceManager->CreateObject(gsl::MeshFilePath + "suzanne.obj", true);
    tempGameObject->mTransformComp->mMatrix.translate(3,0,-1);
    tempGameObject->mMaterialComp->mTextureUnit = 0;
    tempGameObject->mMaterialComp->mShaderProgram = 0;
    mRenderwindow->mGameObjects.push_back(tempGameObject);

    tempGameObject = mResourceManager->CreateObject(gsl::MeshFilePath + "suzanne.obj", true);
    tempGameObject->mTransformComp->mMatrix.translate(6,0,0);
    tempGameObject->mMaterialComp->mTextureUnit = 0;
    tempGameObject->mMaterialComp->mShaderProgram = 0;
    // tempGameObject->mMeshComp->mDrawType=GL_LINES;
    mRenderwindow->mGameObjects.push_back(tempGameObject);

    tempGameObject = mResourceManager->CreateObject(gsl::MeshFilePath + "cube.obj");
    tempGameObject->mTransformComp->mMatrix.translate(-3,0,-1);
    tempGameObject->mMaterialComp->mTextureUnit = 0;
    tempGameObject->mMaterialComp->mShaderProgram = 0;
    mRenderwindow->mGameObjects.push_back(tempGameObject);

    tempGameObject = mResourceManager->CreateObject(gsl::MeshFilePath + "sphere.obj", true);
    tempGameObject->mTransformComp->mMatrix.translate(-6,0,0);
    tempGameObject->mMaterialComp->mTextureUnit = 0;
    tempGameObject->mMaterialComp->mShaderProgram = 0;
    mRenderwindow->mGameObjects.push_back(tempGameObject);

    tempGameObject = mResourceManager->CreateObject(gsl::MeshFilePath + "pyramid.obj");
    tempGameObject->mTransformComp->mMatrix.translate(-9,0,0);
    tempGameObject->mMaterialComp->mTextureUnit = 0;
    tempGameObject->mMaterialComp->mShaderProgram = 0;
    mRenderwindow->mGameObjects.push_back(tempGameObject);


    tempGameObject = mResourceManager->CreateObject(gsl::MeshFilePath + "cube.obj");
    tempGameObject->mTransformComp->mMatrix.translate(3,0,3);
    tempGameObject->mTransformComp->mMatrix.scale(0.1f);
    tempGameObject->mMaterialComp->mTextureUnit = 0;
    tempGameObject->mMaterialComp->mShaderProgram = 0;
    mRenderwindow->mGameObjects.push_back(tempGameObject);


    tempGameObject = mResourceManager->CreateObject("xyz");
    tempGameObject->mMeshComp->mDrawType = GL_LINES;
    tempGameObject->mMaterialComp->mTextureUnit = 0;
    tempGameObject->mMaterialComp->mShaderProgram = 0;
    mRenderwindow->mGameObjects.push_back(tempGameObject);


    //TODO SetupTextures


    // Sound startup stuff
    mStereoSound = SoundManager::getInstance()->createSource(
                "Stereo", gsl::Vector3D(0.0f, 0.0f, 0.0f),
                gsl::SoundFilePath + "funnyhaha.wav", false, 1.0f);

    //mStereoSound->play();

    //Gameloop
    connect(mGameLoopRenderTimer, SIGNAL(timeout()), this, SLOT(GameLoop()));
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

void GameEngine::playMusic(bool bIsPlaying)
{
    if(bIsPlaying)
        mStereoSound->play();
    else
        mStereoSound->stop();
}



GameEngine * GameEngine::getInstance()
{
    if(!mInstance)
    {
        mInstance = new GameEngine();
    }
    return mInstance;
}
