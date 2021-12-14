#include "system.h"
#include "camera.h"
#include "factory.h"
#include "skybox.h"
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
}
void System::init()
{
    //initializeOpenGLFunctions();
    initMeshes();
    //initTextures();
    initSounds();
    initObjects();

    //Connect the gameloop timer to the render function:
    //This makes our render loop
    connect(gameLoopTimer, SIGNAL(timeout()), this, SLOT(gameLoop()));

    //This timer runs the actual MainLoop
    //16 means 16ms = 60 Frames pr second (should be 16.6666666 to be exact...)
    gameLoopTimer->start(16);
    timeStart.start();
}

void System::initMeshes()
{
    factory = new Factory();
    factory->saveMesh("../GEA2021/Assets/Meshes/mariocube.obj", "MarioCube");
    factory->saveMesh("../GEA2021/Assets/Meshes/sphere.obj", "Sphere");
    factory->saveMesh("../GEA2021/Assets/skybox.obj", "Skybox");
    factory->saveMesh("../GEA2021/Assets/Meshes/wario.obj", "Wario");
}
void System::initObjects()
{
    //Skybox
    skybox = static_cast<Skybox*>(factory->createObject("Skybox"));
    skybox->getTransformComponent()->mMatrix.setRotation(-180, 0, 0);
    skybox->getTransformComponent()->mMatrix.setScale(50,50,50);
    gameObjects.push_back(skybox);

    //Surface
    triangleSurface = static_cast<TriangleSurface*>(factory->createObject("TriangleSurface"));
    gameObjects.push_back(triangleSurface);

    //Player
    player = static_cast<Player*>(factory->createObject("Player"));
    player->getTransformComponent()->mMatrix.setScale(0.1f,0.1f,0.1f);
    player->getTransformComponent()->mMatrix.setPosition(0.f,0.6f,0.f);
    dynamic_cast<Player*>(player)->setSurfaceToWalkOn(triangleSurface);
    gameObjects.push_back(player);

    //Lights
    Light* light1 = static_cast<Light*>(factory->createObject("Light"));
    Light* light2 = static_cast<Light*>(factory->createObject("Light"));
    gameObjects.push_back(light1);
    gameObjects.push_back(light2);
    renderSystem->mShaderHandler->lightRefs.push_back(light1);
    renderSystem->mShaderHandler->lightRefs.push_back(light2);

    //Editor camera
    editorCamera = new Camera(90, 4/3);
    editorCamera->init();
    editorCamera->setPosition(gsl::Vector3D(0.f, 0.f, 0.f));
    editorCamera->mProjectionMatrix.perspective(45.f, renderSystem->getAspectRatio(), 0.1f, 100.f);
    renderSystem->mShaderHandler->cameraRef = editorCamera;

    //Play camera
    playCamera = new Camera(90, 4/3);
    playCamera->init();
    playCamera->setPosition(player->cameraTarget);

    //Helper Object
    helperObject = factory->createObject("Cube");

    mainWindow->updateOutliner(gameObjects);
}

void System::initTextures()
{
    //Initializing textures in system.cpp not working therefore we just make rendersystem do it
    renderSystem->initTextures();
}

void System::initSounds()
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
    editorCamera->setSpeed(0.f);  //cancel last frame movement
    playCamera->setSpeed(0.f);
    if(editorMode)
    {
        if(input->RMB) //editor
        {
            if(input->W)
                editorCamera->setSpeed(-cameraSpeed);
            if(input->S)
                editorCamera->setSpeed(cameraSpeed);
            if(input->D)
                editorCamera->moveRight(cameraSpeed);
            if(input->A)
                editorCamera->moveRight(-cameraSpeed);
            if(input->Q)
                editorCamera->updateHeigth(-cameraSpeed);
            if(input->E)
                editorCamera->updateHeigth(cameraSpeed);
        }
        skybox->getTransformComponent()->mMatrix.setPosition(editorCamera->mPosition.x, editorCamera->mPosition.y, editorCamera->mPosition.z);
    }

    else if(!editorMode) //karakter shit her
    {
      static_cast<Player*>(player)->movement(input);
      skybox->getTransformComponent()->mMatrix.setPosition(playCamera->mPosition.x, playCamera->mPosition.y, playCamera->mPosition.z);
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

void System::createObject(std::string objectName)
{
    GameObject* newObject = factory->createObject(objectName);
    gameObjects.push_back(newObject);
}

void System::saveLevel()
{
    std::multimap<std::string, struct SpawnSettings> objectMap;
    for(unsigned int i = 0; i < gameObjects.size(); i++)
    {
        SpawnSettings settings;
        std::string objectType = gameObjects[i]->mObjectType;
        gsl::Matrix4x4 m = gameObjects[i]->getTransformComponent()->mMatrix;
        settings.initialPos =  m.getPosition();
        settings.initialScale = m.getScale();
        settings.initialRot = m.getRotation();
        objectMap.insert(std::pair<std::string, struct SpawnSettings>(objectType, settings));
    }
    level.saveLevelAs("savedLevel", objectMap);
}

void System::loadLevel()
{
    clearLevel();
    resetLevel();
    level.loadLevel("../GEA2021/Saves/savedLevel.json");
    factory->openLevel(level);
    mainWindow->updateOutliner(gameObjects);
}

void System::clearLevel()
{
    while(!gameObjects.empty())
    {
        gameObjects.pop_back();

    }
    for (uint32_t ID = 0; ID < gsl::MAX_OBJECTS; ++ID)
    {
        factory->mAvailableIDs.push(ID);
    }
    gameObjects.clear();
    factory->cameracounter = 0;
    factory->cubecounter = 0;
    factory->lightCounter = 0;
    factory->skyboxcounter = 0;
    factory->mariocounter = 0;
    factory->spherecounter = 0;
    factory->trianglecounter = 0;
}

void System::resetLevel()
{
    clearLevel();
    initObjects();

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
    gsl::Vector3D rightPlaneToObjectVector = playCamera->nearPlaneBottomRight - Sphere.center;
    float rightPlaneHeightToObject = gsl::Vector3D::dot(rightPlaneToObjectVector, playCamera->rightPlaneNormal);
    if(rightPlaneHeightToObject + Sphere.radius >= 0)
    {
        gsl::Vector3D leftPlaneToObjectVector = playCamera->nearPlaneTopLeft - Sphere.center;
        float leftPlaneHeightToObject = gsl::Vector3D::dot(leftPlaneToObjectVector, playCamera->leftPlaneNormal);
        if(leftPlaneHeightToObject + Sphere.radius >= 0)
        {
            gsl::Vector3D nearPlaneToObjectVector = playCamera->nearPlaneBottomRight - Sphere.center;
            float nearPlaneHeightToObject = gsl::Vector3D::dot(nearPlaneToObjectVector, playCamera->nearPlaneNormal);
            if(nearPlaneHeightToObject + Sphere.radius >= 0)
            {
                gsl::Vector3D farPlaneToObjectVector = playCamera->farPlaneBottomLeft - Sphere.center;
                float farPlaneHeightToObject = gsl::Vector3D::dot(farPlaneToObjectVector, playCamera->farPlaneNormal);
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


