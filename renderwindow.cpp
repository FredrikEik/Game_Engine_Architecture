#include "renderwindow.h"
#include <QTimer>
#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <QOpenGLDebugLogger>
#include <QKeyEvent>
#include <QStatusBar>
#include <QDebug>

#include <iostream>

#include "shader.h"
#include "mainwindow.h"
#include "camera.h"
#include "constants.h"
#include "texture.h"
#include "meshhandler.h"
#include "gameobject.h"
#include "resourcemanager.h"
#include "soundmanager.h"
#include "gameengine.h"

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
}

RenderWindow::~RenderWindow()
{
}

// Sets up the general OpenGL stuff and the buffers needed to render a triangle
void RenderWindow::init()
{

    // TODO: Fix textures!!!



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
    int textureUnits;
    glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &textureUnits);
    std::cout << "  This GPU as " << textureUnits << " texture units / slots in total, ";
    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &mTextureUnits);
    std::cout << "and supports " << mTextureUnits << " texture units pr shader" << std::endl;

    //**********************  Texture stuff: **********************
    //Returns a pointer to the Texture class. This reads and sets up the texture for OpenGL
    //and returns the Texture ID that OpenGL uses from Texture::id()
    mTextures[0] = new Texture();
    mTextures[1] = new Texture("hund.bmp");

    //Set the textures loaded to a texture unit
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, mTextures[0]->mGLTextureID);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, mTextures[1]->mGLTextureID);


    //Start the Qt OpenGL debugger
    //Really helpfull when doing OpenGL
    //Supported on most Windows machines
    //reverts to plain glGetError() on Mac and other unsupported PCs
    // - can be deleted
    startOpenGLDebugger();

    //general OpenGL stuff:
    glEnable(GL_DEPTH_TEST);            //enables depth sorting - must then use GL_DEPTH_BUFFER_BIT in glClear
    glEnable(GL_CULL_FACE);       //draws only front side of models - usually what you want - test it out!
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

    mShaderPrograms[2] = new Shader((gsl::ShaderFilePath + "mousepickingvertex.vert").c_str(),
                                    (gsl::ShaderFilePath + "mousepickingfragment.frag").c_str());
    qDebug() << "Texture shader program id: " << mShaderPrograms[2]->getProgram();

    setupPlainShader(0);
    setupTextureShader(1);
    setupMousPickingShader(2);

    GameEngine::getInstance()->SetUpScene();


    // Should move to GameEngie
    //********************** Set up camera **********************
//    mCurrentCamera = new Camera();
//    mCurrentCamera->setPosition(gsl::Vector3D(1.f, .5f, 4.f));
}

