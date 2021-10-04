#include "rendersystem.h"
#include <QTimer>
#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <QOpenGLDebugLogger>
#include <QKeyEvent>
#include <QStatusBar>
#include <QDebug>

#include <iostream>

#include "shaderhandler.h"
#include "mainwindow.h"
#include "gameobject.h"
#include "xyz.h"
#include "triangle.h"
#include "camera.h"
#include "constants.h"
#include "texturehandler.h"
#include "components.h"
#include "resourcemanager.h"
#include "soundsystem.h"
#include "coreengine.h"
#include "math_constants.h"
#include "meshhandler.h"    //to check linebox

RenderSystem::RenderSystem(const QSurfaceFormat &format, MainWindow *mainWindow)
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

RenderSystem::~RenderSystem()
{
}

// Sets up the general OpenGL stuff and the buffers needed to render a triangle
void RenderSystem::init()
{
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
    mShaderPrograms[0] = new ShaderHandler((gsl::ShaderFilePath + "plainvertex.vert").c_str(),
                                    (gsl::ShaderFilePath + "plainfragment.frag").c_str());
    qDebug() << "Plain shader program id: " << mShaderPrograms[0]->getProgram();

    mShaderPrograms[1] = new ShaderHandler((gsl::ShaderFilePath + "textureshader.vert").c_str(),
                                    (gsl::ShaderFilePath + "textureshader.frag").c_str());
    qDebug() << "Texture shader program id: " << mShaderPrograms[1]->getProgram();

    setupPlainShader(0);
    setupTextureShader(1);

    //********************** Making the object to be drawn **********************
    //Safe to do here because we know OpenGL is started
    //Probably should be placed elsewhere
    CoreEngine::getInstance()->setUpScene();

    //********************** Set up camera **********************
    //Done in CoreEngine->setUpScene
}

