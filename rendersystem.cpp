
#include "rendersystem.h"
#include <QTimer>
#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <QOpenGLDebugLogger>
#include <QKeyEvent>
#include <QStatusBar>
#include <QDebug>
#include <chrono>

#include "dos.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include "shaderhandler.h"
#include "mainwindow.h"
#include "gameobject.h"
#include "camera.h"
#include "constants.h"
#include "texturehandler.h"
#include "components.h"
#include "resourcemanager.h"
#include "soundsystem.h"
#include "coreengine.h"
#include "math_constants.h"
#include "vector4d.h"
#include "matrix4x4.h"
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
        glEnable(GL_CULL_FACE);       //draws only front side of models - usually what you want - test it out!
    glClearColor(0.4f, 0.4f, 0.4f,1.0f);    //gray color used in glClear GL_COLOR_BUFFER_BIT

    CoreEngine::getInstance()->setUpScene();
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
    mShaderPrograms[2] = new ShaderHandler((gsl::ShaderFilePath + "skyboxshader.vert").c_str(),
                                    (gsl::ShaderFilePath + "skyboxshader.frag").c_str());
    qDebug() << "Texture shader program id: " << mShaderPrograms[2]->getProgram();

    mShaderPrograms[3] = new ShaderHandler((gsl::ShaderFilePath + "phongshader.vert").c_str(),
                                    (gsl::ShaderFilePath + "phongshader.frag").c_str());
    qDebug() << "Texture shader program id: " << mShaderPrograms[3]->getProgram();

    //setup alle shaders
    setupPlainShader(0);
    setupTextureShader(1);
    setupSkyboxShader(2);
    setupLightShader(3);

    this->dt = 0;
    this->curTime = 0;
    this->lastTime = 0;

}