// Called each frame - doing the rendering
void RenderWindow::render()
{
    // TODO: move camera and inputhandler to the resource manager


    //--->Shoudl be in GameEngine
    //Keyboard / mouse input
    handleInput();

    mVerticesDrawn = 0;
    mObjectsDrawn = 0;

    mTimeStart.restart(); //restart FPS clock
    mContext->makeCurrent(this); //must be called every frame (every time mContext->swapBuffers is called)

    initializeOpenGLFunctions();    //must call this every frame it seems...

//    //to clear the screen for each redraw
//    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


//    for(int i{0}; i < mGameObjects.size(); i++)
//    {
//        glUseProgram(mShaderPrograms[mGameObjects[i]->mMaterialComp->mShaderProgram]->getProgram() );
//        glUniformMatrix4fv( vMatrixUniform, 1, GL_TRUE, mCurrentCamera->mViewMatrix.constData());
//        glUniformMatrix4fv( pMatrixUniform, 1, GL_TRUE, mCurrentCamera->mProjectionMatrix.constData());
//        glUniformMatrix4fv( mMatrixUniform, 1, GL_TRUE, mGameObjects[i]->mTransformComp->mMatrix.constData());


//        int id = mGameObjects[i]->id;
//        int r = (id & 0x000000FF) >>  0;
//        int g = (id & 0x0000FF00) >>  8;
//        int b = (id & 0x00FF0000) >> 16;


////        glUniform4f(0, r/255.0f, g/255.0f, b/255.0f, 1.0f);



//        glBindVertexArray( mGameObjects[i]->mMeshComp->mVAO[0] );
//        glDrawArrays(mGameObjects[i]->mMeshComp->mDrawType, 0, mGameObjects[i]->mMeshComp->mVertices[0].size());
//    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // For GameObjects
    for(int i{0}; i < mGameObjects.size(); i++)
    {

        gsl::Vector3D objPos = mGameObjects[i]->mTransformComp->mMatrix.getPosition();

        gsl::Vector3D rightPlaneNormal = (mCurrentCamera->getRightVector());
        //right is minus rotation
        rightPlaneNormal.rotateY(-FOV);

        gsl::Vector3D leftPlaneNormal = -(mCurrentCamera->getRightVector());
        leftPlaneNormal.rotateY(FOV);

        gsl::Vector3D vectorToObj = objPos - mCurrentCamera->position();

        float distanceToRightObject = ((vectorToObj * rightPlaneNormal) / rightPlaneNormal.length());
        float distanceToLeftObject = ((vectorToObj * leftPlaneNormal) / leftPlaneNormal.length());

        gsl::Vector3D forwardPlaneNormal = mCurrentCamera->getFowrardVector();
        float distanceToFrontObject = ((vectorToObj * forwardPlaneNormal) / forwardPlaneNormal.length());

        float  distanceCamToObj = (objPos - mCurrentCamera->position()).length();

        if(bUsingFrustumCulling)
        {
            if(distanceToRightObject > 0)
            {
                continue;
            }
            if(distanceToLeftObject > 0)
            {
                continue;
            }
            if(distanceToFrontObject > mFarPlane)
            {
                continue;
            }
        }
        // Maybe not needed
//        if(distanceToFrontObject < mNearPlane)
//        {
//            continue;
//        }
        // Get a handle for our "pickingColorID" uniform
         //Hardcoded to 2,

//         This line says linking error
//        GLuint pickingColorID = glGetUniformLocation(2, "PickingColor");

        glUseProgram(mShaderPrograms[mGameObjects[i]->mMaterialComp->mShaderProgram]->getProgram() );
        //glUseProgram(2);
        GLuint pickingColorID = glGetUniformLocation(mShaderPrograms[2]->getProgram(), "PickingColor");

        if(mGameObjects[i]->mMaterialComp->mShaderProgram == 2)
        {
                    int id = mGameObjects[i]->id+50;
                    int r = (id & 0x000000FF) >>  0;
                    int g = (id & 0x0000FF00) >>  8;
                    int b = (id & 0x00FF0000) >> 16;
            glUniform4f(pickingColorID, r/255.0f, g/255.0f, b/255.0f, 1.0f);
            glUniformMatrix4fv( vMatrixUniform2, 1, GL_TRUE, mCurrentCamera->mViewMatrix.constData());
            glUniformMatrix4fv( pMatrixUniform2, 1, GL_TRUE, mCurrentCamera->mProjectionMatrix.constData());
            glUniformMatrix4fv( mMatrixUniform2, 1, GL_TRUE, mGameObjects[i]->mTransformComp->mMatrix.constData());
        }
        else if(mGameObjects[i]->mMaterialComp->mShaderProgram == 1)
        {
        //send data to shader
            glUniform1i(mTextureUniform, mGameObjects[i]->mMaterialComp->mTextureUnit);
            glUniformMatrix4fv( vMatrixUniform1, 1, GL_TRUE, mCurrentCamera->mViewMatrix.constData());
            glUniformMatrix4fv( pMatrixUniform1, 1, GL_TRUE, mCurrentCamera->mProjectionMatrix.constData());
            glUniformMatrix4fv( mMatrixUniform1, 1, GL_TRUE, mGameObjects[i]->mTransformComp->mMatrix.constData());
        }
        // need to fix [] for LOD
        //qDebug() << "Distacne: " << distanceToFrontObject;
        if(mGameObjects[i]->mMeshComp->bUsingLOD && bRenderingLOD)
        {
            if(distanceCamToObj < 10)
            {
                glBindVertexArray( mGameObjects[i]->mMeshComp->mVAO[0] );
                glDrawArrays(mGameObjects[i]->mMeshComp->mDrawType, 0, mGameObjects[i]->mMeshComp->mVertices[0].size());
                mVerticesDrawn += mGameObjects[i]->mMeshComp->mVertices[0].size();
                mObjectsDrawn++;
            }else if( distanceCamToObj < 20)
            {
                glBindVertexArray( mGameObjects[i]->mMeshComp->mVAO[1] );
                glDrawArrays(mGameObjects[i]->mMeshComp->mDrawType, 0, mGameObjects[i]->mMeshComp->mVertices[1].size());
                mVerticesDrawn += mGameObjects[i]->mMeshComp->mVertices[1].size();
                mObjectsDrawn++;
            }else{
                glBindVertexArray( mGameObjects[i]->mMeshComp->mVAO[2] );
                glDrawArrays(mGameObjects[i]->mMeshComp->mDrawType, 0, mGameObjects[i]->mMeshComp->mVertices[2].size());
                mVerticesDrawn += mGameObjects[i]->mMeshComp->mVertices[2].size();
                mObjectsDrawn++;
            }
        }else
        {
            glBindVertexArray( mGameObjects[i]->mMeshComp->mVAO[0] );
            glDrawArrays(mGameObjects[i]->mMeshComp->mDrawType, 0, mGameObjects[i]->mMeshComp->mVertices[0].size());
            mVerticesDrawn += mGameObjects[i]->mMeshComp->mVertices[0].size();
            mObjectsDrawn++;
        }


        // TO TURN ON: uncomment makecollision box in the end of the createObject funktion in mehshandler.cpp;
        if( bShowAllCollisionBoxes )
        {
            glBindVertexArray( mGameObjects[i]->mCollisionLines->mVAO[0] );
            glDrawElements(mGameObjects[i]->mCollisionLines->mDrawType, mGameObjects[i]->mCollisionLines->mIndices->size(), GL_UNSIGNED_INT, nullptr);
        }
        if( mGameObjects[i]->mCollisionComp->bShowCollisionBox)
        {
            glBindVertexArray( mGameObjects[i]->mCollisionLines->mVAO[0] );
            glDrawElements(mGameObjects[i]->mCollisionLines->mDrawType, mGameObjects[i]->mCollisionLines->mIndices->size(), GL_UNSIGNED_INT, nullptr);
        }



        mObjectsDrawn++;
        glBindVertexArray(0);
    }

    //Calculate framerate before
    // checkForGLerrors() because that takes a long time
    // and before swapBuffers(), else it will show the vsync time
    calculateFramerate();

    //using our expanded OpenGL debugger to check if everything is OK.
    checkForGLerrors();

    //Qt require us to call this swapBuffers() -function.
    // swapInterval is 1 by default which means that swapBuffers() will (hopefully) block
    // and wait for vsync.
    mContext->swapBuffers(this);

    glUseProgram(0);
}

