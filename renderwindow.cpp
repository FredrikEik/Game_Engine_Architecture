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
#include "visualobject.h"
#include "xyz.h"
#include "camera.h"
#include "constants.h"
#include "texturehandler.h"


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

    //Make the gameloop timer:
    mRenderTimer = new QTimer(this);
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
    mTextures[2] = new Texture("Skyb", true);

    //Set the textures loaded to a texture unit
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, mTextures[0]->mGLTextureID);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, mTextures[1]->mGLTextureID);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_CUBE_MAP, mTextures[2]->mGLTextureID);


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

    mShaderPrograms[2] = new Shader((gsl::ShaderFilePath + "phongvertex.vert").c_str(),
                                    (gsl::ShaderFilePath + "phongfragment.frag").c_str());
    qDebug() << "Phong shader program id: " << mShaderPrograms[2]->getProgram();

    mShaderPrograms[3] = new Shader((gsl::ShaderFilePath + "skyboxvertex.vert").c_str(),
                                    (gsl::ShaderFilePath + "skyboxfragment.frag").c_str());
    qDebug() << "Phong shader program id: " << mShaderPrograms[3]->getProgram();


    setupPlainShader(0);
    setupTextureShader(1);
    setupPhongShader(2);
    setupSkyboxShader(3);


    //********************** Set up camera **********************
    mEditorCamera.setPosition(gsl::Vector3D(1.f, 5.f, 8.f));
    mPlayCamera.setPosition(gsl::Vector3D(9.f, 13.f, 11.f));
    mPlayCamera.pitch(90);

    mCurrentCamera = &mEditorCamera;


    //********************** create input **********************
    mInputComponent = new InputComponent();
    mInputSystem = new InputSystem();
    mCollisionSystem = new CollisionSystem();

    mLvl = new Level(&mPlayCamera);
    mSkyBox = mLvl->mSkyBox;
    mLight = mLvl->mLight;
    mPlayer = mLvl->mPlayer;
    mMainWindow->update();


}