void RenderSystem::render()
{
    mTimeStart.restart();   //restart FPS clock
    mVerticesDrawn = 0;     //reset vertex counter
    mObjectsDrawn = 0;      //reset object counter
    mParticlesDrawn = 0;

    mContext->makeCurrent(this);
    initializeOpenGLFunctions();

    //klarer skjermen for hver redraw
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(0); //reset shader type before rendering

    for(unsigned int i{0}; i < mGameObjects.size(); i++)
    {
        ///lod og frustum
        gsl::Vector3D cameraPos = mCurrentCamera->mPosition;
        gsl::Vector3D gobPos = mGameObjects[i]->mTransform->mMatrix.getPosition();
        gsl::Vector3D distanceVector = gobPos -cameraPos;

        float angle = gsl::rad2degf(acos(distanceVector.normalized() * mCurrentCamera->mForward.normalized()));
        //if angle between camera Forward, and camera->GameObject > FOV of camera

        //sjekker frustum hvis objekte har det aktivert
        if(!toogleFrustumDrawing && mGameObjects[i]->useFrustum)
        {
                    if( angle > mFOVangle)
                        continue;   //ikke draw
        }





        ///LOD calculation
        float length = distanceVector.length();


        glUseProgram(mShaderPrograms[mGameObjects[i]->mMaterial->mShaderProgram]->getProgram() );

        //oppdaterer alle 4 forskjellige shader alternativene
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
            glUniform1i(mTextureUniform, mGameObjects[i]->mMaterial->mTextureUnit);
        }

        else if (mGameObjects[i]->mMaterial->mShaderProgram == 2)
        {
            viewMatrix = vMatrixUniformSS;
            projectionMatrix = pMatrixUniformSS;
            modelMatrix = mMatrixUniformSS;

            glUniform1i(mTextureUniformSS, mGameObjects[i]->mMaterial->mTextureUnit);
        }
        else if (mGameObjects[i]->mMaterial->mShaderProgram == 3)
        {

            ///setter opp lys
            viewMatrix = vMatrixUniform2;
            projectionMatrix = pMatrixUniform2;
            modelMatrix = mMatrixUniform2;
            glUniform1i(mTextureUniform2, mGameObjects[i]->mMaterial->mTextureUnit);
            glUniform1f(mAmbientStrengt, mAmbientStrengt);
            glUniform1f(mLightStrengt, 2);

        }


        ///sender data til shader
        glUniformMatrix4fv( viewMatrix, 1, GL_TRUE, mCurrentCamera->mViewMatrix.constData());
        glUniformMatrix4fv( projectionMatrix, 1, GL_TRUE, mCurrentCamera->mProjectionMatrix.constData());
        glUniformMatrix4fv( modelMatrix, 1, GL_TRUE, mGameObjects[i]->mTransform->mMatrix.constData());



        ///drawer objektene
        //sjekker hvem som skal drawes i bare play mode også
        if(mGameObjects[i]->RenderInPlaymode)
        {


            if(mGameObjects[i]->mMesh->mVertexCount[1] > 0) ///sjekker LOD levels
            {
                if (length < 8)
                {
                    glBindVertexArray( mGameObjects[i]->mMesh->mVAO[0] );
                    glDrawArrays(mGameObjects[i]->mMesh->mDrawType, 0, mGameObjects[i]->mMesh->mVertexCount[0]);
                    mVerticesDrawn += mGameObjects[i]->mMesh->mVertexCount[0];
                    mObjectsDrawn++;
                }
                else if(length < 12)
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
            else    //ingen LOD
            {
                glBindVertexArray( mGameObjects[i]->mMesh->mVAO[0] );
                glDrawArrays(mGameObjects[i]->mMesh->mDrawType, 0, mGameObjects[i]->mMesh->mVertexCount[0]);
                mVerticesDrawn += mGameObjects[i]->mMesh->mVertexCount[0];
                mObjectsDrawn++;
            }
        }


        ///lager linebox rundt objekte som er mousepicket
        if(i == mIndexToPickedObject)
        {
            linebox2 = CoreEngine::getInstance()->mResourceManager->makeLineBox("suzanne3.obj");
            glBindVertexArray( linebox2.mVAO[0] );
            glDrawElements(linebox2.mDrawType, linebox2.mIndexCount[0], GL_UNSIGNED_INT, nullptr);
        }


        srand( (float)time( NULL ) );
        //oppdaterer scenen hvis spille spilles og starter timeren
        if(CoreEngine::getInstance()->isPlaying == true)
        {

            CoreEngine::getInstance()->startTimer = true;
            CoreEngine::getInstance()->updateScene();


        }

        //oppdaterer alle partiklene
        for(unsigned int j{0}; j < mParticles.size(); j++)
        {

           glUseProgram(mShaderPrograms[mParticles[j]->mMaterial->mShaderProgram]->getProgram() );

           if (mParticles[j]->mMaterial->mShaderProgram == 0)
           {
               viewMatrix = vMatrixUniform;
               projectionMatrix = pMatrixUniform;
               modelMatrix = mMatrixUniform;
           }

           if(mParticles[j]->isAlive)
           {

            glUniformMatrix4fv( viewMatrix, 1, GL_TRUE, mCurrentCamera->mViewMatrix.constData());
            glUniformMatrix4fv( projectionMatrix, 1, GL_TRUE, mCurrentCamera->mProjectionMatrix.constData());
            glUniformMatrix4fv( modelMatrix, 1, GL_TRUE, mParticles[j]->mTransform->mMatrix.constData());

            glBindVertexArray( mParticles[j]->mMesh->mVAO[0] );
            glDrawArrays(mParticles[j]->mMesh->mDrawType, 0, mParticles[j]->mMesh->mVertexCount[0]);
            mVerticesDrawn += mParticles[j]->mMesh->mVertexCount[0];
            mParticlesDrawn ++;

            if(CoreEngine::getInstance()->isPlaying == true)
            {


                if(CoreEngine::getInstance()->particlesSpawned == true)
                {

                    ///beveger partiklene i en tilfeldig retning for å få en splash effekt
                    mParticles[j]->mTransform->mMatrix.translate(
                                                             ((float) rand()/(RAND_MAX / 1 )) - .5f,
                                                             ((float) rand()/(RAND_MAX / 1 )) - .5f,
                                                             ((float) rand()/(RAND_MAX / 1 )) - .5f
                                                            );


               }
                ///fjerner partiklene etter 1 sekund
                if(timer.elapsedSeconds() >= 1)
                {
                    mParticles[j]->isAlive = false;

                }

            }

           }


        }

        glBindVertexArray(0);
    }

    mMainWindow->UpdateScore(CoreEngine::getInstance()->score);

    Camera *tempcam = CoreEngine::getInstance()->mGameCamera;

    ///lager frustum
    MeshData frustum = CoreEngine::getInstance()->mResourceManager->makeFrustum(
                tempcam ->mFrustum
                );

    gsl::Matrix4x4 temp(true);
    temp.translate(tempcam->mPosition);
    temp.rotateY(-tempcam->mYaw);
    temp.rotateX(tempcam->mPitch);


    glBindVertexArray( frustum.mVAO[0] );

    if(toogleFrustumDrawing)
    {
        ///tegner frustum
        glDrawElements(frustum.mDrawType, frustum.mIndexCount[0], GL_UNSIGNED_INT, nullptr);


    }


    timerSetup(!CoreEngine::getInstance()->particleTimer);

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


void RenderSystem::rotateObj(double val)
{
    mGameObjects[1]->mTransform->mMatrix.rotateZ(val);
}

void RenderSystem::timerSetup(bool toggle)
{

    if(toggle)
    {
    timer.start();
    }

}

void RenderSystem::toggleFrustum(bool toggle)
{
    if(toggle)

        toogleFrustumDrawing = false;


    else
        toogleFrustumDrawing = true;

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

void RenderSystem::setupSkyboxShader(int shaderIndex)
{
    mMatrixUniformSS = glGetUniformLocation( mShaderPrograms[shaderIndex]->getProgram(), "mMatrix" );
    vMatrixUniformSS = glGetUniformLocation( mShaderPrograms[shaderIndex]->getProgram(), "vMatrix" );
    pMatrixUniformSS = glGetUniformLocation( mShaderPrograms[shaderIndex]->getProgram(), "pMatrix" );
    mTextureUniformSS = glGetUniformLocation( mShaderPrograms[shaderIndex]->getProgram(), "skybox");
}

void RenderSystem::setupLightShader(int shaderIndex)
{
    mMatrixUniform2 = glGetUniformLocation( mShaderPrograms[shaderIndex]->getProgram(), "mMatrix" );
    vMatrixUniform2 = glGetUniformLocation( mShaderPrograms[shaderIndex]->getProgram(), "vMatrix" );
    pMatrixUniform2 = glGetUniformLocation( mShaderPrograms[shaderIndex]->getProgram(), "pMatrix" );

    mCameraPosition = glGetUniformLocation( mShaderPrograms[shaderIndex]->getProgram(), "cameraPosition" );
    mLightPosition = glGetUniformLocation( mShaderPrograms[shaderIndex]->getProgram(), "lightPosition" );
    mLightDirection = glGetUniformLocation( mShaderPrograms[shaderIndex]->getProgram(), "lightDirection" );
    mLightColor = glGetUniformLocation( mShaderPrograms[shaderIndex]->getProgram(), "lightColor" );
    mObjectColor = glGetUniformLocation( mShaderPrograms[shaderIndex]->getProgram(), "objectColor" );
    mAmbientColor = glGetUniformLocation( mShaderPrograms[shaderIndex]->getProgram(), "ambientColor");
    mAmbientStrengt = glGetUniformLocation( mShaderPrograms[shaderIndex]->getProgram(), "ambientStrengt" );
    mLightStrengt = glGetUniformLocation( mShaderPrograms[shaderIndex]->getProgram(), "lightStrengt" );
    mSpecularStrength = glGetUniformLocation( mShaderPrograms[shaderIndex]->getProgram(), "specularStrength" );
    mSpecularExponent = glGetUniformLocation( mShaderPrograms[shaderIndex]->getProgram(), "specularExponent" );

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
                                                  "  |  Vertices drawn: " + QString::number(mVerticesDrawn) +
                                                  "  |  Particles drawn: " + QString::number(mParticlesDrawn));                    ;
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
    {
        input.LMB = false;
        if(!CoreEngine::getInstance()->isPlaying)
        mousePicking(event);
    }

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



void RenderSystem::mousePicking(QMouseEvent *event)
{
    float accuracy = .6f;
    int mouseXpos = event->pos().x();
    int mouseYpos = event->pos().y();


    gsl::Matrix4x4 projectionMatrix = mCurrentCamera->mProjectionMatrix;
    gsl::Matrix4x4 viewMatrix = mCurrentCamera->mViewMatrix;


    float x = (2.0f * mouseXpos) / width() - 1.0f;
    float y = 1.0f - (2.0f * mouseYpos) / height();
    float z = 1.0f;
    gsl::Vector3D ray_nds = gsl::Vector3D(x, y, z);

    gsl::Vector4D ray_clip = gsl::Vector4D(ray_nds.x, ray_nds.y, -1.0, 1.0);

    projectionMatrix.inverse();
    gsl::Vector4D ray_eye = projectionMatrix * ray_clip;
    ray_eye = gsl::Vector4D(ray_eye.x, ray_eye.y, -1.0, 0.0);


    viewMatrix.inverse();
    gsl::Vector4D temp = viewMatrix * ray_eye;
    gsl::Vector3D ray_wor = {temp.x, temp.y, temp.z};
    ray_wor.normalize();

    for(unsigned int i{0}; i < mGameObjects.size(); i++)
    {
        //vektor mellom objekt og kamera
        gsl::Vector3D tempObject = mGameObjects[i]->mTransform->mMatrix.getPosition() - mCurrentCamera->mPosition;
        //lage normalen av ray i forhold til tempobjects vektor
        // krossprodukt
        gsl::Vector3D planeNormal = gsl::Vector3D::cross(ray_wor, tempObject);

        gsl::Vector3D rayNormal = planeNormal ^ ray_wor;
        rayNormal.normalize();

        //dot produkt
        float distance = gsl::Vector3D::dot(tempObject, rayNormal);

        //absolutt verdi, ikke intresert i negative resultater
        distance = abs(distance);

        //sjekker kollisjon
        if(distance < accuracy)
        {
            qDebug() << "Item picked " << i;
            //Legge til funskjonalitet får oppdatere item lista i ui
            mIndexToPickedObject = i;
            break;
        }
        else
            mIndexToPickedObject = -1;

    }
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
