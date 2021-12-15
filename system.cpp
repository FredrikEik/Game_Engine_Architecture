#include "system.h"
#include "camera.h"
#include "factory.h"
#include "skybox.h"
#include "mariocube.h"
#include "sphere.h"
#include "trianglesurface.h"
#include "player.h"
#include "light.h"
#include "rendersystem.h"
#include "texture.h"
#include "mainwindow.h"
#include <QOpenGLFunctions>

System::System(MainWindow *mw, RenderSystem *rs) : renderSystem(rs), mainWindow(mw)
{
    gameLoopTimer = new QTimer(this);
    init();
    isInitialized = true;
}
void System::init()
{
    //Create input
    input = new InputComponent();

    createMeshes();
    createObjects();
    createSounds();
    startGameLoop();
}

void System::startGameLoop()
{
    //Connect the gameloop timer to the render function:
    //This makes our render loop
    connect(gameLoopTimer, SIGNAL(timeout()), this, SLOT(gameLoop()));

    //This timer runs the actual MainLoop
    //16 means 16ms = 60 Frames pr second (should be 16.6666666 to be exact...)
    gameLoopTimer->start(8);
    timeStart.start();
}

void System::createMeshes()
{
    qDebug() << "Created meshes";
    renderSystem->factory->saveMesh("../GEA2021/Assets/Meshes/mariocube.obj", "MarioCube");
    renderSystem->factory->saveMesh("../GEA2021/Assets/Meshes/sphere.obj", "Sphere");
    renderSystem->factory->saveMesh("../GEA2021/Assets/skybox.obj", "Skybox");
    renderSystem->factory->saveMesh("../GEA2021/Assets/Meshes/wario.obj", "Wario");
}
void System::createObjects()
{
    //renderSystem->gameObjects.push_back(factory->createObject("Cube"));

}

//void System::initTextures()
//{
//    //Initializing textures in system.cpp not working therefore we just make rendersystem do it
//    renderSystem->initTextures();
//}

void System::createSounds()
{
    //********************** Sound set up **********************
    SoundManager::getInstance()->init();

    clickSound = SoundManager::getInstance()->createSource(
                "Click", Vector3(10.0f, 0.0f, 0.0f),
                "../GEA2021/Assets/Sounds/click.wav", false, 1.0f);
    /*mMario = SoundManager::getInstance()->createSource(
                "Mario", Vector3(10.0f, 0.0f, 0.0f),
                "../GEA2021/Assets/Sounds/mario.wav", false, 1.0f);*/
    videoGameLandSound = SoundManager::getInstance()->createSource(
                "VideoGameLand", Vector3(10.0f, 0.0f, 0.0f),
                "../GEA2021/Assets/Sounds/videogameland.wav", false, 0.2f);
    videoGameLandSound2 = SoundManager::getInstance()->createSource(
                "VideoGameLand2", Vector3(10.0f, 0.0f, 0.0f),
                "../GEA2021/Assets/Sounds/videogameland2.wav", false, 1.0f);

    //mVideoGameLand->play();
    //mVideoGameLand2->play();

    //mMario->play(); //doesnt work
    //mExplosionSound->play();
    //mExplosionSound->setPosition(Vector3(200.0f, 30.0f, -1000.0f));
}

