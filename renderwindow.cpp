#include "renderwindow.h"
#include <QTimer>
#include <QMatrix4x4>
#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <QOpenGLDebugLogger>
#include <QKeyEvent>
#include <QStatusBar>
#include <QDebug>

#include <iostream> //we use std::cout for GPU readout

#include "shader.h"
#include "plainshader.h"
#include "textureshader.h"
#include "phongshader.h"
#include "cubemapshader.h"

#include "vertex.h"
#include "visualobject.h"
#include "texture.h"
#include "input.h"
#include "camera.h"
#include "mainwindow.h"
#include "xyz.h"
#include "lightsource.h"
#include "plane.h"
#include "ECScoordinator.h"
#include "constants.h"
#include "skybox.h"


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
    //cleans up the GPU memory
    glDeleteVertexArrays( 1, &mVAO );
    glDeleteBuffers( 1, &mVBO );
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

    //Start the Qt OpenGL debugger
    //Really helpfull when doing OpenGL
    //Supported on most Windows machines - at least with NVidia GPUs
    //reverts to plain glGetError() on Mac and other unsupported PCs
    // - can be deleted
    startOpenGLDebugger();

    //general OpenGL stuff:
    glEnable(GL_DEPTH_TEST);            //enables depth sorting - must then use GL_DEPTH_BUFFER_BIT in glClear
    //glEnable(GL_CULL_FACE);          //draws only front side of models - usually what you want - test it out!
    glClearColor(0.4f, 0.4f, 0.4f, 1.0f);    //gray color used in glClear GL_COLOR_BUFFER_BIT

    //ECS Coordinator:
    //Make sure only one instance is created!
    ECScord = std::make_unique<ECScoordinator>();

    //Compile shaders:
    //NB: hardcoded path to files! You have to change this if you change directories for the project.
    //Qt makes a build-folder besides the project folder. That is why we go down one directory
    // (out of the build-folder) and then up into the project folder.
    m_shaderProgramMap.insert(std::pair<std::string, Shader*>{"plain", new PlainShader((gsl::ShaderFilePath + "plainshader.vert").c_str(), (gsl::ShaderFilePath + "plainshader.frag").c_str())});
    m_shaderProgramMap.insert(std::pair<std::string, Shader*>{"texture", new TextureShader((gsl::ShaderFilePath + "texture.vert").c_str(), (gsl::ShaderFilePath + "texture.frag").c_str())});
    m_shaderProgramMap.insert(std::pair<std::string, Shader*>{"phong", new PhongShader((gsl::ShaderFilePath + "phong.vert").c_str(), (gsl::ShaderFilePath + "phong.frag").c_str())});
    m_shaderProgramMap.insert(std::pair<std::string, Shader*>{"cubemap", new CubemapShader((gsl::ShaderFilePath + "cubemap.vert").c_str(), (gsl::ShaderFilePath + "cubemap.frag").c_str())});

    for (auto it = m_shaderProgramMap.begin(); it != m_shaderProgramMap.end(); it++)
        (*it).second->setup();

    //textures
    m_textureMap.insert(std::pair<std::string, Texture*>{"dog", new Texture(gsl::TextureFilePath + "hund.bmp")});

    //Set textures to a texture unit
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, m_textureMap["dog"]->id());

    //Light
    m_light = new LightSource(m_shaderProgramMap["phong"]);
    m_light->setPosition(QVector3D(0.f, 150.f, 150.f));

    //Map
    m_world = new Plane(gsl::TextureFilePath + "hm2.bmp");
    m_world->setShader(m_shaderProgramMap["phong"]);
    m_world->setObjectName("World");

    //Skybox
    m_skybox = new SkyBox();
    m_skybox->setShader(m_shaderProgramMap["cubemap"]);
    m_skybox->setObjectName("Skybox");

    //Camera
    m_camera = new Camera();
    m_camera->pitch(10.f);
    m_camera->perspective(60.f, 4.f/3.f, 0.1f, 1000.f);

    //Objects
    for (auto it = m_objectMap.begin(); it != m_objectMap.end(); it++)
        (*it).second->init(m_shaderProgramMap[(*it).second->getShaderName()]->getModelMatrixUniform());

    m_world->init(m_shaderProgramMap["phong"]->getModelMatrixUniform());
    m_light->init(m_shaderProgramMap["phong"]->getModelMatrixUniform());
    m_skybox->init(m_skybox->getShader()->getModelMatrixUniform());


    glBindVertexArray(0);       //unbinds any VertexArray - good practice
}