// Called each frame - doing the job of the RenderSystem!!!!!
void RenderSystem::render()
{
    mTimeStart.restart();   //restart FPS clock
    mVerticesDrawn = 0;     //reset vertex counter
    mObjectsDrawn = 0;      //reset object counter

    mContext->makeCurrent(this); //must be called every frame (every time mContext->swapBuffers is called)

    initializeOpenGLFunctions();    //must call this every frame it seems...

    //to clear the screen for each redraw
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(0); //reset shader type before rendering

    //Draws the objects
    for(int i{0}; i < mGameObjects.size(); i++)
    {
        /************** LOD and Frustum culling stuff ***********************/
        gsl::Vector3D cameraPos = mCurrentCamera->mPosition;
        gsl::Vector3D gobPos = mGameObjects[i]->mTransform->mMatrix.getPosition();
        gsl::Vector3D distanceVector = gobPos -cameraPos;

//        //Frustum cull calculation - that almost works. Have to be tweaked more to work properly
        float angle = gsl::rad2degf(acos(distanceVector.normalized() * mCurrentCamera->mForward.normalized()));
//        qDebug() << "angle:" << angle;    // <-qDebug() really kills performance

//        //if angle between camera Forward, and camera->GameObject > FOV of camera
        if(angle > mFOVangle)
            continue;   //don't draw object

        //LOD calculation
        float length = distanceVector.length();
//        qDebug() << "distance is" << length;
        /*************************************/


        //First object - xyz
        //what shader to use
        //Now mMaterial component holds index into mShaderPrograms!! - probably should be changed
        glUseProgram(mShaderPrograms[mGameObjects[i]->mMaterial->mShaderProgram]->getProgram() );

        /********************** REALLY, REALLY MAKE THIS ANTOHER WAY!!! *******************/

        //This block sets up the uniforms for the shader used in the material
        //Also sets up texture if needed.
        int viewMatrix{-1};
        int projectionMatrix{-1};
        int modelMatrix{-1};

        if (mGameObjects[i]->mMaterial->mShaderProgram == 0)
        {
            viewMatrix = vMatrixUniform;
            projectionMatrix = pMatrixUniform;
            modelMatrix = mMatrixUniform;
        }
        else if (mGameObjects[i]->mMaterial->mShaderProgram == 1)
        {
            viewMatrix = vMatrixUniform1;
            projectionMatrix = pMatrixUniform1;
            modelMatrix = mMatrixUniform1;

            //Now mMaterial component holds texture slot directly - probably should be changed
            glUniform1i(mTextureUniform, mGameObjects[i]->mMaterial->mTextureUnit);
        }
        /************ CHANGE THE ABOVE BLOCK !!!!!! ******************/

        //send data to shader
        glUniformMatrix4fv( viewMatrix, 1, GL_TRUE, mCurrentCamera->mViewMatrix.constData());
        glUniformMatrix4fv( projectionMatrix, 1, GL_TRUE, mCurrentCamera->mProjectionMatrix.constData());
        glUniformMatrix4fv( modelMatrix, 1, GL_TRUE, mGameObjects[i]->mTransform->mMatrix.constData());


        //draw the object
        //***Quick hack*** LOD test:
        if(mGameObjects[i]->mMesh->mVertexCount[1] > 0) //mesh has LODs
        {
            if (length < 4)
            {
                glBindVertexArray( mGameObjects[i]->mMesh->mVAO[0] );
                glDrawArrays(mGameObjects[i]->mMesh->mDrawType, 0, mGameObjects[i]->mMesh->mVertexCount[0]);
                mVerticesDrawn += mGameObjects[i]->mMesh->mVertexCount[0];
                mObjectsDrawn++;
            }
            else if(length < 6)
            {
                glBindVertexArray( mGameObjects[i]->mMesh->mVAO[1] );
                glDrawArrays(mGameObjects[i]->mMesh->mDrawType, 0, mGameObjects[i]->mMesh->mVertexCount[1]);
                mVerticesDrawn += mGameObjects[i]->mMesh->mVertexCount[1];
                mObjectsDrawn++;
            }
            else
            {
                glBindVertexArray( mGameObjects[i]->mMesh->mVAO[2] );
                glDrawArrays(mGameObjects[i]->mMesh->mDrawType, 0, mGameObjects[i]->mMesh->mVertexCount[2]);
                mVerticesDrawn += mGameObjects[i]->mMesh->mVertexCount[2];
                mObjectsDrawn++;
            }
        }
        else    //no LOD exists
        {
            glBindVertexArray( mGameObjects[i]->mMesh->mVAO[0] );
            glDrawArrays(mGameObjects[i]->mMesh->mDrawType, 0, mGameObjects[i]->mMesh->mVertexCount[0]);
            mVerticesDrawn += mGameObjects[i]->mMesh->mVertexCount[0];
            mObjectsDrawn++;
        }

        //Quick hack test to check if linebox/circle works:
        if(i == 1)
        {
            linebox = CoreEngine::getInstance()->mResourceManager->makeLineBox("suzanne.obj");
            MeshData circle = CoreEngine::getInstance()->mResourceManager->
                    makeCircleSphere(mGameObjects[i]->mMesh->mColliderRadius * 0.75, false);
//            glUniformMatrix4fv( modelMatrix, 1, GL_TRUE, gsl::Matrix4x4().identity().constData());
            glBindVertexArray( linebox.mVAO[0] );
            glDrawElements(linebox.mDrawType, linebox.mIndexCount[0], GL_UNSIGNED_INT, nullptr);
            glBindVertexArray( circle.mVAO[0] );
            glDrawElements(circle.mDrawType, circle.mIndexCount[0], GL_UNSIGNED_INT, nullptr);
        }
        if(i == 2)
        {
            linebox2 = CoreEngine::getInstance()->mResourceManager->makeLineBox("suzanne3.obj");
            MeshData circle = CoreEngine::getInstance()->mResourceManager->
                    makeCircleSphere(mGameObjects[i]->mMesh->mColliderRadius * 0.75, false);
//            glUniformMatrix4fv( modelMatrix, 1, GL_TRUE, gsl::Matrix4x4().identity().constData());
            glBindVertexArray( linebox2.mVAO[0] );
            glDrawElements(linebox2.mDrawType, linebox2.mIndexCount[0], GL_UNSIGNED_INT, nullptr);
            glBindVertexArray( circle.mVAO[0] );
            glDrawElements(circle.mDrawType, circle.mIndexCount[0], GL_UNSIGNED_INT, nullptr);
        }
        glBindVertexArray(0);

        CoreEngine::getInstance()->mResourceManager->checkCollision(linebox, linebox2);
    }

    //Moves the dog triangle - should be made another way!!!!
//    if(isPlaying)
//        mGameObjects[1]->mTransform->mMatrix.translate(.001f, .001f, -.001f); //just to move the triangle each frame

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

    glUseProgram(0); //reset shader type before next frame. Got rid of "Vertex shader in program _ is being recompiled based on GL state"
}

