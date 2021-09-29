#include "renderwindow.h"
#include <QTimer>
#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <QOpenGLDebugLogger>
#include <QKeyEvent>
#include <QStatusBar>
#include <QDebug>
#include <QtWidgets>
#include <QLabel>
#include <QWidget>


#include <iostream>

#include "visualobject.h"

#include "shader.h"
#include "mainwindow.h"
#include "visualobject.h"
#include "objects/xyz.h"
#include "objects/triangle.h"
#include "camera.h"
#include "constants.h"
#include "objects/objmesh.h"
#include "objects/cube.h"
#include "widget.h"
#include "systems/ecs/resourcemanager.h"

#include "texture.h"
#include "systems/ecs/Components.h"
#include "systems/ecs/entity.h"

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
       // resourceManager &resourceMgr = resourceManager::getInstance();

    auto start = std::chrono::high_resolution_clock::now();
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
    mTextures[2] = new Texture("goat.bmp");

    //Set the textures loaded to a texture unit
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, mTextures[0]->mGLTextureID);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, mTextures[1]->mGLTextureID);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, mTextures[2]->mGLTextureID);


    Entity *temp = mResourceManager->makeEntity("axis");
   // mEntity.push_back(temp);
   // temp->mMaterial->mShaderProgram = 0;


//    player = makeEntity("cube");
//    player->mMaterial->mShaderProgram = 0;

//    testObject = makeEntity("cube");
//    testObject->mMaterial->mShaderProgram = 0;
//    testObject->mTransform->mMatrix.translate(3,0,0);

//    resourceMgr.setOBBCollider(1, mEntity[1]->mTransform->position - mEntity[1]->mTransform->scale, mEntity[1]->mTransform->position + mEntity[1]->mTransform->scale);
//    resourceMgr.setOBBCollider(2, mEntity[2]->mTransform->position - mEntity[2]->mTransform->scale, mEntity[2]->mTransform->position + mEntity[2]->mTransform->scale);




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
    mCurrentCamera = new Camera();
    mCurrentCamera->setPosition(gsl::Vector3D(1.f, .5f, 4.f));

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float> duration = (end - start);
    qDebug() << "exec tid (RenderWindow::init()): "<<duration.count()*1000.f << "i ms";
}

// Called each frame - doing the rendering
void RenderWindow::render()
{

    mObjectsDrawn = 0;
    //Keyboard / mouse input - should be in a general game loop, not directly in the render loop
    handleInput();

    // Camera update - should be in a general game loop, not directly in the render loop
    mCurrentCamera->update();

    mTimeStart.restart(); //restart FPS clock
    mContext->makeCurrent(this); //must be called every frame (every time mContext->swapBuffers is called)

    initializeOpenGLFunctions();    //must call this every frame it seems...

    //to clear the screen for each redraw
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(0); //reset shader type before rendering

    //Draws the objects
    for(int i{0}; i < mEntity.size(); i++)
    {
        //First objekct - xyz
        //what shader to use
        //Now mMaterial component holds index into mShaderPrograms!! - probably should be changed
        glUseProgram(mShaderPrograms[mEntity[i]->mMaterial->mShaderProgram]->getProgram() );

        /********************** REALLY, REALLY MAKE THIS ANTOHER WAY!!! *******************/

        //This block sets up the uniforms for the shader used in the material
        //Also sets up texture if needed.
        int viewMatrix{-1};
        int projectionMatrix{-1};
        int modelMatrix{-1};

        if (mEntity[i]->mMaterial->mShaderProgram == 0)
        {
            viewMatrix = vMatrixUniform;
            projectionMatrix = pMatrixUniform;
            modelMatrix = mMatrixUniform;
        }
        else if (mEntity[i]->mMaterial->mShaderProgram == 1)
        {
            viewMatrix = vMatrixUniform1;
            projectionMatrix = pMatrixUniform1;
            modelMatrix = mMatrixUniform1;

            //Now mMaterial component holds texture slot directly - probably should be changed
            glUniform1i(mTextureUniform, mEntity[i]->mMaterial->mTextureUnit);
        }
        /************ CHANGE THE ABOVE BLOCK !!!!!! ******************/

        //send data to shader
        glUniformMatrix4fv( viewMatrix, 1, GL_TRUE, mCurrentCamera->mViewMatrix.constData());
        glUniformMatrix4fv( projectionMatrix, 1, GL_TRUE, mCurrentCamera->mProjectionMatrix.constData());
        glUniformMatrix4fv( modelMatrix, 1, GL_TRUE, mEntity[i]->mTransform->mMatrix.constData());


        //buggy frustum atm..

        if(toggleFrustum)
        {
            for(MAX_ENTITIES_TYPE i{1}; i < mEntity.size(); i++)
            {
                if(!mCurrentCamera->frustumCull(mEntity[i]->mTransform->position))
                     mEntity[i]->isHidden = true;
                else mEntity[i]->isHidden = false;
            }

        }
        if(!mEntity[i]->isHidden)
        {
        //draw the object
        glBindVertexArray( mEntity[i]->mMesh->mVAO );
        glDrawArrays(mEntity[i]->mMesh->mDrawType, 0, mEntity[i]->mMesh->mVertexCount);


        mObjectsDrawn++;
        }
        //if(i == 2)
        //{
        //MeshData lineBox = mResourceManager->makeLineBox("goat.obj");
        //MeshData circle = mResourceManager->makeCircleSphere(mEntity[i]->mMesh->mColliderRadius * 0.75, false);
        //glBindVertexArray( lineBox.mVAO );
        //glDrawElements(lineBox.mDrawType, lineBox.mIndexCount, GL_UNSIGNED_INT, nullptr);
        //glBindVertexArray( circle.mVAO );
        //glDrawElements(circle.mDrawType, circle.mIndexCount, GL_UNSIGNED_INT, nullptr);
        //}

        glBindVertexArray(0);

    }
        //collision test
//        for(MAX_ENTITIES_TYPE i{0}; i < mEntity.size(); i++)
//        {
//            if(resourceManager::getInstance().testCollision(player, testObject))
//               qDebug() << "collided ! ! ! ! ! ! ";
    //    }


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
    checkCollision();
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
    mTextureUniform = glGetUniformLocation(mShaderPrograms[shaderIndex]->getProgram(), "textureSampler");
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
    //    qDebug() << mAspectratio;
    mCurrentCamera->mProjectionMatrix.perspective(45.f, mAspectratio, 0.1f, 100.f);

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
                                                  "FPS (approximated): " + QString::number(1E9 / nsecElapsed, 'g', 7) + " Rendered objects: " + QString::number(mObjectsDrawn));
            frameCount = 0;     //reset to show a new message in 60 frames
        }

    }
}

