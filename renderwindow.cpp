#include "renderwindow.h"
#include <QTimer>
#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <QOpenGLDebugLogger>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QStatusBar>
#include <QDebug>
#include <QFile>
#include <QJSEngine>

#include <iostream>
#include <stdio.h> // for snprintf

#include "shader.h"
#include "mainwindow.h"
#include "visualobject.h"
#include "xyz.h"
#include "triangle.h"
#include "camera.h"
#include "constants.h"
#include "texture.h"
#include "MathStuff/MathStuff.h"
#include "spawner.h"
#include "text2d.h"

#include "cube.h"
#include "objimport.h"
#include "soundmanager.h"


RenderWindow::RenderWindow(const QSurfaceFormat &format, MainWindow *mainWindow)
    : mContext(nullptr), mInitialized(false), mMainWindow(mainWindow)

{
    //This is sent to QWindow:
    setSurfaceType(QWindow::OpenGLSurface);
    setFormat(format);
    //Make the OpenGL context
    mContext = new QOpenGLContext(this);
    //Give the context the wanted OpenGL format (v4.1 Core)
    mContext->setFormat(requestedFormat());
    if (!mContext->create()) {
        delete mContext;
        mContext = nullptr;
        qDebug() << "Context could not be made - quitting this application";
    }

    //Make the Script engine itself
    QJSEngine engine;

    QString fileName = "../GEA2021/Script/tinyScript.js";

    QFile scriptFile(fileName);

    if (!scriptFile.open(QIODevice::ReadOnly))
        qDebug() << "Error - NO FILE HERE: " << fileName;

    //reads the file
    QTextStream stream(&scriptFile);
    QString contents = stream.readAll();
    //now "contents" holds the whole JavaScript

    scriptFile.close();

    //Loads the whole script into script engine:
    //The important part! fileName is used to report bugs in the file
    engine.evaluate(contents, fileName);

    //Make the gameloop timer:
    mRenderTimer = new QTimer(this);

    mScript = new Script();
    //Makes a script-version for the script engine:
    QJSValue objectTest = engine.newQObject(mScript);
    //Make a name for the object in the script engine
    engine.globalObject().setProperty("cObject", objectTest);
    QJSValue useFunction = engine.evaluate("setCVariable");
    useFunction.call();
    qDebug() << "C value set to:" << mScript->getSpeed();
}

RenderWindow::~RenderWindow()
{
}

