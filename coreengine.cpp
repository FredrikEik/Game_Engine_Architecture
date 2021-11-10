#include "coreengine.h"

#include "resourcemanager.h"
#include "soundsystem.h"
#include "gameobject.h"
#include "rendersystem.h"
#include "camera.h"
#include "mainwindow.h"

CoreEngine* CoreEngine::mInstance = nullptr;    //static pointer to instance

CoreEngine::CoreEngine(RenderSystem *renderSystemIn, MainWindow *mainWindowIn)
    : mRenderSystem{renderSystemIn}, mMainWindow{mainWindowIn}
{
    //Make logger
    mLogger = Logger::getInstance();
    //feed in MainWindow - have to be done, else logger will crash program
    mLogger->setMainWindow(mMainWindow);
    // current date/time based on current system
    time_t now = time(0);
    // convert now to string form
    char* dt = ctime(&now);
    mLogger->logText("Logger started " + std::string(dt), LColor::HIGHLIGHT);

    mResourceManager = &ResourceManager::getInstance();
    mSoundSystem = SoundSystem::getInstance();

    mInstance = this;

    //Make the gameloop timer:
    mGameLoopTimer = new QTimer(this);

    //Make EditorCamera:
    mEditorCamera = new Camera(50.f, 0.1f, 1000.f);
    mEditorCamera->mName = "EditorCam";
    mGameCamera = new Camera(50.f, 0.1f, 300.f);
    mGameCamera->mName = "GameCam";
}

CoreEngine *CoreEngine::getInstance()
{
    return mInstance;
}

void CoreEngine::togglePlayMode(bool shouldPlay)
{
    isPlaying = shouldPlay;
    SoundSystem::getInstance()->togglePlaySounds(isPlaying);
    mRenderSystem->mIsPlaying = isPlaying;
}

void CoreEngine::setUpScene()
{
    //ask resource manager to set up all assets
    mResourceManager->setUpAllSounds();
    mResourceManager->setUpAllMeshes();
    mResourceManager->setUpAllTextures();
    mResourceManager->setUpAllShaders();
    mResourceManager->setUpAllMaterials();  //materials uses textures and shaders!

    //********************** Making the object to be drawn **********************

    // *** Editor assets *** - always made
    //Axis
    GameObject *temp = mResourceManager->addObject("axis");
    temp->mName = "Axis";
    mRenderSystem->mGameObjects.push_back(temp);

    //Grid
    temp = mResourceManager->addObject("editorgrid");
    temp->mName = "EditorGrid";
    mRenderSystem->mGameObjects.push_back(temp);

    readScene("scene1.json");

    //dog triangle
    temp = mResourceManager->addObject("triangle");
    temp->mName = "DogTriangle";
    temp->mMaterial = mResourceManager->getMaterial("Texture");

    temp->mTransform->mPosition = gsl::Vector3D(5.f, 2.f, -2.f);
    //Adds sound to moving triangle:
    mResourceManager->addComponent("caravan_mono.wav", temp);

    //Ads light to this mesh - RenderSystem only supports one light at the moment:
    //Also this is so simple now, that I just make the component on the spot
    temp->mLightComponent = new LightComponet();

    //Hack to test sound system
    if(temp->mSoundComponent)
        temp->mSoundComponent->shouldPlay = true;

    mRenderSystem->mGameObjects.push_back(temp);

    //terrain
    temp = mResourceManager->addObject("terrain");
    temp->mName = "Terrain";
    temp->mMaterial = mResourceManager->getMaterial("Phong");
    mRenderSystem->mGameObjects.push_back(temp);

    //Suzannes:
//    bool useTexture{true};      //quick test to force stupid material change when rendering
//    for(int i{-50}; i < 100; i++)
//    {
//        for(int j{0}; j < 30; j++)
//        {
//            temp = mResourceManager->addObject("suzanne.obj");
//            temp->mTransform->mMatrix.translate(1.f*i, 0.f, -2.f*j);
//            //TODO: Scaling have to be made easier and more automatic than this!
//            temp->mTransform->mMatrix.scale(0.3f);
//            temp->mMesh->mColliderRadius *= 0.3f;   //this should be done automatically
//            temp->mTransform->mScale.setAlltoSame(0.3f);
//            temp->mName = "monkey " + std::to_string((i*10)+j+1);

//            if(useTexture)
//                temp->mMaterial = mResourceManager->getMaterial("Texture");
//            useTexture = !useTexture;
//            mRenderSystem->mGameObjects.push_back(temp);
//        }
//    }

//    temp = mResourceManager->addObject("suzanne.obj");
//    temp->mTransform->mMatrix.translate(0.f, 0.f, 0.f);
//    temp->mTransform->mMatrix.scale(0.5f);
//    mRenderSystem->mGameObjects.push_back(temp);

    mEditorCamera->mPosition = gsl::Vector3D(0.5f, 1.f, 4.f);
//    mEditorCamera->pitch(-20);
    mRenderSystem->mEditorCamera = mEditorCamera;

    mGameCamera->mPosition = gsl::Vector3D(0.f, 1.f, 0.f);
    mRenderSystem->mGameCamera = mGameCamera;

    //update SceneOutliner to show all objects:
    mMainWindow->updateOutliner(mRenderSystem->mGameObjects);

    //Copy data from GameObject that has lightcomponent
    //The last one in list will be used
    //because I only support one light
    for(auto gob : mRenderSystem->mGameObjects)
    {
        if(gob->mLightComponent)
        {
            mRenderSystem->mLight = gob->mLightComponent;
            mRenderSystem->mLightPosition = gob->mTransform;
        }
    }

    //Connect the gameloop timer to the render function:
    //This makes our render loop
    connect(mGameLoopTimer, SIGNAL(timeout()), this, SLOT(gameLoop()));
    //This timer runs the actual MainLoop
    //16 means 16ms = 60 Frames pr second (should be 16.6666666 to be exact...)
    mGameLoopTimer->start(20);
}