void RenderWindow::setupPlainShader(int shaderIndex)
{
    mMatrixUniform = glGetUniformLocation( mShaderPrograms[shaderIndex]->getProgram(), "mMatrix" );
    vMatrixUniform = glGetUniformLocation( mShaderPrograms[shaderIndex]->getProgram(), "vMatrix" );
    pMatrixUniform = glGetUniformLocation( mShaderPrograms[shaderIndex]->getProgram(), "pMatrix" );
}

void RenderWindow::setupTextureShader(int shaderIndex)
{
    mMatrixUniform1 = glGetUniformLocation( mShaderPrograms[shaderIndex]->getProgram(), "mMatrix" );
    vMatrixUniform1 = glGetUniformLocation( mShaderPrograms[shaderIndex]->getProgram(), "vMatrix" );
    pMatrixUniform1 = glGetUniformLocation( mShaderPrograms[shaderIndex]->getProgram(), "pMatrix" );
    mTextureUniform = glGetUniformLocation( mShaderPrograms[shaderIndex]->getProgram(), "textureSampler");
}

void RenderWindow::setupMousPickingShader(int shaderIndex)
{
    mMatrixUniform2 = glGetUniformLocation( mShaderPrograms[shaderIndex]->getProgram(), "mMatrix" );
    vMatrixUniform2 = glGetUniformLocation( mShaderPrograms[shaderIndex]->getProgram(), "vMatrix" );
    pMatrixUniform2 = glGetUniformLocation( mShaderPrograms[shaderIndex]->getProgram(), "pMatrix" );
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
//    if (isExposed())
//    {
//        //This timer runs the actual MainLoop
//        //16 means 16ms = 60 Frames pr second (should be 16.6666666 to be exact...)
////        mRenderTimer->start(16);
//      mTimeStart.start();
//    }

    //calculate aspect ration and set projection matrix
    mAspectratio = static_cast<float>(width()) / height();
    //    qDebug() << mAspectratio;
    mCurrentCamera->mProjectionMatrix.perspective(FOV, mAspectratio, mNearPlane , mFarPlane);
    //    qDebug() << mCamera.mProjectionMatrix;
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
                                                  "FPS (approximated): " + QString::number(1E9 / nsecElapsed, 'g', 7) + " |  Objects Drawn: " + QString::number(mObjectsDrawn) + " | Vertices Drawn: " + QString::number(mVerticesDrawn)) ;
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

void RenderWindow::renderLOD(bool bIsToggleOn)
{
    if(bIsToggleOn)
        bRenderingLOD = false;
    else
        bRenderingLOD = true;

}

void RenderWindow::toggleFrustumCulling(bool bIsToggleOn)
{
    if(bIsToggleOn)
        bUsingFrustumCulling = false;
    else
        bUsingFrustumCulling = true;
}

void RenderWindow::toggleShowCollsionBox(bool bIsToggleOn)
{
    if(bIsToggleOn)
        bShowAllCollisionBoxes = true;
    else
        bShowAllCollisionBoxes = false;
}

std::vector<GameObject*> RenderWindow::getAllGameObject()
{
    return mGameObjects;
}


//GameObject* RenderWindow::RenderWindow::addObject(std::string assetName)
//{
//     GameObject *temp = ResourceManager::getInstance();
//}

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
    if(mInput.RMB)
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
}

void RenderWindow::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::RightButton)
        mInput.RMB = true;
    if (event->button() == Qt::LeftButton)
        mInput.LMB = true;
    if (event->button() == Qt::MiddleButton)
        mInput.MMB = true;
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
            setCameraSpeed(0.01f);
        if (numDegrees.y() > 1)
            setCameraSpeed(-0.01f);
    }
    event->accept();
}

void RenderWindow::mouseMoveEvent(QMouseEvent *event)
{
    if (mInput.RMB)
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