void RenderWindow::drawObjects()
{
    //This block sets up the uniforms for the shader used in the material
    //Also sets up texture if needed.
    int viewMatrix{-1};
    int projectionMatrix{-1};
    int modelMatrix{-1};
    //Draws the objects
    for(int i{0}; i < (int)mLvl->mVisualObjects.size(); i++)
    {
        glUseProgram(mShaderPrograms[mLvl->mVisualObjects[i]->mMaterial->mShaderProgram]->getProgram() );

        if (mLvl->mVisualObjects[i]->mMaterial->mShaderProgram == 0) //PlainShader
        {
            viewMatrix = vMatrixUniform;
            projectionMatrix = pMatrixUniform;
            modelMatrix = mMatrixUniform;
        }
        else if (mLvl->mVisualObjects[i]->mMaterial->mShaderProgram == 1)//TextureShader
        {
            viewMatrix = vMatrixUniform1;
            projectionMatrix = pMatrixUniform1;
            modelMatrix = mMatrixUniform1;
            glUniform1i(mTextureUniform, mLvl->mVisualObjects[i]->mMaterial->mTextureUnit);
        }
        else if (mLvl->mVisualObjects[i]->mMaterial->mShaderProgram == 2)//PhongShader
        {
            viewMatrix = vMatrixUniform2;
            projectionMatrix = pMatrixUniform2;
            modelMatrix = mMatrixUniform2;
            glUniform3f(mLightPositionUniform, mLight->mTransform->mMatrix.getPosition().x, mLight->mTransform->mMatrix.getPosition().y, mLight->mTransform->mMatrix.getPosition().y);
            glUniform3f(mCameraPositionUniform, mCurrentCamera->position().x, mCurrentCamera->position().y, mCurrentCamera->position().z);
            glUniform3f(mLightColorUniform, mLight->mLightColor.x(), mLight->mLightColor.y(), mLight->mLightColor.z());
        }
        else if (mLvl->mVisualObjects[i]->mMaterial->mShaderProgram == 3)//SkyboxShader
        {
            viewMatrix = vMatrixUniform3;
            projectionMatrix = pMatrixUniform3;
            modelMatrix = mMatrixUniform3;

            glUniform1i(mTextureUniform3, mLvl->mVisualObjects[i]->mMaterial->mTextureUnit);

        }

        //send data to shader
        glUniformMatrix4fv( viewMatrix, 1, GL_TRUE, mCurrentCamera->mViewMatrix.constData());
        glUniformMatrix4fv( projectionMatrix, 1, GL_TRUE, mCurrentCamera->mProjectionMatrix.constData());
        glUniformMatrix4fv( modelMatrix, 1, GL_TRUE, mLvl->mVisualObjects[i]->mTransform->mMatrix.constData());

        if(mLvl->mVisualObjects[i]->drawMe == true)
        {
            glBindVertexArray(mLvl->mVisualObjects[i]->mMesh->mVAO );
            glDrawArrays(mLvl->mVisualObjects[i]->mMesh->mDrawType, 0, Lod(i));
            glBindVertexArray(0);
        }

    }

    glUniformMatrix4fv( modelMatrix, 1, GL_TRUE, mLight->mTransform->mMatrix.constData());
    glBindVertexArray(mLight->mMesh->mVAO );
    glDrawArrays(mLight->mMesh->mDrawType, 0, mLight->mMesh->mVertices.size());
    glBindVertexArray(0);

    glUniformMatrix4fv( modelMatrix, 1, GL_TRUE, mSkyBox->mTransform->mMatrix.constData());
    glBindVertexArray(mSkyBox->mMesh->mVAO );
    glDrawArrays(mSkyBox->mMesh->mDrawType, 0, mSkyBox->mMesh->mVertices.size());
    glBindVertexArray(0);

    if(playM==false){
        glUniformMatrix4fv( modelMatrix, 1, GL_TRUE, mLvl->mFrustumSystem->mTransform->mMatrix.constData());
        glBindVertexArray(mLvl->mFrustumSystem->mMesh->mVAO );
        glDrawArrays(mLvl->mFrustumSystem->mMesh->mDrawType, 0, mLvl->mFrustumSystem->mMesh->mVertices.size());
        glBindVertexArray(0);}
}



// Called each frame - doing the rendering
void RenderWindow::render()
{


    mTimeStart.restart(); //restart FPS clock
    mContext->makeCurrent(this); //must be called every frame (every time mContext->swapBuffers is called)

    initializeOpenGLFunctions();    //must call this every frame it seems...

    // HandleInput();
    mInputSystem->update(mPlayer,mCurrentCamera,mInput);
    mCurrentCamera->update();
    mLvl->checkCollision();
    int a = rand() % 20;
    if(playM)
    {
        mLvl->moveEnemy(a);
    }
    //to clear the screen for each redraw
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    drawObjects();


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
}

std::string RenderWindow::createShapes(string shapeID)
{
    VisualObject* temp = mShapeFactory.createShape(shapeID);
    temp->init();
    temp->move(1,1,0.5);
    temp->mMaterial->mShaderProgram = 0;    //plain shader
    mLvl->mTransformComp.push_back(temp->mTransform);
    mLvl->mNameComp.push_back(temp->mNameComp);
    mVisualObjects.push_back(temp);
    return temp->mNameComp->ObjectName;
}




int RenderWindow::Lod(int i)
{

    return mLvl->mVisualObjects[i]->mMesh->mVertices.size();
//    if(i!=8 && i!=9 )
//    {
//        gsl::Vector3D camToObject = mVisualObjects[i]->mTransform->mMatrix.getPosition() - mCurrentCamera->position();
//        //Testing lenght from cam to object
//        if(camToObject.length()>5 && mVisualObjects[i]->mMesh->mVertices.size()>60) /*||mCollisionSystem->CheckMousePickCollision(distance, mPlayer->mCollision)*/
//        {
//             return mVisualObjects[i]->mMesh->mVertices.size()/2;
//        }else if(camToObject.length()>10 && mVisualObjects[i]->mMesh->mVertices.size()>60)
//        {
//            return mVisualObjects[i]->mMesh->mVertices.size()/4;
//        }else
//            return mVisualObjects[i]->mMesh->mVertices.size();
//    }else
//        return mVisualObjects[i]->mMesh->mVertices.size();


}


