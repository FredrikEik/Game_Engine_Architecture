#include "renderwindow.h"

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

    initObject();

    //********************** Set up camera **********************

    //mEditorCamera.setPosition(gsl::Vector3D(1.f, .5f, 4.f));
    mPlayCamera.setPosition(gsl::Vector3D(1.f, 20.f, 9.f));
    mPlayCamera.pitch(70);


    mCurrentCamera = &mEditorCamera;


    //********************** create input **********************
    mInputComponent = new InputComponent();
    mInputSystem = new InputSystem();
    mCollisionSystem = new CollisionSystem();
}

void RenderWindow::initObject()
{
    //********************** Making the object to be drawn **********************

    mShapeFactory.makeVertices();

    VisualObject* temp = mShapeFactory.createShape("Circle");
    temp->init();
    temp->mMaterial->mShaderProgram = 0;    //plain shader
    temp->move(2.f, 1.f, .5f);
    mVisualObjects.push_back(temp);
    mTransComps.push_back(temp->mTransform);
    mNameComps.push_back(temp->mNameComp);

    temp = mShapeFactory.createShape("Square");
    temp->init();
    temp->mMaterial->mShaderProgram = 0;    //plain shader
    temp->move(-2.f, 0.f, .5f);
    mVisualObjects.push_back(temp);
    mTransComps.push_back(temp->mTransform);
    mNameComps.push_back(temp->mNameComp);

    temp = mShapeFactory.createShape("Triangle");
    temp->init();
    temp->mMaterial->mShaderProgram = 0;    //plain shader
    //    temp->mMaterial->mTextureUnit = 1;      //dog texture
    temp->move(3.f, 0.f, .5f);
    mVisualObjects.push_back(temp);
    mTransComps.push_back(temp->mTransform);
    mNameComps.push_back(temp->mNameComp);

    temp = mShapeFactory.createShape("Plain");
    temp->init();
    temp->mMaterial->mShaderProgram = 0;   //plain shader
    mVisualObjects.push_back(temp);
    mTransComps.push_back(temp->mTransform);
    mNameComps.push_back(temp->mNameComp);

    temp = mShapeFactory.createShape("Monkey.obj");
    temp->init();
    temp->mMaterial->mShaderProgram = 0;    //plain shader
    mVisualObjects.push_back(temp);
    mTransComps.push_back(temp->mTransform);
    mNameComps.push_back(temp->mNameComp);

    temp = mShapeFactory.createShape("Pacman.obj");
    temp->init();
    temp->mMaterial->mShaderProgram = 0;    //plain shader
    temp->move(4.f, 2.f, .5f);
    mVisualObjects.push_back(temp);
    mTransComps.push_back(temp->mTransform);
    mNameComps.push_back(temp->mNameComp);

    mPlayer = new Player();
    mPlayer->mMaterial->mShaderProgram = 0; //plain shader
    mPlayer->init();
    mPlayer->move(0,1,0);
    mVisualObjects.push_back(mPlayer);
    mTransComps.push_back(mPlayer->mTransform);
    mNameComps.push_back(mPlayer->mNameComp);

    temp = new XYZ();
    temp->mMaterial->mShaderProgram = 0; //plain shader
    temp->init();

    mFrustumSystem = new FrustumSystem();
    mFrustumSystem->mMaterial->mShaderProgram = 0;    //plain shader
    mFrustumSystem->init();
    //    mVisualObjects.push_back(mFrustumSystem);

    for(int i=0; i<10; i++)
    {
        for(int y=0; y<10; y++)
        {
            temp = mShapeFactory.createShape("Monkey.obj");
            temp->init();
            temp->move((i-y), 0.5, y-5);
            temp->mMaterial->mShaderProgram = 0;    //plain shader
            mTransComps.push_back(temp->mTransform);
            mNameComps.push_back(temp->mNameComp);
            mVisualObjects.push_back(temp);
        }
    }

    //makes the soundmanager
    //it is a Singleton!!!
    SoundManager::getInstance()->init();
    mLaserSound = SoundManager::getInstance()->createSource(
                "Laser", gsl::Vector3D(20.0f, 0.0f, 0.0f),
                "../GEA2021/Assets/laser.wav", true, 0.7f);
}