// Called each frame - doing the rendering!
void RenderWindow::render()
{
    //Keyboard & mouse input
    handleInput();

    mTimeStart.restart(); //restart FPS clock
    mContext->makeCurrent(this); //must be called every frame (every time mContext->swapBuffers is called)

    initializeOpenGLFunctions();    //must call this every frame it seems...

    //clear the screen for each redraw
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_camera->update(m_skybox->getShader()->getProjectionMatrixUniform(), m_skybox->getShader()->getViewMatrixUniform());

    for(auto it : m_shaderProgramMap)
    {
        it.second->use();
        m_camera->update(it.second->getProjectionMatrixUniform(), it.second->getViewMatrixUniform());
    }

    glUseProgram(m_skybox->getShader()->getProgram());
    m_skybox->draw();

    for (auto it = m_objectMap.begin(); it != m_objectMap.end(); it++)
    {
        (*it).second->getShader()->use();
        m_camera->update((*it).second->getShader()->getProjectionMatrixUniform(), (*it).second->getShader()->getViewMatrixUniform());
        (*it).second->draw();
    }

    //Draw special objects
    glUseProgram(m_shaderProgramMap["phong"]->getProgram());
    m_camera->update(m_shaderProgramMap["phong"]->getProjectionMatrixUniform(), m_shaderProgramMap["phong"]->getViewMatrixUniform());

    //Put these in a container?
    m_world->draw();
    m_light->draw();


    //Calculate framerate before
    // checkForGLerrors() because that call takes a long time
    // and before swapBuffers(), else it will show the vsync time
    calculateFramerate();

    //using our expanded OpenGL debugger to check if everything is OK.
    checkForGLerrors();

    //Qt require us to call this swapBuffers() -function.
    // swapInterval is 1 by default which means that swapBuffers() will (hopefully) block
    // and wait for vsync.
    mContext->swapBuffers(this);

}

//This function is called from Qt when window is exposed (shown)
// and when it is resized
//exposeEvent is a overridden function from QWindow that we inherit from
void RenderWindow::exposeEvent(QExposeEvent *)
{
    //if not already initialized - run init() function - happens on program start up
    if (!mInitialized)
        init();

    //This is just to support modern screens with "double" pixels (Macs and some 4k Windows laptops)
    const qreal retinaScale = devicePixelRatio();

    //Set viewport width and height to the size of the QWindow we have set up for OpenGL
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
}

//The way this function is set up is that we start the clock before doing the draw call,
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
        if (frameCount > 30)    //once pr 30 frames = update the message == twice pr second (on a 60Hz monitor)
        {
            //showing some statistics in status bar
            mMainWindow->statusBar()->showMessage(" Time pr FrameDraw: " +
                                                  QString::number(nsecElapsed/1000000.f, 'g', 4) + " ms  |  " +
                                                  "FPS (approximated): " + QString::number(1E9 / nsecElapsed, 'g', 7));
            frameCount = 0;     //reset to show a new message in 30 frames
        }
    }
}

//Uses QOpenGLDebugLogger if this is present
//Reverts to glGetError() if not
void RenderWindow::checkForGLerrors()
{
    if(mOpenGLDebugLogger)  //if our machine got this class to work
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
            switch (err) {
            case 1280:
                qDebug() << "GL_INVALID_ENUM - Given when an enumeration parameter is not a "
                                "legal enumeration for that function.";
                break;
            case 1281:
                qDebug() << "GL_INVALID_VALUE - Given when a value parameter is not a legal "
                                "value for that function.";
                break;
            case 1282:
                qDebug() << "GL_INVALID_OPERATION - Given when the set of state for a command "
                                "is not legal for the parameters given to that command. "
                                "It is also given for commands where combinations of parameters "
                                "define what the legal parameters are.";
                break;
            }
        }
    }
}