void System::gameLoop()
{
    //Keyboard / mouse input
    handleInput();

    //Code game logic here
    //...

    //Render scene each tick
    if(renderSystem)
    {
        renderSystem->render();
    }
    //Restart FPS clock
    timeStart.restart();
}
void System::handleInput()
{
    //Camera
    renderSystem->editorCamera->setSpeed(0.f);  //cancel last frame movement
    renderSystem->playCamera->setSpeed(0.f);
    if(editorMode)
    {
        if(input->RMB) //editor
        {
            if(input->W)
                renderSystem->editorCamera->setSpeed(-cameraSpeed);
            if(input->S)
                renderSystem->editorCamera->setSpeed(cameraSpeed);
            if(input->D)
                renderSystem->editorCamera->moveRight(cameraSpeed);
            if(input->A)
                renderSystem->editorCamera->moveRight(-cameraSpeed);
            if(input->Q)
                renderSystem->editorCamera->updateHeigth(-cameraSpeed);
            if(input->E)
                renderSystem->editorCamera->updateHeigth(cameraSpeed);
        }
        if(renderSystem->skybox != nullptr)
        renderSystem->skybox->getTransformComponent()->mMatrix.setPosition(renderSystem->editorCamera->mPosition.x, renderSystem->editorCamera->mPosition.y, renderSystem->editorCamera->mPosition.z);
    }

    else if(!editorMode)
    {
      static_cast<Player*>(renderSystem->player)->movement(input);
      renderSystem->HUD->getMaterialComponent()->mTextureUnit = 5 + renderSystem->playerHP;
      renderSystem->skybox->getTransformComponent()->mMatrix.setPosition(renderSystem->playCamera->mPosition.x, renderSystem->playCamera->mPosition.y, renderSystem->playCamera->mPosition.z);
    }

}

void System::setCameraSpeed(float value)
{
    cameraSpeed += value;

    //Keep within some min and max values
    if(cameraSpeed < 0.01f)
        cameraSpeed = 0.01f;
    if (cameraSpeed > 0.3f)
        cameraSpeed = 0.3f;
}

void System::saveLevel()
{
    std::multimap<gsl::ObjectType, struct SpawnSettings> objectMap;
    for(unsigned int i = 0; i < renderSystem->gameObjects.size(); i++)
    {
        SpawnSettings settings;
        gsl::ObjectType objectType = renderSystem->gameObjects[i]->mObjectType;
        gsl::Matrix4x4 m = renderSystem->gameObjects[i]->getTransformComponent()->mMatrix;
        settings.initialPos =  m.getPosition();
        settings.initialScale = m.getScale();
        settings.initialRot = m.getRotation();
        objectMap.insert(std::pair<gsl::ObjectType, struct SpawnSettings>(objectType, settings));
    }
    level.saveLevelAs("savedLevel", objectMap);
}

void System::loadLevel()
{
    resetLevel();
    level.loadLevel("../GEA2021/Saves/savedLevel.json");
    renderSystem->factory->openLevel(level);
    mainWindow->updateOutliner(renderSystem->gameObjects);
}

void System::clearLevel()
{
    while(!renderSystem->gameObjects.empty())
    {
        renderSystem->gameObjects.pop_back();

    }
    for (uint32_t ID = 0; ID < gsl::MAX_OBJECTS; ++ID)
    {
        renderSystem->factory->mAvailableIDs.push(ID);
    }
    renderSystem->gameObjects.clear();
    renderSystem->factory->cameracounter = 0;
    renderSystem->factory->cubecounter = 0;
    renderSystem->factory->lightCounter = 0;
    renderSystem->factory->skyboxcounter = 0;
    renderSystem->factory->mariocounter = 0;
    renderSystem->factory->spherecounter = 0;
    renderSystem->factory->trianglecounter = 0;
}

void System::resetLevel()
{
    renderSystem->format();
    clearLevel();
    renderSystem->playerHP = 3;
    renderSystem->initObjects();
    mainWindow->updateOutliner(renderSystem->gameObjects);

    //    gameObjects.clear();
    //    gameLoopTimer->stop();


    //    //Make the gameloop timer:
    //    gameLoopTimer = new QTimer(this);

    //    mMainWindow->init();
    //    System::init();

    //    mRenderTimer->start(16);    //starts the timer
}

