#include "coreengine.h"

#include "resourcemanager.h"
#include "soundsystem.h"

#include "gameobject.h"
#include "renderwindow.h"
#include "camera.h"

CoreEngine* CoreEngine::mInstance = nullptr;    //static pointer to instance

CoreEngine::CoreEngine(RenderWindow *renderWindowIn) : mRenderWindow{renderWindowIn}
{
    mResourceManager = &ResourceManager::getInstance();
    mSoundSystem = SoundSystem::getInstance();
    mInstance = this;
}

CoreEngine *CoreEngine::getInstance()
{
    return mInstance;
}

void CoreEngine::setUpScene()
{
    //********************** Making the object to be drawn **********************

    //Axis
    GameObject *temp = mResourceManager->addObject("axis");
    mRenderWindow->mGameObjects.push_back(temp);

    //dog triangle
    temp = mResourceManager->addObject("triangle");
    temp->mMaterial->mShaderProgram = 1;
    temp->mMaterial->mTextureUnit = 1;

    temp->mTransform->mMatrix.translate(0.f, 0.f, .5f);
    //Adds sound to moving triangle:
    mResourceManager->addComponent("caravan_mono.wav", temp);

    //Hack to test sound system
    temp->mSoundComponent->shouldPlay = true;

    mRenderWindow->mGameObjects.push_back(temp);

    //Suzanne:
    temp = mResourceManager->addObject("suzanne.obj");
    temp->mTransform->mMatrix.translate(1.f, 1.f, -.5f);
    temp->mTransform->mMatrix.scale(0.5f);
    mRenderWindow->mGameObjects.push_back(temp);

    //Suzanne 2 to check if resource handelig is correct:
    temp = mResourceManager->addObject("suzanne.obj");
    temp->mTransform->mMatrix.translate(-1.f, 1.f, -.5f);
    temp->mTransform->mMatrix.scale(0.3f);
    mRenderWindow->mGameObjects.push_back(temp);

    mCamera = new Camera();
    mCamera->mPosition = gsl::Vector3D(1.f, .5f, 4.f);
    mRenderWindow->mCurrentCamera = mCamera;
}