void RenderSystem::setupPlainShader(int shaderIndex)
{
    mMatrixUniform = glGetUniformLocation( mShaderPrograms[shaderIndex]->getProgram(), "mMatrix" );
    vMatrixUniform = glGetUniformLocation( mShaderPrograms[shaderIndex]->getProgram(), "vMatrix" );
    pMatrixUniform = glGetUniformLocation( mShaderPrograms[shaderIndex]->getProgram(), "pMatrix" );
}

void RenderSystem::setupTextureShader(int shaderIndex)
{
    mMatrixUniform1 = glGetUniformLocation( mShaderPrograms[shaderIndex]->getProgram(), "mMatrix" );
    vMatrixUniform1 = glGetUniformLocation( mShaderPrograms[shaderIndex]->getProgram(), "vMatrix" );
    pMatrixUniform1 = glGetUniformLocation( mShaderPrograms[shaderIndex]->getProgram(), "pMatrix" );
    mTextureUniform = glGetUniformLocation(mShaderPrograms[shaderIndex]->getProgram(), "textureSampler");
}

//This function is called from Qt when window is exposed (shown)
// and when it is resized
//exposeEvent is a overridden function from QWindow that we inherit from
void RenderSystem::exposeEvent(QExposeEvent *)
{
    //if not already initialized - run init() function
    if (!mInitialized)
        init();

    //This is just to support modern screens with "double" pixels (Macs and some 4k Windows laptops)
    const qreal retinaScale = devicePixelRatio();

    //Set viewport width and height
    glViewport(0, 0, static_cast<GLint>(width() * retinaScale), static_cast<GLint>(height() * retinaScale));

    //calculate aspect ration and set projection matrix
    mAspectratio = static_cast<float>(width()) / height();
    //    qDebug() << mAspectratio;
    mCurrentCamera->mProjectionMatrix.perspective(mFOVangle, mAspectratio, 0.1f, 100.f);
    //    qDebug() << mCamera.mProjectionMatrix;
}

//The way this is set up is that we start the clock before doing the draw call,
// and check the time right after it is finished (done in the render function)
//This will approximate what framerate we COULD have.
//The actual frame rate on your monitor is limited by the vsync and is probably 60Hz
void RenderSystem::calculateFramerate()
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
                                                  "FPS (approximated): " + QString::number(1E9 / nsecElapsed, 'g', 5) +
                                                  "  |  Objects drawn: " + QString::number(mObjectsDrawn) +
                                                  "  |  Vertices drawn: " + QString::number(mVerticesDrawn));                    ;
            frameCount = 0;     //reset to show a new message in 60 frames
        }
    }
}

//Simple way to turn on/off wireframe mode
//Not totally accurate, but draws the objects with
//lines instead of filled polygons
void RenderSystem::toggleWireframe(bool buttonState)
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