void RenderWindow::drawObject()
{

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
        glUniformMatrix4fv( viewMatrix, 1, GL_TRUE, mCurrentCamera->mViewMatrix.constData());
        glUniformMatrix4fv( projectionMatrix, 1, GL_TRUE, mCurrentCamera->mProjectionMatrix.constData());
        glUniformMatrix4fv( modelMatrix, 1, GL_TRUE, mVisualObjects[i]->mTransform->mMatrix.constData());

        glBindVertexArray( mVisualObjects[i]->mMesh->mVAO );
        glDrawArrays(mVisualObjects[i]->mMesh->mDrawType, 0, levelOfDetail(i));
        glBindVertexArray(0);
    }
    if(playM==false){
        glBindVertexArray( mFrustumSystem->mMesh->mVAO );
        glDrawArrays(mFrustumSystem->mMesh->mDrawType, 0, mFrustumSystem->mMesh->mVertices.size());
        glBindVertexArray(0);}
}

// Called each frame - doing the rendering
void RenderWindow::render()
{
    // HandleInput();
    mInputSystem->update(mCurrentCamera, mPlayer, mInput);
    mCurrentCamera->update();
    mFrustumSystem->updateFrustumPos(mEditorCamera.position());
    if(mCollisionSystem->CheckSphOnBoxCol(mPlayer->mCollision, mVisualObjects[1]->mCollision))
        qDebug() <<"Collision detected"; //testing collision

    mTimeStart.restart(); //restart FPS clock
    mContext->makeCurrent(this); //must be called every frame (every time mContext->swapBuffers is called)

    initializeOpenGLFunctions();    //must call this every frame it seems...

    //to clear the screen for each redraw
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    drawObject();

    //Moves the obj - should be mada another way!!!!
    //mVisualObjects[1]->mTransform->mMatrix.translate(.001f, .001f, -.001f);     //just to move the triangle each frame

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

    //mEditorCamera.calculateFrustumVectors();
    //calculate aspect ration and set projection matrix
    mCurrentCamera->mFrustumComp.mAspectRatio = static_cast<float>(width()) / height();
    //    qDebug() << mAspectratio;
    mPlayCamera.calculateProjectionMatrix();
    mEditorCamera.calculateProjectionMatrix();
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
        //glDisable(GL_CULL_FACE);
    }
    else
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);    //turn off wireframe mode
        //glEnable(GL_CULL_FACE);
    }

}

void RenderWindow::playMode(bool p)
{
    if(p)
    {
        mCurrentCamera = &mPlayCamera;
        mLaserSound->play();
        playM = true;
    }
    else
    {
        mCurrentCamera = &mEditorCamera;
        mLaserSound->stop();
        playM = false;
    }
}

void RenderWindow::createShapes(string shapeID)
{
    VisualObject* temp = mShapeFactory.createShape(shapeID);
    temp->init();
    temp->move(1,1,0.5);
    temp->mMaterial->mShaderProgram = 0;    //plain shader
    mTransComps.push_back(temp->mTransform);
    mNameComps.push_back(temp->mNameComp);
    mVisualObjects.push_back(temp);
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

int RenderWindow::levelOfDetail(int i)
{

    //making the vector from camera to object we test against
    gsl::Vector3D camToObject = mVisualObjects[i]->mTransform->mMatrix.getPosition() - mCurrentCamera->position();

    if(camToObject.length()<5)
    {
        return mVisualObjects[i]->mMesh->mVertices.size();
    }
    else if(camToObject.length()<20)
    {
        if(mVisualObjects[i]->mMesh->mVertices.size()>30)
            return mVisualObjects[i]->mMesh->mVertices.size()/2;
        else
            return mVisualObjects[i]->mMesh->mVertices.size();
    }
    else
        if(mVisualObjects[i]->mMesh->mVertices.size()>30)
            return mVisualObjects[i]->mMesh->mVertices.size()/4;
        else
            return mVisualObjects[i]->mMesh->mVertices.size();

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
            mMainWindow->selectWithMousePick(i);

            MousePickindex = i;
            qDebug() <<"Mouse Collision detected";
        }

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
            mCurrentCamera->yaw(mInputComponent->mCameraRotateSpeed * mInputComponent->mMouseXlast);
        if (mInputComponent->mMouseYlast != 0)
            mCurrentCamera->pitch(mInputComponent->mCameraRotateSpeed * mInputComponent->mMouseYlast);
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
