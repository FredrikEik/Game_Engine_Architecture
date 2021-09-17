#include "gameengine.h"
#include "resourcemanager.h"
#include "gameobject.h"
#include "renderwindow.h"


// Make Singelton
GameEngine* GameEngine::mInstance = NULL;

GameEngine::GameEngine()
{

}

void GameEngine::Coreinit()
{
    mResourceManager = new class ResourceManager();
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
}

void GameEngine::setRenderPointer(RenderWindow *temp)
{
    mRenderwindow = temp;
}