//Uses QOpenGLDebugLogger if this is present
//Reverts to glGetError() if not
void RenderSystem::checkForGLerrors()
{
    if(mOpenGLDebugLogger)
    {
        const QList<QOpenGLDebugMessage> messages = mOpenGLDebugLogger->loggedMessages();
        for (const QOpenGLDebugMessage &message : messages)
        {
            if (!(message.type() == message.OtherType)) // got rid of "object ...
                                                        //will use VIDEO memory as the source for buffer object operations"
                qDebug() << message;
        }
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
void RenderSystem::startOpenGLDebugger()
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

void RenderSystem::keyPressEvent(QKeyEvent *event)
{
    Input &input = CoreEngine::getInstance()->mInput;

    if (event->key() == Qt::Key_Escape) //Shuts down whole program
    {
        mMainWindow->close();
    }

    if (event->key() == Qt::Key_R) //toggle play
    {
        mMainWindow->on_pb_togglePlay_toggled(!isPlaying);
    }

    //    You get the keyboard input like this
    if(event->key() == Qt::Key_W)
    {
        input.W = true;
    }
    if(event->key() == Qt::Key_S)
    {
        input.S = true;
    }
    if(event->key() == Qt::Key_D)
    {
        input.D = true;
    }
    if(event->key() == Qt::Key_A)
    {
        input.A = true;
    }
    if(event->key() == Qt::Key_Q)
    {
        input.Q = true;
    }
    if(event->key() == Qt::Key_E)
    {
        input.E = true;
    }
    if(event->key() == Qt::Key_Z)
    {
    }
    if(event->key() == Qt::Key_X)
    {
    }
    if(event->key() == Qt::Key_Up)
    {
        input.UP = true;
    }
    if(event->key() == Qt::Key_Down)
    {
        input.DOWN = true;
    }
    if(event->key() == Qt::Key_Left)
    {
        input.LEFT = true;
    }
    if(event->key() == Qt::Key_Right)
    {
        input.RIGHT = true;
    }
    if(event->key() == Qt::Key_U)
    {
    }
    if(event->key() == Qt::Key_O)
    {
    }
}

void RenderSystem::keyReleaseEvent(QKeyEvent *event)
{
    Input &input = CoreEngine::getInstance()->mInput;

    if(event->key() == Qt::Key_W)
    {
        input.W = false;

    }
    if(event->key() == Qt::Key_S)
    {
        input.S = false;
    }
    if(event->key() == Qt::Key_D)
    {
        input.D = false;
    }
    if(event->key() == Qt::Key_A)
    {
        input.A = false;
    }
    if(event->key() == Qt::Key_Q)
    {
        input.Q = false;
    }
    if(event->key() == Qt::Key_E)
    {
        input.E = false;
    }
    if(event->key() == Qt::Key_Z)
    {
    }
    if(event->key() == Qt::Key_X)
    {
    }
    if(event->key() == Qt::Key_Up)
    {
        input.UP = false;
    }
    if(event->key() == Qt::Key_Down)
    {
        input.DOWN = false;
    }
    if(event->key() == Qt::Key_Left)
    {
        input.LEFT = false;
    }
    if(event->key() == Qt::Key_Right)
    {
        input.RIGHT = false;
    }
    if(event->key() == Qt::Key_U)
    {
    }
    if(event->key() == Qt::Key_O)
    {
    }
}

void RenderSystem::mousePressEvent(QMouseEvent *event)
{
    Input &input = CoreEngine::getInstance()->mInput;

    if (event->button() == Qt::RightButton)
        input.RMB = true;
    if (event->button() == Qt::LeftButton)
        input.LMB = true;
    if (event->button() == Qt::MiddleButton)
        input.MMB = true;
}

void RenderSystem::mouseReleaseEvent(QMouseEvent *event)
{
    Input &input = CoreEngine::getInstance()->mInput;

    if (event->button() == Qt::RightButton)
        input.RMB = false;
    if (event->button() == Qt::LeftButton)
        input.LMB = false;
    if (event->button() == Qt::MiddleButton)
        input.MMB = false;
}

void RenderSystem::wheelEvent(QWheelEvent *event)
{
    QPoint numDegrees = event->angleDelta() / 8;

    Input &input = CoreEngine::getInstance()->mInput;

    //if RMB, change the speed of the camera
    if (input.RMB)
    {
        if (numDegrees.y() < 1)
            mCurrentCamera->setCameraSpeed(0.001f);
        if (numDegrees.y() > 1)
            mCurrentCamera->setCameraSpeed(-0.001f);
    }
    event->accept();
}

void RenderSystem::mouseMoveEvent(QMouseEvent *event)
{
    Input &input = CoreEngine::getInstance()->mInput;

    if (input.RMB)
    {
        //Using mMouseXYlast as deltaXY so we don't need extra variables
        mMouseXlast = event->pos().x() - mMouseXlast;
        mMouseYlast = event->pos().y() - mMouseYlast;

        if (mMouseXlast != 0)
            mCurrentCamera->yaw(mCurrentCamera->mCameraRotateSpeed * mMouseXlast);
        if (mMouseYlast != 0)
            mCurrentCamera->pitch(mCurrentCamera->mCameraRotateSpeed * mMouseYlast);
    }
    mMouseXlast = event->pos().x();
    mMouseYlast = event->pos().y();
}