// Sets up the general OpenGL stuff and the buffers needed to render a triangle
void RenderWindow::init()
{
    //Connect the gameloop timer to the render function:
    //This makes our render loop
    connect(mRenderTimer, SIGNAL(timeout()), this, SLOT(render()));
    //********************** General OpenGL stuff **********************

    //The OpenGL context has to be set.
    //The context belongs to the instanse of this class!
    if (!mContext->makeCurrent(this)) {
        qDebug() << "makeCurrent() failed";
        return;
    }

    //just to make sure we don't init several times
    //used in exposeEvent()
    if (!mInitialized)
        mInitialized = true;

    //must call this to use OpenGL functions
    initializeOpenGLFunctions();

    //Print render version info (what GPU is used):
    //(Have to use cout to see text- qDebug just writes numbers...)
    //Nice to see if you use the Intel GPU or the dedicated GPU on your laptop
    // - can be deleted
    std::cout << "The active GPU and API: \n";
    std::cout << "  Vendor: " << glGetString(GL_VENDOR) << std::endl;
    std::cout << "  Renderer: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "  Version: " << glGetString(GL_VERSION) << std::endl;

    //Get the texture units of your GPU
    int mTextureUnits; //Supported Texture Units (slots) pr shader. - maybe have in header!?
    int textureUnits = 7;
    glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &textureUnits);
    std::cout << "  This GPU as " << textureUnits << " texture units / slots in total, ";
    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &mTextureUnits);
    std::cout << "and supports " << mTextureUnits << " texture units pr shader" << std::endl;

    //**********************  Texture stuff: **********************
    //Returns a pointer to the Texture class. This reads and sets up the texture for OpenGL
    //and returns the Texture ID that OpenGL uses from Texture::id()
    mTextures[0] = new Texture();
    mTextures[1] = new Texture("hund.bmp");
    mTextures[2] = new Texture("cobbleStone.bmp");
    mTextures[3] = new Texture("whitefur.bmp");
    mTextures[4] = new Texture("firesky.bmp");
    mTextures[5] = new Texture("lava.bmp");
    mTextures[6] = new Texture("font.bmp");

    //Set the textures loaded to a texture unit
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, mTextures[0]->mGLTextureID);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, mTextures[1]->mGLTextureID);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, mTextures[2]->mGLTextureID);
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, mTextures[3]->mGLTextureID);
    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_2D, mTextures[4]->mGLTextureID);
    glActiveTexture(GL_TEXTURE5);
    glBindTexture(GL_TEXTURE_2D, mTextures[5]->mGLTextureID);
    glActiveTexture(GL_TEXTURE6);
    glBindTexture(GL_TEXTURE_2D, mTextures[6]->mGLTextureID);


    //Start the Qt OpenGL debugger
    //Really helpfull when doing OpenGL
    //Supported on most Windows machines
    //reverts to plain glGetError() on Mac and other unsupported PCs
    // - can be deleted
    startOpenGLDebugger();

    //general OpenGL stuff:
    glEnable(GL_DEPTH_TEST);            //enables depth sorting - must then use GL_DEPTH_BUFFER_BIT in glClear
    //    glEnable(GL_CULL_FACE);       //draws only front side of models - usually what you want - test it out!
    glClearColor(0.4f, 0.4f, 0.4f,1.0f);    //gray color used in glClear GL_COLOR_BUFFER_BIT

    //Compile shaders:
    //NB: hardcoded path to files! You have to change this if you change directories for the project.
    //Qt makes a build-folder besides the project folder. That is why we go down one directory
    // (out of the build-folder) and then up into the project folder.
    mShaderPrograms[0] = new Shader((gsl::ShaderFilePath + "plainvertex.vert").c_str(),
                                (gsl::ShaderFilePath + "plainfragment.frag").c_str());
    qDebug() << "Plain shader program id: " << mShaderPrograms[0]->getProgram();

    mShaderPrograms[1] = new Shader((gsl::ShaderFilePath + "textureshader.vert").c_str(),
                                    (gsl::ShaderFilePath + "textureshader.frag").c_str());
    qDebug() << "Texture shader program id: " << mShaderPrograms[1]->getProgram();

    setupPlainShader(0);
    setupTextureShader(1);

    //********************** Set up camera **********************
    mGameCamera = new Camera();
    mEditorCamera = new Camera();

    mCurrentCamera = mEditorCamera;
    mCurrentCamera->setPosition(gsl::Vector3D(1.f, .5f, 4.f));

    SoundManager::getInstance()->init();

    mStereoSound = SoundManager::getInstance()->createSource("Stereo", Vector3(0.0f, 0.0f, 0.0f),
                                              gsl::SoundFilePath + "death.wav", false, 0.05f);

    mPop = SoundManager::getInstance()->createSource("Stereo", Vector3(0.0f, 0.0f, 0.0f),
                                                     gsl::SoundFilePath + "pop.wav", false, 0.1f);

    mJump = SoundManager::getInstance()->createSource("Stereo", Vector3(0.0f, 0.0f, 0.0f),
                                                      gsl::SoundFilePath + "jump.wav", false, 0.1f);

//    mStereoSound->play();

    mMousePicker = new MousePicker(mCurrentCamera);
    mMainWindow->setMouseTracking(true);

    ObjFactory->createObject("Goat");
    mMainWindow->addObjectToWorldList("Player");
    ObjFactory->mGameObject.back()->TransformComp->mMatrix.setPosition(3.f, 0.51f, 0.f);
    ObjFactory->mGameObject.back()->TransformComp->mTrueScaleMatrix.setPosition(3.f, 0.51f, 0.f);
    mPlayer = new player(ObjFactory->mGameObject.back());

    skyBox = new SkyBox();
    skyBox->init();
    skyBox->mName = "skybox";
    skyBox->mTexture = 4;
    ObjFactory->mGameObject.push_back(skyBox);
    mMainWindow->addObjectToWorldList("SkyBox");
    ObjFactory->setOBJindex(ObjFactory->mGameObject.size() - 1);
    setScaleX(100);
    setScaleY(100);
    setScaleZ(100);
    ObjFactory->setOBJindex(-1);

    MapSpawner = new Spawner(ObjFactory, mMainWindow);
    //MapSpawner->spawnRow();
    MapSpawner->spawnRow(100);
    //MapSpawner->spawnHindrances(100);
    //MapSpawner->addObjectToEditor(object);

    mCurrentScore = new Text2D("SCORE:0.000", 0, 190);
    ObjFactory->mGameObject.push_back(mCurrentScore);
    mMainWindow->addObjectToWorldList("ScoreCounter");
    mHighScore = new Text2D("HIGHSCORE:0.000", 0, 180);
    ObjFactory->mGameObject.push_back(mHighScore);
    mMainWindow->addObjectToWorldList("HighScoreCounter");
}