void RenderWindow::checkRenderedObjects()
{
    if(mMainWindow)
    {
        mMainWindow->statusBar()->showMessage("objects rendered: ");
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

void RenderWindow::toogleVisibility(bool buttonState)
{

        if (buttonState)
        {
           toggleFrustum = true;
        }
       else
        {
            toggleFrustum = false;
       }

}

void RenderWindow::checkCollision()
{
    bool objectCollide = false;

//    objectCollide = testObject->CheckCollide(player->BoundingBoxMin, player->BoundingBoxMax);

//    if(objectCollide == true)
//    {
//        testObject->Collided();
//        gsl::Vector3D SkyHighBoundsMin{100.f,100.f,100.f};
//        gsl::Vector3D SkyHighBoundsMax{100.f,100.f,100.f};
//        testObject->setBoundingBox(SkyHighBoundsMin, SkyHighBoundsMax);

//        qDebug() << "collided ! ! ! ! ! ! ";
//    }

}

void RenderWindow::transformObjectX(double in)
{
     mEntity[1]->mTransform->mMatrix.rotateX(in *-1);
     //mEntity[1]->move();
}

void RenderWindow::transformObjectY(double in)
{
    mEntity[1]->mTransform->mMatrix.rotateY(in);
}

void RenderWindow::transformObjectZ(double in)
{
    mEntity[1]->mTransform->mMatrix.rotateZ(in);
}

//void RenderWindow::spawnObject(QString in)
//{

//    Entity *temp;
//    if(in == "cube")
//    {
//        temp = makeEntity("cube");
//        temp->mMaterial->mShaderProgram = 0;
//        temp->mTransform->mMatrix.translate(1.f, 0.f, -1.f);
//    }
//    else if(in == "triangle")
//    {
//        temp = makeEntity("triangle");
//        temp->mMaterial->mShaderProgram = 1;    //texture shader
//        temp->mMaterial->mTextureUnit = 1;      //dog texture
//        temp->mTransform->mMatrix.translate(0.f, 0.f, .5f);
//    }
//   else  if(in == "goat")
//    {
//        temp = makeEntity("\\Textures\\goat.obj");
//        temp->mMaterial->mShaderProgram = 1;    //texture shader
//        temp->mMaterial->mTextureUnit = 2;      //dog texture
//        temp->mTransform->mMatrix.translate(-3.f, 0.f, -3.f);
//    }
//    else
//        qDebug() << "no matching file to create";

//    mEntity.push_back(temp );

//}

//Entity *RenderWindow::makeEntity(std::string assetName, int shaderType, int textureUnit)
//{

//    Entity *temp = resourceManager::getInstance().makeEntity(assetName);
//    temp->mMaterial->mShaderProgram = shaderType;
//    temp->mMaterial->mTextureUnit = textureUnit;

//    //quick hackety-hack to move objects made
//    static float value = 0.f;

//    temp->mTransform->mMatrix.translate(value,value, value);
//    mEntity.push_back(temp);
//    value += 0.2f;
//    return temp;
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

    else
    {
        float playerSpeed = 0.05f;

        if(mInput.W)
        {
            //player->move(1.f,0,0);
            player->move(playerSpeed,0,0);

        }


        if(mInput.S)
        {
            //player->move(1.f,0,0);
            player->move(-playerSpeed,0,0);

        }
        if(mInput.A)
        {
            //player->move(1.f,0,0);
            player->move(0,0,-playerSpeed);

        }
        if(mInput.D)
        {
            //player->move(1.f,0,0);
            player->move(0,0,playerSpeed);

        }

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
    if(event->key() == Qt::Key_P)
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
            setCameraSpeed(0.001f);
        if (numDegrees.y() > 1)
            setCameraSpeed(-0.001f);
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