void RenderWindow::mousePickingRay(QMouseEvent *event)
{
    int mouseXPixel = event->pos().x();
    int mouseYPixel = event->pos().y(); //y is 0 at top of screen!

    gsl::Matrix4x4 projMatrix = mCurrentCamera->mProjectionMatrix;
    gsl::Matrix4x4 viewMatrix = mCurrentCamera->mViewMatrix;

    //step 1
    float x = (2.0f * mouseXPixel) / width() - 1.0f;
    float y = 1.0f - (2.0f * mouseYPixel) / height();
    float z = 1.0f;
    gsl::Vector3D ray_nds = gsl::Vector3D(x, y, z);

    //step 2
    gsl::Vector4D ray_clip = gsl::Vector4D(ray_nds.x, ray_nds.y, -1.0, 1.0);

    //step 3
    // projMatrix.inverse();
    gsl::Vector4D ray_eye = projMatrix * ray_clip;
    ray_eye = gsl::Vector4D(ray_eye.x, ray_eye.y, -1.0, 0.0);

    //step 4
    viewMatrix.inverse();
    gsl::Vector4D temp = viewMatrix * ray_eye;
    ray_wor = {temp.x, temp.y, temp.z};
    ray_wor.normalize();

    qDebug() << ray_wor;


    for(int i{0}; i < mVisualObjects.size(); i++)

    {      //making the vector from camera to object we test against
        gsl::Vector3D camToObject = mVisualObjects[i]->mTransform->mMatrix.getPosition() - mCurrentCamera->position();

        //making the normal of the ray - in relation to the camToObject vector
        //this is the normal of the surface the camToObject and ray_wor makes:
        gsl::Vector3D planeNormal = ray_wor ^ camToObject;    //^ gives the cross product

        //this will now give us the normal vector of the ray - that lays in the plane of the ray_wor and camToObject
        gsl::Vector3D rayNormal = planeNormal ^ ray_wor;
        rayNormal.normalize();

        //now I just project the camToObject vector down on the rayNormal == distance from object to ray
        //getting distance from GameObject to ray using dot product:
        float distance = camToObject * rayNormal;   //* gives the dot product

        //we are interested in the absolute distance, so fixes any negative numbers
        distance = abs(distance);

        if(mCollisionSystem->CheckMousePickCollision(distance, mVisualObjects[i]->mCollision) /*||mCollisionSystem->CheckMousePickCollision(distance, mPlayer->mCollision)*/)
        {
            mousePickCollide = true;
            mMainWindow->SelectWithMousePick(i);

            MousePickindex = i;
            qDebug() <<"Mouse Collision detected";
        }

    }

}

void RenderWindow::playMode(bool p)
{
    if(p)
    {
        mCurrentCamera = &mPlayCamera;
        //mLaserSound->play();
        playM = true;
    }
    else
    {
        mCurrentCamera = &mEditorCamera;
        //mLaserSound->stop();
        playM = false;
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

}


void RenderWindow::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::RightButton)
        if(playM == false)
            mInput.RMB = true;
    if (event->button() == Qt::LeftButton)
    {
        mInput.LMB = true;
        mousePickingRay(event);
    }

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

void RenderWindow::mouseMoveEvent(QMouseEvent *event)
{
    if (mInput.RMB)
    {
        //Using mMouseXYlast as deltaXY so we don't need extra variables
        mInputComponent->mMouseXlast = event->pos().x() - mInputComponent->mMouseXlast;
        mInputComponent-> mMouseYlast = event->pos().y() - mInputComponent->mMouseYlast;

        if (mInputComponent->mMouseXlast != 0)
            mEditorCamera.yaw(mInputComponent->mCameraRotateSpeed * mInputComponent->mMouseXlast);
        if (mInputComponent->mMouseYlast != 0)
            mEditorCamera.pitch(mInputComponent->mCameraRotateSpeed * mInputComponent->mMouseYlast);
    }
    mInputComponent->mMouseXlast = event->pos().x();
    mInputComponent->mMouseYlast = event->pos().y();

}