// Called each frame - doing the rendering
void RenderWindow::render()
{
    //Keyboard / mouse input
    handleInput();

    mCurrentCamera->update();

    mTimeStart.restart(); //restart FPS clock
    mVerticesDrawn = 0;     //reset vertex counter
    mObjectsDrawn = 0;      //reset object counter

    mContext->makeCurrent(this); //must be called every frame (every time mContext->swapBuffers is called)

    initializeOpenGLFunctions();    //must call this every frame it seems...

    //to clear the screen for each redraw
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    mCurrentCamera->calculateFrustumVectors();

    skyBox->Update(mCurrentCamera->getPosition());

    MapSpawner->update(mPlayer->mMesh->TransformComp->mMatrix.getPosition().z);

    char number[6];
    const char* text = "SCORE:";
    if (mPlayer->mMesh->TransformComp->mMatrix.getPosition().z == 0.f)
        snprintf(number, sizeof (number), "%f", 0.f);
    else
        snprintf(number, sizeof (number), "%f", -mPlayer->mMesh->TransformComp->mMatrix.getPosition().z);
    char* buffer = new char[strlen(text) + strlen(number) + 1 + 1];
    strcpy(buffer, text);
    strcat(buffer, number);
    if (-mPlayer->mMesh->TransformComp->mMatrix.getPosition().z > mHighScoreCount)
    {
        mHighScoreCount = -mPlayer->mMesh->TransformComp->mMatrix.getPosition().z;
        const char* text1 = "HIGHSCORE:";
        char* buffer1 = new char[strlen(text1) + strlen(number) + 1 + 1];
        strcpy(buffer1, text1);
        strcat(buffer1, number);
        mHighScore->updateText(buffer1);
        delete[] buffer1;
    }
    mCurrentScore->updateText(buffer);
    delete[] buffer;

    unsigned int cullSafe;
    if(bPlayGame)
        cullSafe = 0; // original -1
    else
    {
        cullSafe = 1;
    }

    if(ObjFactory->mGameObject.size() > 0)
    {
        glUseProgram(mShaderPrograms[1]->getProgram());
        glUniform1i(mTextureUniform, 0); // chooses texture slot 0
        mTextureIndex = 0;

        for (unsigned int i = 0; i < ObjFactory->mGameObject.size(); i++){
            //send data to shader
            if (mTextureIndex != ObjFactory->mGameObject[i]->mTexture)
            {
                mTextureIndex = ObjFactory->mGameObject[i]->mTexture;
                glUniform1i(mTextureUniform, mTextureIndex);
            }

            glUniformMatrix4fv( vMatrixUniform1, 1, GL_TRUE, mCurrentCamera->mViewMatrix.constData());
            glUniformMatrix4fv( pMatrixUniform1, 1, GL_TRUE, mCurrentCamera->mProjectionMatrix.constData());
            glUniformMatrix4fv( mMatrixUniform1, 1, GL_TRUE, ObjFactory->mGameObject[i]->getTransformComp()->mMatrix.constData());
            if (ObjFactory->mGameObject[i]->bHudElement)// this should ake it 2d. hopefully
                glUniform1i( m2DUniform1, 1);
            else
                glUniform1i( m2DUniform1, 0);
            //draw the object
            if(mUseFrustumCulling && i > cullSafe && ObjFactory->mGameObject.size() > 0 && ObjFactory->mGameObject[i]->mName != "skybox" && !ObjFactory->mGameObject[i]->bHudElement)
            {
                if(frustumCulling(i))
                    continue;
            }
            ObjFactory->mGameObject[i]->draw();

            if (i > 1)
            {
                bool test;
                test = objectsColliding(*mPlayer->mMesh->CollisionComp, *ObjFactory->mGameObject[i]->getCollisionComp(),
                                        *mPlayer->mMesh->TransformComp,
                                        *ObjFactory->mGameObject[i]->getTransformComp());

                if (test && bPlayGame)
                    toggleGameMode();
            }
        }

        calculateFramerate();

        checkForGLerrors();

        mContext->swapBuffers(this);
    }
}

/**
 * @brief RenderWindow::setupPlainShader
 * @param shaderIndex
 */
void RenderWindow::setupPlainShader(int shaderIndex)
{
    mMatrixUniform = glGetUniformLocation( mShaderPrograms[shaderIndex]->getProgram(), "mMatrix" );
    vMatrixUniform = glGetUniformLocation( mShaderPrograms[shaderIndex]->getProgram(), "vMatrix" );
    pMatrixUniform = glGetUniformLocation( mShaderPrograms[shaderIndex]->getProgram(), "pMatrix" );
    m2DUniform = glGetUniformLocation( mShaderPrograms[shaderIndex]->getProgram(), "HudElement");
}

/**
 * @brief RenderWindow::setupTextureShader
 * @param shaderIndex
 */