void CoreEngine::readScene(std::string sceneName)
{
    mLogger->logText("Loading scene " + sceneName, LColor::HIGHLIGHT);

    //****************** Read scene from json file **************************
    //this code have a lot of checking code, so it is long.
    //probably should be broken up into more functions
    QFile loadFile(QString((gsl::AssetFilePath + sceneName).c_str()));
    if (!loadFile.open(QIODevice::ReadOnly)) {
        mLogger->logText("Couldn't open " + sceneName + " file for reading.", LColor::DAMNERROR);
        return;
    }
    mLogger->logText(sceneName + " file opened for reading.");
    QByteArray saveData = loadFile.readAll();   //read whole file
    QJsonDocument loadDoc(QJsonDocument::fromJson(saveData));   //convert to json document
    QJsonObject jsonObject = loadDoc.object();  //read first object == whole thing

    //should contain Materials as first property and this should be an array
    if (jsonObject.contains("Scene") && jsonObject["Scene"].isArray())
    {
        QJsonArray entityArray = jsonObject["Scene"].toArray();
        //go through each entity in file
        int entityIndex{0};   //used to print log afterwards
        for ( ; entityIndex < entityArray.size(); ++entityIndex) {
            GameObject *tempGOB{nullptr};
            QString name;   //this is pushed as the key to the mEntityMap later ???? - maybe not

            QJsonObject entityObject = entityArray[entityIndex].toObject();   //first entity as object

            if (entityObject.contains("name") && entityObject["name"].isString())
                name = entityObject["name"].toString();

            if (entityObject.contains("mesh") && entityObject["mesh"].isString())
            {
                QString mesh = entityObject["mesh"].toString();
                tempGOB = mResourceManager->addObject(mesh.toStdString());
            }
            if (entityObject.contains("position") && entityObject["position"].isArray())
            {
                QJsonArray positionArray = entityObject["position"].toArray();
                if (positionArray[0].isDouble())
                    tempGOB->mTransform->mPosition.x = positionArray[0].toDouble();
                if (positionArray[1].isDouble())
                    tempGOB->mTransform->mPosition.y = positionArray[1].toDouble();
                if (positionArray[2].isDouble())
                    tempGOB->mTransform->mPosition.z = positionArray[2].toDouble();
            }
            if (entityObject.contains("scale") && entityObject["scale"].isArray())
            {
                QJsonArray scaleArray = entityObject["scale"].toArray();
                if (scaleArray[0].isDouble())
                    tempGOB->mTransform->mScale.x = scaleArray[0].toDouble();
                if (scaleArray[1].isDouble())
                    tempGOB->mTransform->mScale.y = scaleArray[1].toDouble();
                if (scaleArray[2].isDouble())
                    tempGOB->mTransform->mScale.z = scaleArray[2].toDouble();
            }
            if (entityObject.contains("rotation") && entityObject["rotation"].isArray())
            {
                QJsonArray rotationArray = entityObject["rotation"].toArray();
                if (rotationArray[0].isDouble())
                    tempGOB->mTransform->mRotation.x = rotationArray[0].toDouble();
                if (rotationArray[1].isDouble())
                    tempGOB->mTransform->mRotation.y = rotationArray[1].toDouble();
                if (rotationArray[2].isDouble())
                    tempGOB->mTransform->mRotation.z = rotationArray[2].toDouble();
            }
//            if (entityObject.contains("material") && entityObject["material"].isString())
//            {
//                QString material = entityObject["material"].toString();
//                tempGOB = mResourceManager->addObject(mesh.toStdString());
//            }
//            if (entityObject.contains("color") && entityObject["color"].isArray())
//            {
//                QJsonArray colorArray = entityObject["color"].toArray();
//                if (colorArray[0].isDouble())
//                    tempGOB.mColor.x = colorArray[0].toDouble();
//                if (colorArray[1].isDouble())
//                    tempGOB.mColor.y = colorArray[1].toDouble();
//                if (colorArray[2].isDouble())
//                    tempGOB.mColor.z = colorArray[2].toDouble();
//            }
//            if (entityObject.contains("useColor") && entityObject["useColor"].isBool())
//            {
//                bool useColor = entityObject["useColor"].toBool();
//                temp.mUseColor = useColor;
//            }
//            if (entityObject.contains("specularStrength") && entityObject["specularStrength"].isDouble())
//            {
//                float specularStrenght = entityObject["specularStrength"].toDouble();
//                temp.mSpecularStrength = specularStrenght;
//            }
//            if (entityObject.contains("specularExponent") && entityObject["specularExponent"].isDouble())
//            {
//                int specularExponent = entityObject["specularExponent"].toDouble();
//                temp.mSpecularExponent = specularExponent;
//            }

//            mMaterials.push_back(temp); //this should performe a copy...
//            mMaterialMap.emplace(name.toStdString(), materialIndex + 1);
            mRenderSystem->mGameObjects.push_back(tempGOB);
        }
        mLogger->logText("  " + std::to_string(entityIndex) + " entities read from file");
    }
    else
        mLogger->logText("scene.json file does not contain a Materials object", LColor::DAMNERROR);
}