//Tries to start the extended OpenGL debugger that comes with Qt
//Usually works on Windows machines, but not on Mac...
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

//Use new object when called to create the object at the same time.
VisualObject* RenderWindow::createObject(VisualObject* object, std::string objectName, std::string shaderToUse, QVector3D position)
{
    m_objectMap.insert(std::pair<std::string, VisualObject*>{objectName, object});

    object->setObjectName(objectName);
    object->setShaderName(shaderToUse);
    object->setShader(m_shaderProgramMap[shaderToUse]);
    object->setPosition(position);

    return object;
}

void RenderWindow::createAllObjects(/*bool bReset*/)
{
//    if(bReset)
//    {
//        delete m_player;
//        //delete all objects related to gameplay

//    }

    m_world = createObject(new Plane("../3Dprog21/Assets/hm2.bmp"), "World", "phong", QVector3D(0.f, 0.f, 0.f));
    m_light = createObject(new LightSource(m_shaderProgramMap["phong"]), "Lightsource", "phong", QVector3D(150.f, 150.f, 150.f));
    //m_skybox = createObject(new SkyBox(), "Skybox", "cubemap", QVector3D(0.f, 0.f, 0.f));
    //m_player = dynamic_cast<Player*>(createObject(new Player(), "Player", "phong", QVector3D(50.f, 0.f, 50.f)));
    //m_player->setPlane(dynamic_cast<Plane*>(m_world));
    if(!m_player)
        qDebug() << "Error: Dynamic cast failed for m_player";
}

void RenderWindow::setCameraSpeed(float value)
{
    m_cameraSpeed += value;

    //Make sure its not to high/low
    if (m_cameraSpeed > 1.f)
        m_cameraSpeed = 1.f;
    if (m_cameraSpeed < 0.01f)
        m_cameraSpeed = 0.01f;
}

void RenderWindow::setMouseSensitivity(float value)
{
    m_mouseSensitiviy += value;

    //clamp
    if(m_mouseSensitiviy > 3.f)
        m_mouseSensitiviy = 3.f;
    if(m_mouseSensitiviy < 0.1f)
        m_mouseSensitiviy = 0.1f;
}

void RenderWindow::handleInput()
{
    //Camera Movement
    if(m_input.RMB)
    {
        if(m_input.W)
            m_camera->moveForward(m_cameraSpeed);
        if(m_input.S)
            m_camera->moveForward(-m_cameraSpeed);
        if(m_input.A)
            m_camera->moveRight(-m_cameraSpeed);
        if(m_input.D)
            m_camera->moveRight(m_cameraSpeed);
        if(m_input.E)
            m_camera->moveUp(m_cameraSpeed);
        if(m_input.Q)
            m_camera->moveUp(-m_cameraSpeed);
    }

    if(m_camera->editorCamera())
    {
        if(m_input.C && bKeyReleased)
        {
            if(!bWiremode)
            {
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                bWiremode = true;
                bKeyReleased = false;
            }
            else
            {
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                bWiremode = false;
                bKeyReleased = false;
            }
        }
    }

    //Object Movement
//    if(m_player)
//    {
//        //Replace with const values later??
//        if(m_input.LEFT)
//            m_player->move(QVector3D{0.1f, 0.f, 0.f});
//        if(m_input.RIGHT)
//            m_player->move(QVector3D{-0.1f, 0.f, 0.f});
//        if(m_input.UP)
//            m_player->move(QVector3D{0.f, 0.f, 0.1f});
//        if(m_input.DOWN)
//            m_player->move(QVector3D{0.f, 0.f, -0.1f});
//    }
}