void RenderWindow::setupTextureShader(int shaderIndex)
{
    mMatrixUniform1 = glGetUniformLocation( mShaderPrograms[shaderIndex]->getProgram(), "mMatrix" );
    vMatrixUniform1 = glGetUniformLocation( mShaderPrograms[shaderIndex]->getProgram(), "vMatrix" );
    pMatrixUniform1 = glGetUniformLocation( mShaderPrograms[shaderIndex]->getProgram(), "pMatrix" );
    mTextureUniform = glGetUniformLocation(mShaderPrograms[shaderIndex]->getProgram(), "textureSampler");
    m2DUniform1 = glGetUniformLocation( mShaderPrograms[shaderIndex]->getProgram(), "HudElement");
}

//This function is called from Qt when window is exposed (shown)
// and when it is resized
//exposeEvent is a overridden function from QWindow that we inherit from
void RenderWindow::exposeEvent(QExposeEvent *)
{
    //if not already initialized - run init() function
    if (!mInitialized)
        init();

    //This is just to support modern screens with "double" pixels (Macs and some 4k Windows laptops)
    const qreal retinaScale = devicePixelRatio();

    //Set viewport width and height
    glViewport(0, 0, static_cast<GLint>(width() * retinaScale), static_cast<GLint>(height() * retinaScale));

    //If the window actually is exposed to the screen we start the main loop
    //isExposed() is a function in QWindow
    if (isExposed())
    {
        //This timer runs the actual MainLoop
        //16 means 16ms = 60 Frames pr second (should be 16.6666666 to be exact...)
        mRenderTimer->start(16);
        mTimeStart.start();
    }

    //calculate aspect ration and set projection matrix
    mAspectratio = static_cast<float>(width()) / height();
    mGameCamera->mProjectionMatrix.perspective(45.f, mAspectratio, 0.1f, 100.f);
    mEditorCamera->mProjectionMatrix.perspective(45.f, mAspectratio, 0.1f, 100.f);
    gsl::Matrix4x4 projectionMatrix =  mCurrentCamera->mProjectionMatrix;
}

//The way this is set up is that we start the clock before doing the draw call,
// and check the time right after it is finished (done in the render function)
//This will approximate what framerate we COULD have.
//The actual frame rate on your monitor is limited by the vsync and is probably 60Hz
void RenderWindow::calculateFramerate()
{
    long nsecElapsed = mTimeStart.nsecsElapsed();
    static int frameCount{0};                       //counting actual frames for a quick "timer" for the statusbar

    if (mMainWindow)            //if no mainWindow, something is really wrong...
    {
        ++frameCount;
        if (frameCount > 30)    //once pr 30 frames = update the message twice pr second (on a 60Hz monitor)
        {
            //showing some statistics in status bar
            mMainWindow->statusBar()->showMessage(" Time pr FrameDraw: " +
                                                  QString::number(nsecElapsed/1000000.f, 'g', 4) + " ms  |  " +
                                                  "FPS (approximated): " + QString::number(1E9 / nsecElapsed, 'g', 7) +
                                                  "  |  Obj Count: " + QString::number(ObjFactory->mGameObject.size()));
            frameCount = 0;     //reset to show a new message in 60 frames
        }
    }
}

//Simple way to turn on/off wireframe mode
//Not totally accurate, but draws the objects with
//lines instead of filled polygons
void RenderWindow::toggleWireframe(bool buttonState)
{
    if (buttonState)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);    //turn on wireframe mode
        glDisable(GL_CULL_FACE);
    }
    else
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);    //turn off wireframe mode
        glEnable(GL_CULL_FACE);
    }
}

void RenderWindow::playSound()
{
    mStereoSound->play();
}

void RenderWindow::pauseSound()
{
    mStereoSound->pause();
}

void RenderWindow::stopSound()
{
    mStereoSound->stop();
}

/**
 * @brief RenderWindow::setSelectionIndex
 * @param index
 */
void RenderWindow::setSelectionIndex(int index)
{
    ObjFactory->setOBJindex(index);
    //qDebug() << index;
}

/**
 * @brief RenderWindow::deleteSelection
 */
void RenderWindow::deleteSelection()
{
    int index = ObjFactory->getOBJindex();
    //qDebug() << index;
    if (index != -1)
    {
        ObjFactory->mGameObject.erase(ObjFactory->mGameObject.begin()+index);
        mMainWindow->removeObjectFromWorldList();
        index = mMainWindow->getCurrentRow();
        ObjFactory->setOBJindex(index);
        mMainWindow->displayCurrentTransform(index);
    }
}

/**
 * @brief RenderWindow::getTransform
 * @param index
 * @return
 */
gsl::Matrix4x4 RenderWindow::getTransform(int index)
{
     gsl::Matrix4x4 Matrix = ObjFactory->mGameObject[index]->getTransformComp()->mTrueScaleMatrix;

     return Matrix;
}
/**
 * @brief RenderWindow::getRotation
 * @param index
 * @return
 */
