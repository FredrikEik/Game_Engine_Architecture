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
#include "triangle.h"
#include "camera.h"
#include "constants.h"
#include "texture.h"
#include "components.h"
#include "rendersystem.h"

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
    SoundManager::getInstance()->cleanUp();
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

    //********************** Making the object to be drawn **********************

    /****************** THIS SHOULD USE A RESOURCE MANAGER / OBJECT FACTORY!!!!! ******************************************/
    /***** should not use separate classes init() - function ****************/

    //Axis
    VisualObject *temp = new XYZ();
    temp->mMaterial->mShaderProgram = 0; //plain shader
    temp->init();
    mVisualObjects.push_back(temp);
    ////*************************************start**////////////

    TriangleTransform = new TransformComponent();


    // Positions            // Colors       //UV
    TriangleTransform->mMatrix.setToIdentity();

    TriangelMesh = new MeshComponent() ;
    TriangelMesh->mVertices.push_back(Vertex{-0.5f, -0.5f, 0.0f,   1.0f, 0.0f, 0.0f,  0.f, 0.f}); // Bottom Left
    TriangelMesh->mVertices.push_back(Vertex{0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,    1.0f, 0.f}); // Bottom Right
    TriangelMesh->mVertices.push_back(Vertex{0.0f,  0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.5f, 1.f}); // Top
    TriangelMesh->mDrawType = GL_TRIANGLES;
    TriangleMaterial = new MaterialComponent();
    RenderSys->init(TriangelMesh);


    TriangleMaterial->mShaderProgram = 1;
    TriangleMaterial->mTextureUnit = 1;

    TriangleTransform->mMatrix.translate(0.f, 0.f, .5f);

    ////*************************************start**////////////


    ///PURE ECS TEST
    entities.push_back(0);

    TransformComponent * Susy = new TransformComponent();
    Susy->mMatrix.setToIdentity();
    Susy->entity = 0;
    Susy->mMatrix.translate(0.f, 0.f, .9f);


    transformCompVec.push_back(Susy);
    MeshComponent * SusyMesh = new MeshComponent();
    MeshSys->CreateMeshComponent("Suzanne.obj", SusyMesh);
    meshCompVec.push_back(SusyMesh);

    MaterialComponent * SusyMat = new MaterialComponent();
    SusyMat->entity = 0;
    SusyMat->mShaderProgram = 0;
    SusyMat->mTextureUnit = 0;
    MaterialCompVec.push_back(SusyMat);




    RenderSys->init(meshCompVec[0]);
    ///

    ///PURE ECS TEST
    entities.push_back(1);

    TransformComponent * headTran = new TransformComponent();
    headTran->mMatrix.setToIdentity();
    headTran->entity = 1;
    headTran->mMatrix.translate(0.f, 0.f, .11f);

    transformCompVec.push_back(headTran);
    MeshComponent * headMesh = new MeshComponent();
    MeshSys->CreateMeshComponent("head.obj", headMesh);
    headMesh->entity = 1;
    meshCompVec.push_back(headMesh);

    MaterialComponent * headMat = new MaterialComponent();
    headMat->entity = 1;
    headMat->mShaderProgram = 0;
    headMat->mTextureUnit = 0;
    MaterialCompVec.push_back(headMat);


    RenderSys->init(meshCompVec[1]);
    ///


    //entities.push_back(2);
    //MaterialComponent * matc1 = new MaterialComponent();
   // MeshComponent * mc1 = new MeshComponent();
    //TransformComponent * tc1 = new TransformComponent();
    //entitySys->construct(2, tc1, mc1, matc1, "Suzanne.obj", MeshSys, meshCompVec, transformCompVec, MaterialCompVec, QVector3D(0.f, 0.f, 0.f),0,0);
    //RenderSys->init(mc1);

    entitySys->construct("Suzanne.obj", QVector3D(0.0f,0.0f,0.0f),0,0,2);
    entitySys->construct("plane.obj", QVector3D(-5.0f,0.0f,0.0f),0,0,2);
    entitySys->construct("sphere.obj", QVector3D(5.0f,0.0f,0.0f),0,0);

    SoundManager::getInstance()->init();

    mExplosionSound = SoundManager::getInstance()->createSource(
                "Explosion", Vector3(10.0f, 0.0f, 0.0f),
                "../GEA2021/Assets/Audio/explosion.wav", false, 1.0f);
    mLaserSound = SoundManager::getInstance()->createSource(
                "Laser", Vector3(20.0f, 0.0f, 0.0f),
                "../GEA2021/Assets/Audio/laser.wav", true, 0.7f);

    mStereoSound = SoundManager::getInstance()->createSource(
                "Stereo", Vector3(0.0f, 0.0f, 0.0f),
                "../GEA2021/Assets/Audio/stereo.wav", false, 1.0f);

    mSong = SoundManager::getInstance()->createSource(
                "Caravan", Vector3(0.0f, 0.0f, 0.0f),
                "../GEA2021/Assets/Audio/Caravan_mono.wav", false, 1.0f);


    //dog triangle
    temp = new Triangle();
    temp->init();
    temp->mMaterial->mShaderProgram = 1;    //texture shader
    temp->mMaterial->mTextureUnit = 1;      //dog texture
    temp->mTransform->mMatrix.translate(0.f, 0.f, .5f);
    mVisualObjects.push_back(temp);

    //********************** Set up camera **********************
    mCurrentCamera = new Camera();
    mCurrentCamera->setPosition(gsl::Vector3D(1.f, .5f, 4.f));

    mSong->play();
    mMainWindow->updateViewPort();
}