void System::toggleEditorMode()
{
    //Toggles editor mode
    editorMode = !editorMode;

    if(editorMode)
    {
        videoGameLandSound->stop();
        resetLevel();
    }
    else
    {
        videoGameLandSound->play();

//        if (mIndexToPickedObject > -1)
//        {
//            helperObject->getTransformComponent()->mMatrix.setScale(1,1,1);
//            hjelpeObjektMesh->mDrawType = GL_TRIANGLES;
//            hjelpeObjekt->setMeshComponent(hjelpeObjektMesh);
//            mIndexToPickedObject = -1;
//        }

    }
}

bool System::isColliding(BoxCollisionComponent &Box1, BoxCollisionComponent &Box2)
{
    return ((Box1.max.getX() > Box2.min.getX()) &&
            (Box1.min.getX() < Box2.max.getX()) &&
            (Box1.max.getY() > Box2.min.getY()) &&
            (Box1.min.getY() < Box2.max.getY()) &&
            (Box1.max.getZ() > Box2.min.getZ()) &&
            (Box1.min.getZ() < Box2.max.getZ()));
}

bool System::isColliding(BoxCollisionComponent &Box, SphereCollisionComponent &Sphere)
{
    gsl::Vector3D closestPointInBox = gsl::V3Dmin(gsl::V3Dmax(Sphere.center, Box.min),Box.max);
    gsl::Vector3D distanceVector = closestPointInBox - Sphere.center;

    double distanceSquared = sqrt(pow(distanceVector.getX(), 2) + pow(distanceVector.getY(), 2) + pow(distanceVector.getZ(), 2));

    return (distanceSquared < pow(Sphere.radius,2));
}

bool System::isCollidingWithFrustum(SphereCollisionComponent &Sphere)
{
    Camera* cullCamera = renderSystem->editorCamera;
    gsl::Vector3D rightPlaneToObjectVector = cullCamera->nearPlaneBottomRight - Sphere.center;
    float rightPlaneHeightToObject = gsl::Vector3D::dot(rightPlaneToObjectVector, cullCamera->rightPlaneNormal);
    if(rightPlaneHeightToObject + Sphere.radius >= 0)
    {
        gsl::Vector3D leftPlaneToObjectVector = cullCamera->nearPlaneTopLeft - Sphere.center;
        float leftPlaneHeightToObject = gsl::Vector3D::dot(leftPlaneToObjectVector, cullCamera->leftPlaneNormal);
        if(leftPlaneHeightToObject + Sphere.radius >= 0)
        {
            gsl::Vector3D nearPlaneToObjectVector = cullCamera->nearPlaneBottomRight - Sphere.center;
            float nearPlaneHeightToObject = gsl::Vector3D::dot(nearPlaneToObjectVector, cullCamera->nearPlaneNormal);
            if(nearPlaneHeightToObject + Sphere.radius >= 0)
            {
                gsl::Vector3D farPlaneToObjectVector = cullCamera->farPlaneBottomLeft - Sphere.center;
                float farPlaneHeightToObject = gsl::Vector3D::dot(farPlaneToObjectVector, cullCamera->farPlaneNormal);
                if(farPlaneHeightToObject + Sphere.radius >= 0)
                {
//                    gsl::Vector3D topPlaneToObjectVector = playCamera->nearPlaneTopRight - Sphere.center;
//                    float topPlaneHeightToObject = gsl::Vector3D::dot(topPlaneToObjectVector, playCamera->topPlaneNormal);
//                    if(topPlaneHeightToObject + Sphere.radius >= 0)
//                    {
//                        gsl::Vector3D bottomPlaneToObjectVector = playCamera->nearPlaneBottomLeft - Sphere.center;
//                        float bottomPlaneHeightToObject = gsl::Vector3D::dot(bottomPlaneToObjectVector, playCamera->bottomPlaneNormal);
//                        if(bottomPlaneHeightToObject + Sphere.radius >= 0)
//                        {
                            // qDebug() << "Object inside frustum";
                            return true;
//                        }
//                    }
                }
            }
        }
    }
    return false;
}