gsl::Vector3D RenderWindow::getRotation(int index)
{
    gsl::Vector3D Rotation = ObjFactory->mGameObject[index]->getTransformComp()->Rot;

    return Rotation;
}
/**
 * @brief RenderWindow::getScale
 * @param index
 * @return
 */
gsl::Vector3D RenderWindow::getScale(int index)
{
    gsl::Vector3D Scale = ObjFactory->mGameObject[index]->getTransformComp()->Scal;

    return Scale;
}
/**
 * @brief RenderWindow::setPositionX
 * @param value
 */
void RenderWindow::setPositionX(double value)
{
    int index = ObjFactory->getOBJindex();
    if (index == -1)
        return;
    gsl::Vector3D position = ObjFactory->mGameObject[index]->getTransformComp()->mTrueScaleMatrix.getPosition();
    ObjFactory->mGameObject[index]->getTransformComp()->mTrueScaleMatrix.setPosition(value,position.getY(),position.getZ());
    updateMatrix();
}
/**
 * @brief RenderWindow::setPositionY
 * @param value
 */
void RenderWindow::setPositionY(double value)
{
    int index = ObjFactory->getOBJindex();
    if (index == -1)
        return;
    gsl::Vector3D position = ObjFactory->mGameObject[index]->getTransformComp()->mTrueScaleMatrix.getPosition();
    ObjFactory->mGameObject[index]->getTransformComp()->mTrueScaleMatrix.setPosition(position.getX(),value,position.getZ());
    updateMatrix();
}
/**
 * @brief RenderWindow::setPositionZ
 * @param value
 */
void RenderWindow::setPositionZ(double value)
{
    int index = ObjFactory->getOBJindex();
    if (index == -1)
        return;
    gsl::Vector3D position = ObjFactory->mGameObject[index]->getTransformComp()->mTrueScaleMatrix.getPosition();
    ObjFactory->mGameObject[index]->getTransformComp()->mTrueScaleMatrix.setPosition(position.getX(),position.getY(),value);
    updateMatrix();
}
/**
 * @brief RenderWindow::setRotationX
 * @param value
 */
void RenderWindow::setRotationX(double value)
{
    int index = ObjFactory->getOBJindex();
    if (index == -1)
        return;
    float x = value - ObjFactory->mGameObject[index]->getTransformComp()->Rot.getX();
    ObjFactory->mGameObject[index]->getTransformComp()->Rot.setX(x+ObjFactory->mGameObject[index]->getTransformComp()->Rot.getX());
    ObjFactory->mGameObject[index]->getTransformComp()->mTrueScaleMatrix.rotateX(x);
    updateMatrix();
}
/**
 * @brief RenderWindow::setRotationY
 * @param value
 */
void RenderWindow::setRotationY(double value)
{
    int index = ObjFactory->getOBJindex();
    if (index == -1)
        return;
    float y = value - ObjFactory->mGameObject[index]->getTransformComp()->Rot.getY();
    ObjFactory->mGameObject[index]->getTransformComp()->Rot.setY(y+ObjFactory->mGameObject[index]->getTransformComp()->Rot.getY());
    ObjFactory->mGameObject[index]->getTransformComp()->mTrueScaleMatrix.rotateY(y);
    updateMatrix();
}
/**
 * @brief RenderWindow::setRotationZ
 * @param value
 */
void RenderWindow::setRotationZ(double value)
{
    int index = ObjFactory->getOBJindex();
    if (index == -1)
        return;
    float z = value - ObjFactory->mGameObject[index]->getTransformComp()->Rot.getZ();
    ObjFactory->mGameObject[index]->getTransformComp()->Rot.setZ(z+ObjFactory->mGameObject[index]->getTransformComp()->Rot.getZ());
    ObjFactory->mGameObject[index]->getTransformComp()->mTrueScaleMatrix.rotateZ(z);
    updateMatrix();
}
/**
 * @brief RenderWindow::setScaleX
 * @param value
 */
void RenderWindow::setScaleX(double value)
{
    int index = ObjFactory->getOBJindex();
    if (index == -1)
        return;
    ObjFactory->mGameObject[index]->getTransformComp()->Scal.setX(value);
    updateMatrix();
}
/**
 * @brief RenderWindow::setScaleY
 * @param value
 */
void RenderWindow::setScaleY(double value)
{
    int index = ObjFactory->getOBJindex();
    if (index == -1)
        return;
    ObjFactory->mGameObject[index]->getTransformComp()->Scal.setY(value);
    updateMatrix();
}
/**
 * @brief RenderWindow::setScaleZ
 * @param value
 */
void RenderWindow::setScaleZ(double value)
{
    int index = ObjFactory->getOBJindex();
    if (index == -1)
        return;
    ObjFactory->mGameObject[index]->getTransformComp()->Scal.setZ(value);
    updateMatrix();
}