// Called each frame - doing the job of the RenderSystem!!!!!
void RenderWindow::render()
{

    //


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


    SoundManager::getInstance()->updateListener(mCurrentCamera->position(), gsl::Vector3D(0,0,0), mCurrentCamera->forward(), mCurrentCamera->up());
/*
    //LOD SWITCHER - OLE PLS DONT HATE
    //calc length between obj and camera.
    //use length to switch LOD level
    //use length and LOD level to switch only one time
    gsl::Vector3D LODlength = transformCompVec[2]->mMatrix.getPosition() - mCurrentCamera->position();

    if((LODlength.length() < 5.0f) && (meshCompVec[2]->LOD0 == false) && meshCompVec[2]->LODLevel != 0)
    {
        qDebug() << "LOD level 0";
        //LOD LEVEL 0
        meshCompVec[2]->LODLevel = 0;
        entitySys->LODSuzanneSwithcer(meshCompVec[2]);
        RenderSys->init(meshCompVec[2]);
        meshCompVec[2]->LOD0 = true;
        meshCompVec[2]->LOD1 = false;
        meshCompVec[2]->LOD2 = false;
    }
    else if((LODlength.length() >= 5.0f && LODlength.length() < 10.0f) && (meshCompVec[2]->LOD1 == false) && meshCompVec[2]->LODLevel != 1)
    {
        qDebug() << "LOD level 1";
        //LOD LEVEL 1
        meshCompVec[2]->LODLevel = 1;
        entitySys->LODSuzanneSwithcer(meshCompVec[2]);
        RenderSys->init(meshCompVec[2]);
        meshCompVec[2]->LOD0 = false;
        meshCompVec[2]->LOD1 = true;
        meshCompVec[2]->LOD2 = false;
    }
    else if((LODlength.length() >= 10.0f && LODlength.length() < 20.0f) && (meshCompVec[2]->LOD2 == false) && meshCompVec[2]->LODLevel != 2)
    {
        qDebug() << "LOD level 2";
        //LOD LEVEL 2
        meshCompVec[2]->LODLevel = 2;
        entitySys->LODSuzanneSwithcer(meshCompVec[2]);
        RenderSys->init(meshCompVec[2]);
        meshCompVec[2]->LOD0 = false;
        meshCompVec[2]->LOD1 = false;
        meshCompVec[2]->LOD2 = true;
    }
*/


    //Draws the objects
    for(int i{0}; i < mVisualObjects.size(); i++)
    {
        //First objekct - xyz
        //what shader to use
        //Now mMaterial component holds index into mShaderPrograms!! - probably should be changed
        glUseProgram(mShaderPrograms[mVisualObjects[i]->mMaterial->mShaderProgram]->getProgram() );

        /********************** REALLY, REALLY MAKE THIS ANTOHER WAY!!! *******************/

        //This block sets up the uniforms for the shader used in the material
        //Also sets up texture if needed.
        int viewMatrix{-1};
        int projectionMatrix{-1};
        int modelMatrix{-1};

        if (mVisualObjects[i]->mMaterial->mShaderProgram == 0)
        {
            viewMatrix = vMatrixUniform;
            projectionMatrix = pMatrixUniform;
            modelMatrix = mMatrixUniform;
        }
        else if (mVisualObjects[i]->mMaterial->mShaderProgram == 1)
        {
            viewMatrix = vMatrixUniform1;
            projectionMatrix = pMatrixUniform1;
            modelMatrix = mMatrixUniform1;

            //Now mMaterial component holds texture slot directly - probably should be changed
            glUniform1i(mTextureUniform, mVisualObjects[i]->mMaterial->mTextureUnit);
        }
        /************ CHANGE THE ABOVE BLOCK !!!!!! ******************/

        //send data to shader
        glUniformMatrix4fv( viewMatrix, 1, GL_TRUE, mCurrentCamera->Cam.mViewMatrix.constData());
        glUniformMatrix4fv( projectionMatrix, 1, GL_TRUE, mCurrentCamera->Cam.mProjectionMatrix.constData());
        glUniformMatrix4fv( modelMatrix, 1, GL_TRUE, mVisualObjects[i]->mTransform->mMatrix.constData());

        //draw the object
        glBindVertexArray( mVisualObjects[i]->mMesh->mVAO );
        glDrawArrays(mVisualObjects[i]->mMesh->mDrawType, 0, mVisualObjects[i]->mMesh->mVertices.size());
        glBindVertexArray(0);
    }

    int eSize = entities.size();
    for(int i = 0; i < eSize; i++){
        if(entities[i] == meshCompVec[i]->entity && entities[i] == transformCompVec[i]->entity && entities[i] == MaterialCompVec[i]->entity){

            glUseProgram(mShaderPrograms[MaterialCompVec[i]->mShaderProgram]->getProgram());
            RenderSys->draw(meshCompVec[i],
                    MaterialCompVec[i],
                    transformCompVec[i],
                    vMatrixUniform,
                    pMatrixUniform,
                    mMatrixUniform,
                    mCurrentCamera);
            //----------------------------------------------------
            //HARDCODED COLLIDER BABY
            //monkey thats moving is entity id 2
            if(transformCompVec[2]->entity == 2 && meshCompVec[2]->entity == 2){
                if(collisionSys->isColliding(meshCompVec[2],transformCompVec[2],meshCompVec[i],transformCompVec[i]))
                {
                    //run collision code
                    if(meshCompVec[i]->entity != 2)
                        meshCompVec[i]->isDrawable = false;
                }
            }
            //------------------------------------------------------
        }
    }
    if(isPaused){

    }else{
        for(int i = 0; i < eSize; i++){
            if(transformCompVec[i]->entity == 2){
                transformCompVec[i]->mMatrix.translate(0.002f, 0.f,0.f);
                mSong->setPosition(transformCompVec[i]->mMatrix.getPosition());
            }
        }
    }



    /*
    for(int i = 0; i < eSize; i++){
        qDebug() << entities[i];
    }
    qDebug() << "\n";
    */

    //RENDER TRIANGLE
    //GLUSEPROGRAM GETS THE CURRENT MATERIAL
    glUseProgram(mShaderPrograms[TriangleMaterial->mShaderProgram]->getProgram());

    RenderSys->draw(TriangelMesh,       //Meshcomponent
                    TriangleMaterial,   //MaterialComponent
                    TriangleTransform,  //TransformComponent
                    vMatrixUniform,     //viewMatrix
                    pMatrixUniform,     //Projection matrix
                    mMatrixUniform,     //ModelMatrix
                    mCurrentCamera );   //Camera


    //for(auto i = entities->begin(); i < entities->end(); i++){


    // }


    //Moves the dog triangle - should be mada another way!!!!
    mVisualObjects[1]->mTransform->mMatrix.translate(.001f, .001f, -.001f);     //just to move the triangle each frame
    TriangleTransform->mMatrix.translate(-.002f, -.002f, .002f);

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
    mCurrentCamera->Cam.mProjectionMatrix.perspective(45.f, mAspectratio, 0.1f, 100.f);
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
void RenderWindow::checkForGLerrors()
{
    if(mOpenGLDebugLogger)
    {
        const QList<QOpenGLDebugMessage> messages = mOpenGLDebugLogger->loggedMessages();
        for (const QOpenGLDebugMessage &message : messages)
        {
            if (!(message.type() == message.OtherType)) // got rid of "object ... will use VIDEO memory as the source for buffer object operations"
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

        setCursor(Qt::BlankCursor);
        QPoint mousePos = mapFromGlobal(QCursor::pos());
        auto origo = QPoint(width()/2, height()/2);
        auto xOffset = mousePos.x()-origo.x();
        auto yOffset = mousePos.y()-origo.y();
        auto multiplier{0.1f};


        //mCamera.yaw(-1*xOffset*multiplier);
        //mCamera.pitch(-1*yOffset*multiplier);

        QCursor::setPos(mapToGlobal(origo));


        //Using mMouseXYlast as deltaXY so we don't need extra variables
        //mMouseXlast = event->pos().x() - mMouseXlast;
        //mMouseYlast = event->pos().y() - mMouseYlast;

        if (mMouseXlast != 0)
            mCurrentCamera->yaw(mCameraRotateSpeed * xOffset * multiplier/*mCameraRotateSpeed * mMouseXlast*/);
        if (mMouseYlast != 0)
            mCurrentCamera->pitch(mCameraRotateSpeed * yOffset * multiplier);
    }else{
        setCursor(Qt::ArrowCursor);
    }
    mMouseXlast = event->pos().x();
    mMouseYlast = event->pos().y();
}