void CoreEngine::handleInput()
{
    //Camera

    //TODO: Probably a cleaner way to do this!
    mEditorCamera->setSpeed(0.f);  //cancel last frame movement

    //move camera to selected object
    if(mInput.F && mRenderSystem->mIndexToPickedObject >-1)
    {
        int tempIndex = mRenderSystem->mIndexToPickedObject;
        mEditorCamera->mPosition = mRenderSystem->mGameObjects.at(tempIndex)->mTransform->mPosition;
        //dynamic distance to object
        mEditorCamera->mPosition.z += (mRenderSystem->mGameObjects.at(tempIndex)->mMesh->mColliderRadius)*7;
        //dynamic height placement
        mEditorCamera->mPosition.y += (mRenderSystem->mGameObjects.at(tempIndex)->mMesh->mColliderRadius*4);
        mEditorCamera->mPitch = -30.f;
        mEditorCamera->mYaw = 0.f;
        mEditorCamera->update();
        mInput.F = false;
    }

    if(mInput.RMB)
    {
        if(mInput.W)
            mEditorCamera->setSpeed(-mEditorCamera->mCameraSpeed);
        if(mInput.S)
            mEditorCamera->setSpeed(mEditorCamera->mCameraSpeed);
        if(mInput.D)
            mEditorCamera->moveRight(mEditorCamera->mCameraSpeed);
        if(mInput.A)
            mEditorCamera->moveRight(-mEditorCamera->mCameraSpeed);
        if(mInput.Q)
            mEditorCamera->updateHeigth(-mEditorCamera->mCameraSpeed);
        if(mInput.E)
            mEditorCamera->updateHeigth(mEditorCamera->mCameraSpeed);
    }
}

void CoreEngine::gameLoop()
{
    //Keyboard / mouse input
    handleInput();

    mEditorCamera->update();
//    SoundSystem::getInstance()->update(mRenderSystem);

    mRenderSystem->render();
}