/**
 * @brief RenderWindow::setScale
 * @param x
 * @param y
 * @param z
 * @param index
 */
void RenderWindow::setScale(double x, double y, double z, int index)
{
    if (x == 0)
        x = 0.01;
    if (y == 0)
        y = 0.01;
    if (z == 0)
        z = 0.01;

    ObjFactory->mGameObject[index]->getTransformComp()->mMatrix.scale(gsl::Vector3D(x, y, z));
}

/**
 * @brief RenderWindow::updateMatrix
 */
void RenderWindow::updateMatrix()
{
    int index = ObjFactory->getOBJindex();
    if (index == -1)
        return;
    ObjFactory->mGameObject[index]->getTransformComp()->mMatrix = ObjFactory->mGameObject[index]->getTransformComp()->mTrueScaleMatrix;
    setScale(ObjFactory->mGameObject[index]->getTransformComp()->Scal.getX(),
             ObjFactory->mGameObject[index]->getTransformComp()->Scal.getY(),
             ObjFactory->mGameObject[index]->getTransformComp()->Scal.getZ(),
             index);
}

/**
 * @brief RenderWindow::toggleGameMode
 */
void RenderWindow::toggleGameMode()
{
    if (bPlayGame) {
        bPlayGame = false;
        mCurrentCamera = mEditorCamera;
        if (mPlayer)
        {
            mPlayer->mMesh->TransformComp->mMatrix.setPosition(3.f, 0.51f, 0.f);
            mPlayer->mMesh->TransformComp->mTrueScaleMatrix.setPosition(3.f, 0.51f, 0.f);
        }
        if (MapSpawner)
        {
            MapSpawner->resetSpawner();
        }
        stopSound();
        mMainWindow->disableWorldObjects(false);
    }
    else {
        bPlayGame = true;
        ObjFactory->setOBJindex(-1);
        mCurrentCamera = mGameCamera;
        mCurrentCamera->setPosition(gsl::Vector3D(3.f, 2.f, 5.f));
        mCurrentCamera->setPitch(0.f);
        mCurrentCamera->setYaw(0.f);
        playSound();
        mMainWindow->disableWorldObjects(true);
    }
}


/**
 * @brief RenderWindow::mousePickingRay
 * @param event
 */
void RenderWindow::mousePickingRay(QMouseEvent *event)
{
    int mouseX = event->pos().x();
    int mouseY = event->pos().y();

    mMousePicker->update(mouseX, mouseY, mMainWindow->getWidth(), mMainWindow->getHeight());

    //qDebug() << "x: " << mouseX << " y: " << mouseY <<" mp x: " << mMousePicker->getCurrentRay().getX() << " mp y: " << mMousePicker->getCurrentRay().getY() << " mp z: " << mMousePicker->getCurrentRay().getZ();

    //Mousepicking
    if(mInput.LMB && !bPlayGame)
    {
        for (int i = 0; i < ObjFactory->mGameObject.size(); i++ ) {
            if (mMousePicker->TestRayHitSphere(ObjFactory->mGameObject[i]->getTransformComp()->mMatrix.getPosition(), 0.5f))
            {
                ObjFactory->setOBJindex(i);
                mMainWindow->setSelection(i);
                break;
            }
        }
    }
}

/**
 * @brief RenderWindow::frustumCulling
 * @param gameObjectIndex
 * @return
 */
bool RenderWindow::frustumCulling(int gameObjectIndex)
{
    Camera *cullCamera{nullptr};
    cullCamera = mCurrentCamera;

    //vector from position of cam to object;
    gsl::Vector3D vectorToObject = ObjFactory->mGameObject[gameObjectIndex]->TransformComp->mMatrix.getPosition()
            - cullCamera->mPosition;

    //radius of object sphere
    float gobRadius = 0.5f;
    //Mesh data is not scaled so have to calculate for that
    //TODO: The system will break if scaling is not uniform...
    gobRadius *= ObjFactory->mGameObject[gameObjectIndex]->TransformComp->Scal.x;

    //if radius is not set == very small
    if(gobRadius <= 0.000001f)
        return false;

    //length of vectorToObject onto frustum normal
    float tempDistance{0.f};

    //shortcut to frustum
    Frustum &frustum = cullCamera->mFrustum;

    //the collider sphere seems to be a little to small, so adding this
    //padding to not cull them to early
    float padding{0.2f}; //

    //Project vector down to frustum normals:
    //Right plane:
    tempDistance = frustum.mRightPlane * vectorToObject;    // * here is dot product
    if(tempDistance > (gobRadius + padding)) //comment out gobRadius inside this to test frustum culling by clipping length
        return true;

    //Left plane:
    tempDistance = frustum.mLeftPlane * vectorToObject;    // * here is dot product
    if(tempDistance > (gobRadius + padding)) //comment out gobRadius inside this to test frustum culling by clipping length
        return true;

    //insert the rest of planes here

    return false;
}