//Event sent from Qt when program receives a keyPress
void RenderWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape)
        mMainWindow->close();       //Shuts down the whole program

    //Moving object
    if(event->key() == Qt::Key_Left)
        m_input.LEFT = true;
    if(event->key() == Qt::Key_Right)
        m_input.RIGHT = true;
    if(event->key() == Qt::Key_Up)
        m_input.UP = true;
    if(event->key() == Qt::Key_Down)
        m_input.DOWN = true;

    //Camera movement
    if(event->key() == Qt::Key_W)
        m_input.W = true;
    if(event->key() == Qt::Key_S)
        m_input.S = true;
    if(event->key() == Qt::Key_A)
        m_input.A = true;
    if(event->key() == Qt::Key_D)
        m_input.D = true;
    if(event->key() == Qt::Key_Q)
        m_input.Q = true;
    if(event->key() == Qt::Key_E)
        m_input.E = true;

    //Misc keys
    if(event->key() == Qt::Key_Control)
        m_input.CTRL = true;
    if(event->key() == Qt::Key_Shift)
        m_input.SHIFT = true;
    if(event->key() == Qt::Key_Z)
        m_input.Z = true;
    if(event->key() == Qt::Key_X)
        m_input.X = true;
    if(event->key() == Qt::Key_C)
        m_input.C = true;
}

void RenderWindow::keyReleaseEvent(QKeyEvent *event)
{
    //Moving object
    if(event->key() == Qt::Key_Left)
        m_input.LEFT = false;
    if(event->key() == Qt::Key_Right)
        m_input.RIGHT = false;
    if(event->key() == Qt::Key_Up)
        m_input.UP = false;
    if(event->key() == Qt::Key_Down)
        m_input.DOWN = false;

    //Camera movement
    if(event->key() == Qt::Key_W)
        m_input.W = false;
    if(event->key() == Qt::Key_S)
        m_input.S = false;
    if(event->key() == Qt::Key_A)
        m_input.A = false;
    if(event->key() == Qt::Key_D)
        m_input.D = false;
    if(event->key() == Qt::Key_Q)
        m_input.Q = false;
    if(event->key() == Qt::Key_E)
        m_input.E = false;

    //Misc keys
    if(event->key() == Qt::Key_Control)
        m_input.CTRL = false;
    if(event->key() == Qt::Key_Shift)
        m_input.SHIFT = false;
    if(event->key() == Qt::Key_Z)
        m_input.Z = false;
    if(event->key() == Qt::Key_X)
        m_input.X = false;
    if(event->key() == Qt::Key_C)
    {
        m_input.C = false;
        bKeyReleased = true;
    }
}

void RenderWindow::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::RightButton)
        m_input.RMB = true;
    if(event->button() == Qt::LeftButton)
        m_input.LMB = true;
    if(event->button() == Qt::MiddleButton)
        m_input.MMB = true;
}

void RenderWindow::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::RightButton)
        m_input.RMB = false;
    if(event->button() == Qt::LeftButton)
        m_input.LMB = false;
    if(event->button() == Qt::MiddleButton)
        m_input.MMB = false;
}

void RenderWindow::wheelEvent(QWheelEvent *event)
{
    QPoint numDegrees = event->angleDelta() / 8;

    //If CTRL is held, change the speed of the camera
    if(m_input.CTRL)
    {
        if (numDegrees.y() > 1)
            setCameraSpeed(0.01f);
        if (numDegrees.y() < 1)
            setCameraSpeed(-0.01f);
    }
    //Change mouse sensitivity
    if(m_input.SHIFT)
    {
        if (numDegrees.y() > 1)
            setMouseSensitivity(0.1f);
        if (numDegrees.y() < 1)
            setMouseSensitivity(-0.1f);
    }

    event->accept();
}

void RenderWindow::mouseMoveEvent(QMouseEvent *event)
{
    if(m_input.RMB)
    {
        m_mouseXlast = event->pos().x() - m_mouseXlast;
        m_mouseYlast = event->pos().y() - m_mouseYlast;

        if (m_mouseXlast != 0)
            m_camera->yaw(m_cameraRotateSpeed * m_mouseXlast * m_mouseSensitiviy);
        if (m_mouseYlast != 0)
            m_camera->pitch(m_cameraRotateSpeed * m_mouseYlast * m_mouseSensitiviy);
    }
    m_mouseXlast = event->pos().x();
    m_mouseYlast = event->pos().y();
}