void RenderWindow::wheelEvent(QWheelEvent *event)
{
    QPoint numDegrees = event->angleDelta() / 8;

    //if RMB, change the speed of the camera
    if (mInput.RMB)
    {
        if (numDegrees.y() < 1)
            mCurrentCamera->setSpeed(0.001f);
        if (numDegrees.y() > 1)
            mInputSystem->setCameraSpeed(mCurrentCamera,-0.001f);
    }
    event->accept();
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
    mTextureUniform = glGetUniformLocation(mShaderPrograms[shaderIndex]->getProgram(), "textureSampler");
}


void RenderWindow::setupPhongShader(int shaderIndex)
{
    mMatrixUniform2 = glGetUniformLocation( mShaderPrograms[shaderIndex]->getProgram(), "mMatrix" );
    vMatrixUniform2 = glGetUniformLocation( mShaderPrograms[shaderIndex]->getProgram(), "vMatrix" );
    pMatrixUniform2 = glGetUniformLocation( mShaderPrograms[shaderIndex]->getProgram(), "pMatrix" );


  //  mUsingTextureUniform = glGetUniformLocation( mShaderPrograms[shaderIndex]->getProgram(), "usingTextures" );
    mLightColorUniform = glGetUniformLocation( mShaderPrograms[shaderIndex]->getProgram(), "lightColor" );
    mObjectColorUniform = glGetUniformLocation( mShaderPrograms[shaderIndex]->getProgram(), "objectColor" );
    mAmbientLightStrengthUniform = glGetUniformLocation( mShaderPrograms[shaderIndex]->getProgram(), "ambientStrengt" );
    mLightPositionUniform = glGetUniformLocation( mShaderPrograms[shaderIndex]->getProgram(), "lightPosition" );
    mSpecularStrengthUniform = glGetUniformLocation( mShaderPrograms[shaderIndex]->getProgram(), "specularStrength" );
    mSpecularExponentUniform = glGetUniformLocation( mShaderPrograms[shaderIndex]->getProgram(), "specularExponent" );
    mLightPowerUniform = glGetUniformLocation( mShaderPrograms[shaderIndex]->getProgram(), "lightStrengt" );
    mCameraPositionUniform = glGetUniformLocation( mShaderPrograms[shaderIndex]->getProgram(), "cameraPosition" );
    mTextureUniformPhong = glGetUniformLocation(mShaderPrograms[shaderIndex]->getProgram(), "textureSampler");
}

void RenderWindow::setupSkyboxShader(int shaderIndex)
{
    mMatrixUniform3 = glGetUniformLocation( mShaderPrograms[shaderIndex]->getProgram(), "mMatrix" );
    vMatrixUniform3 = glGetUniformLocation( mShaderPrograms[shaderIndex]->getProgram(), "vMatrix" );
    pMatrixUniform3 = glGetUniformLocation(mShaderPrograms[shaderIndex]->getProgram(), "pMatrix" );
    mTextureUniform3 = glGetUniformLocation(mShaderPrograms[shaderIndex]->getProgram(), "skybox");
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

    //mEditorCamera.calculateFrustumVectors();
    //calculate aspect ration and set projection matrix
    mPlayCamera.mFrustumComp.mAspectRatio = static_cast<float>(width()) / height();
    //    qDebug() << mAspectratio;
    mPlayCamera.calculateProjectionMatrix();
    mEditorCamera.calculateProjectionMatrix();
}
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
                                                  "FPS (approximated): " + QString::number(1E9 / nsecElapsed, 'g', 7));
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
        //glDisable(GL_CULL_FACE);
    }
    else
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);    //turn off wireframe mode
        //glEnable(GL_CULL_FACE);
    }
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