/**
 * @brief RenderWindow::ObjectButton
 * @param object
 */
void RenderWindow::ObjectButton(std::string object)
{
    ObjFactory->createObject(object);
    mMainWindow->addObjectToWorldList(object);
    mPop->play();

}

//Uses QOpenGLDebugLogger if this is present
//Reverts to glGetError() if not
void RenderWindow::checkForGLerrors()
{
    if(mOpenGLDebugLogger)
    {
        const QList<QOpenGLDebugMessage> messages = mOpenGLDebugLogger->loggedMessages();
        for (const QOpenGLDebugMessage &message : messages)
            qDebug() << message;
    }
    else
    {
        GLenum err = GL_NO_ERROR;
        while((err = glGetError()) != GL_NO_ERROR)
        {
            qDebug() << "glGetError returns " << err;
        }
    }
}

//Tries to start the extended OpenGL debugger that comes with Qt
void RenderWindow::startOpenGLDebugger()
{
    QOpenGLContext * temp = this->context();
    if (temp)
    {
        QSurfaceFormat format = temp->format();
        if (! format.testOption(QSurfaceFormat::DebugContext))
            qDebug() << "This system can not use QOpenGLDebugLogger, so we revert to glGetError()";

        if(temp->hasExtension(QByteArrayLiteral("GL_KHR_debug")))
        {
            qDebug() << "System can log OpenGL errors!";
            mOpenGLDebugLogger = new QOpenGLDebugLogger(this);
            if (mOpenGLDebugLogger->initialize()) // initializes in the current context
                qDebug() << "Started OpenGL debug logger!";
        }
    }
}

/**
 * @brief RenderWindow::objectsColliding
 * @param Box1
 * @param Box2
 * @param Box1trans
 * @param Box2trans
 * @return
 */
bool RenderWindow::objectsColliding(CollisionComponent Box1, CollisionComponent Box2, TransformComponent Box1trans, TransformComponent Box2trans)
{
    gsl::Vector3D Box1Pos = Box1trans.mTrueScaleMatrix.getPosition();
    gsl::Vector3D Box2Pos = Box2trans.mTrueScaleMatrix.getPosition();
    gsl::Vector3D Box1min = gsl::Vector3D(Box1.min.x * Box1trans.Scal.x, Box1.min.y * Box1trans.Scal.y, Box1.min.z * Box1trans.Scal.z);
    gsl::Vector3D Box1max = gsl::Vector3D(Box1.max.x * Box1trans.Scal.x, Box1.max.y * Box1trans.Scal.y, Box1.max.z * Box1trans.Scal.z);
    gsl::Vector3D Box2min = gsl::Vector3D(Box2.min.x * Box2trans.Scal.x, Box2.min.y * Box2trans.Scal.y, Box2.min.z * Box2trans.Scal.z);
    gsl::Vector3D Box2max = gsl::Vector3D(Box2.max.x * Box2trans.Scal.x, Box2.max.y * Box2trans.Scal.y, Box2.max.z * Box2trans.Scal.z);
    Box1min += Box1Pos;
    Box1max += Box1Pos;
    Box2min += Box2Pos;
    Box2max += Box2Pos;

    return (Box1min.getX() <= Box2max.getX() && Box1max.getX() >= Box2min.getX()) &&
           (Box1min.getY() <= Box2max.getY() && Box1max.getY() >= Box2min.getY()) &&
           (Box1min.getZ() <= Box2max.getZ() && Box1max.getZ() >= Box2min.getZ());

}

void RenderWindow::setCameraSpeed(float value)
{
    mCameraSpeed += value;

    //Keep within some min and max values
    if(mCameraSpeed < 0.01f)
        mCameraSpeed = 0.01f;
    if (mCameraSpeed > 0.3f)
        mCameraSpeed = 0.3f;
}

void RenderWindow::handleInput()
{
    //Camera
    mCurrentCamera->setSpeed(0.f);  //cancel last frame movement
    if(mInput.RMB && !bPlayGame)
    {
        if(mInput.W)
            mCurrentCamera->setSpeed(-mCameraSpeed);
        if(mInput.S)
            mCurrentCamera->setSpeed(mCameraSpeed);
        if(mInput.D)
            mCurrentCamera->moveRight(mCameraSpeed);
        if(mInput.A)
            mCurrentCamera->moveRight(-mCameraSpeed);
        if(mInput.Q)
            mCurrentCamera->updateHeigth(-mCameraSpeed);
        if(mInput.E)
            mCurrentCamera->updateHeigth(mCameraSpeed);
    }

    //Player
    if (bPlayGame && mPlayer)
    {
        //qDebug() << "Speed: " << mScript->getSpeed();
        float deltaTime = mTimeStart.nsecsElapsed() / 1000000.f;
        if(mInput.A)
            mPlayer->Move(-1.7 / deltaTime);
        if(mInput.D)
            mPlayer->Move(1.7 / deltaTime);
        if(mInput.SPACE){
            mPlayer->Jump(mJump);
        }
        mPlayer->update(deltaTime);
        gsl::Vector3D position = mCurrentCamera->getPosition();
        mCurrentCamera->setPosition(gsl::Vector3D(position.x, position.y, position.z + (-4 / deltaTime)));
    }
}

void RenderWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape) //Shuts down whole program
    {
        mMainWindow->close();
    }

    //    You get the keyboard input like this
    if(event->key() == Qt::Key_W)
    {
        mInput.W = true;
    }
    if(event->key() == Qt::Key_S)
    {
        mInput.S = true;
    }
    if(event->key() == Qt::Key_D)
    {
        mInput.D = true;
    }
    if(event->key() == Qt::Key_A)
    {
        mInput.A = true;
    }
    if(event->key() == Qt::Key_Q)
    {
        mInput.Q = true;
    }
    if(event->key() == Qt::Key_E)
    {
        mInput.E = true;
    }
    if(event->key() == Qt::Key_Z)
    {
    }
    if(event->key() == Qt::Key_X)
    {
    }
    if(event->key() == Qt::Key_Up)
    {
        mInput.UP = true;
    }
    if(event->key() == Qt::Key_Down)
    {
        mInput.DOWN = true;
    }
    if(event->key() == Qt::Key_Left)
    {
        mInput.LEFT = true;
    }
    if(event->key() == Qt::Key_Right)
    {
        mInput.RIGHT = true;
    }
    if(event->key() == Qt::Key_U)
    {
    }
    if(event->key() == Qt::Key_O)
    {
    }
    if(event->key() == Qt::Key_Space)
    {
        mInput.SPACE = true;
    }
}

void RenderWindow::keyReleaseEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_W)
    {
        mInput.W = false;
    }
    if(event->key() == Qt::Key_S)
    {
        mInput.S = false;
    }
    if(event->key() == Qt::Key_D)
    {
        mInput.D = false;
    }
    if(event->key() == Qt::Key_A)
    {
        mInput.A = false;
    }
    if(event->key() == Qt::Key_Q)
    {
        mInput.Q = false;
    }
    if(event->key() == Qt::Key_E)
    {
        mInput.E = false;
    }
    if(event->key() == Qt::Key_Z)
    {
    }
    if(event->key() == Qt::Key_X)
    {
    }
    if(event->key() == Qt::Key_Up)
    {
        mInput.UP = false;
    }
    if(event->key() == Qt::Key_Down)
    {
        mInput.DOWN = false;
    }
    if(event->key() == Qt::Key_Left)
    {
        mInput.LEFT = false;
    }
    if(event->key() == Qt::Key_Right)
    {
        mInput.RIGHT = false;
    }
    if(event->key() == Qt::Key_U)
    {
    }
    if(event->key() == Qt::Key_O)
    {
    }
    if(event->key() == Qt::Key_Space)
    {
        mInput.SPACE = false;
    }
}

void RenderWindow::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::RightButton)
        mInput.RMB = true;
    if (event->button() == Qt::LeftButton)
        mInput.LMB = true;
    if (event->button() == Qt::MiddleButton)
        mInput.MMB = true;

    if(event->button() == Qt::LeftButton)
    {
        mousePickingRay(event);
    }
}

void RenderWindow::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::RightButton)
        mInput.RMB = false;
    if (event->button() == Qt::LeftButton)
        mInput.LMB = false;
    if (event->button() == Qt::MiddleButton)
        mInput.MMB = false;
}

void RenderWindow::wheelEvent(QWheelEvent *event)
{
    QPoint numDegrees = event->angleDelta() / 8;

    //if RMB, change the speed of the camera
    if (mInput.RMB)
    {
        if (numDegrees.y() < 1)
            setCameraSpeed(0.001f);
        if (numDegrees.y() > 1)
            setCameraSpeed(-0.001f);
    }
    event->accept();
}

void RenderWindow::mouseMoveEvent(QMouseEvent *event)
{
    if (mInput.RMB && !bPlayGame)
    {
        //Using mMouseXYlast as deltaXY so we don't need extra variables
        mMouseXlast = event->pos().x() - mMouseXlast;
        mMouseYlast = event->pos().y() - mMouseYlast;

        if (mMouseXlast != 0)
            mCurrentCamera->yaw(mCameraRotateSpeed * mMouseXlast);
        if (mMouseYlast != 0)
            mCurrentCamera->pitch(mCameraRotateSpeed * mMouseYlast);
    }
    mMouseXlast = event->pos().x();
    mMouseYlast = event->pos().y();
}
