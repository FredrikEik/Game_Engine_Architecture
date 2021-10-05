#include "renderwindow.h"
#include <QTimer>
#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <QOpenGLDebugLogger>
#include <QKeyEvent>
#include <QStatusBar>
#include <QDebug>
#include <math.h>
#include <iostream>
#include <algorithm>

#include "shader.h"
#include "mainwindow.h"
#include "gameobject.h"
#include "xyz.h"
#include "cube.h"
#include "plane.h"
#include "triangle.h"
#include "mariocube.h"
#include "sphere.h"
#include "camera.h"
#include "constants.h"
#include "texture.h"
#include "components.h"
#include "factory.h"
#include "objreader.h"
#include "soundmanager.h"
#include "soundsource.h"
#include "vector3.h"
#include "quadtree.cpp"
#include "vector4d.h"
#include "matrix4x4.h"

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
    //mTextures
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

    //factory->createObject("Plane");
    //factory->createObject("Triangle");
    //factory->createObject("Cube");
    //factory->createObject("MarioCube");

    /*
    GameObject *marioCube = new MarioCube();
    marioCube->init();
    mGameObjects.push_back(marioCube);

    GameObject *plane = new Plane();
    plane->init();
    mGameObjects.push_back(plane);

    GameObject *cube = new Cube();
    cube->init();
    mGameObjects.push_back(cube);

    GameObject *triangle = new Triangle();
    triangle->init();
    mGameObjects.push_back(triangle);
    */



    //********************** Set up camera **********************
    glDisable(GL_CULL_FACE);
    mCurrentCamera = new Camera(45.0f, 4/3);
    mCurrentCamera->init();
    mCurrentCamera->setPosition(gsl::Vector3D(1.f, .5f, 4.f));
    mCurrentCamera->updateFrustumPos(45.0f, 4/3);

    mShaderPrograms[0] = new Shader((gsl::ShaderFilePath + "plainvertex.vert").c_str(),
                                    (gsl::ShaderFilePath + "plainfragment.frag").c_str());
                                     qDebug() << "Plain shader program id: " << mShaderPrograms[0]->getProgram();

    mShaderPrograms[1] = new Shader((gsl::ShaderFilePath + "textureshader.vert").c_str(),
                                    (gsl::ShaderFilePath + "textureshader.frag").c_str());
                                     qDebug() << "Texture shader program id: " << mShaderPrograms[1]->getProgram();

    setupPlainShader(0);
    setupTextureShader(1);

    //********************** Set up quadtree *******************
    gsml::Point2D nw{-10,-10}, ne{10,-10}, sw{-10, 10}, se{10, 10}; //specifies the quadtree area
    mQuadtree.init(nw, ne, sw, se);

    //********************** Sound set up **********************

    //SoundSource* mMario{};
    SoundSource* mVideoGameLand{};
    SoundSource* mVideoGameLand2{};

    SoundManager::getInstance()->init();
    mClick = SoundManager::getInstance()->createSource(
                "Click", Vector3(10.0f, 0.0f, 0.0f),
                "../GEA2021/Assets/Sounds/click.wav", false, 1.0f);

    /*mMario = SoundManager::getInstance()->createSource(
                "Mario", Vector3(10.0f, 0.0f, 0.0f),
                "../GEA2021/Assets/Sounds/mario.wav", false, 1.0f);*/
    mVideoGameLand = SoundManager::getInstance()->createSource(
                "VideoGameLand", Vector3(10.0f, 0.0f, 0.0f),
                "../GEA2021/Assets/Sounds/videogameland.wav", false, 0.2f);
    mVideoGameLand2 = SoundManager::getInstance()->createSource(
                "VideoGameLand2", Vector3(10.0f, 0.0f, 0.0f),
                "../GEA2021/Assets/Sounds/videogameland2.wav", false, 1.0f);

    mVideoGameLand->play();
    //mVideoGameLand2->play();





    //mMario->play(); //doesnt work
    //mExplosionSound->play();
    //mExplosionSound->setPosition(Vector3(200.0f, 30.0f, -1000.0f));







    //**********************************************************

}

// Called each frame - doing the rendering
void RenderWindow::render()
{
    //Keyboard / mouse input
    handleInput();

    mCurrentCamera->update();

    mTimeStart.restart(); //restart FPS clock
    mContext->makeCurrent(this); //must be called every frame (every time mContext->swapBuffers is called)

    initializeOpenGLFunctions();    //must call this every frame it seems...

    //to clear the screen for each redraw
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(0); //reset shader type before rendering

    //Draws the objects

    //mCurrentCamera->draw();

    //This should be in a loop! <- Ja vi må loope dette :/
    if(factory->mGameObjects.size() > 0)
    {
        for(auto it = factory->mGameObjects.begin(); it != factory->mGameObjects.end(); it++)
		{	
            unsigned int shaderProgramIndex = it->second->getMaterialComponent()->mShaderProgram;
			glUseProgram(mShaderPrograms[shaderProgramIndex]->getProgram()); // What shader program to use
			//send data to shader
            if(shaderProgramIndex == 1)
            {
                glUniform1i(mTextureUniform, it->second->getMaterialComponent()->mTextureUnit);
            }
			glUniformMatrix4fv( vMatrixUniform[shaderProgramIndex], 1, GL_TRUE, mCurrentCamera->mViewMatrix.constData());
			glUniformMatrix4fv( pMatrixUniform[shaderProgramIndex], 1, GL_TRUE, mCurrentCamera->mProjectionMatrix.constData());
            glUniformMatrix4fv( mMatrixUniform[shaderProgramIndex], 1, GL_TRUE, it->second->getTransformComponent()->mMatrix.constData());

            it->second->draw();
            it->second->move(0.001f, 0.001f, -0.001f);

            //MEGA TEMP COOM COLLISION DEBUG TEST THINGY SUPER DUPER BAD
            /*
            for (unsigned int y=0; y<factory->mGameObjects.size(); y++)
            {
               if(factory->mGameObjects[i] != factory->mGameObjects[y])
               {
                   //Check if both are cubes
                   if(dynamic_cast<Cube*>(factory->mGameObjects[i]) != nullptr && dynamic_cast<Cube*>(factory->mGameObjects[y]) != nullptr)
                   {
                       bool test = isColliding(*dynamic_cast<Cube*>(factory->mGameObjects[i])->getBoxCollisionComponent(),
                                               *dynamic_cast<Cube*>(factory->mGameObjects[y])->getBoxCollisionComponent());
                       qDebug() << "Box " << i << "colliding with box" << y << " = " << test;
                   }

                   //Check if one is sphere and one is cube
                   if(dynamic_cast<Cube*>(factory->mGameObjects[i]) != nullptr && dynamic_cast<Sphere*>(factory->mGameObjects[y]) != nullptr)
                   {
                       bool test = isColliding(*dynamic_cast<Cube*>(factory->mGameObjects[i])->getBoxCollisionComponent(),
                                               *dynamic_cast<Sphere*>(factory->mGameObjects[y])->getSphereCollisionComponent());
                       qDebug() << "Box " << i << "colliding with sphere" << y << " = " << test;
                   }
               }
            }
            */
         }
    }

        //debug mousePickingRay
        /*if (mDrawMousePickRay)
        {
            gsl::Matrix4x4 temp(true);
            glBindVertexArray( mDebugMousePickRay.mVAO[0] );
            glUniformMatrix4fv( mShaderPrograms[0]->pMatrixUniform, 1, GL_TRUE, mCurrentCamera->mProjectionMatrix.constData());
            glUniformMatrix4fv( mShaderPrograms[0]->vMatrixUniform, 1, GL_TRUE, mCurrentCamera->mViewMatrix.constData());
            glUniformMatrix4fv( mShaderPrograms[0]->mMatrixUniform, 1, GL_TRUE, temp.constData());
            glDrawArrays(mDebugMousePickRay.mDrawType, 0, mDebugMousePickRay.mVertexCount[0]);
        }*/

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
    mMatrixUniform[shaderIndex] = glGetUniformLocation( mShaderPrograms[shaderIndex]->getProgram(), "mMatrix" );
    vMatrixUniform[shaderIndex] = glGetUniformLocation( mShaderPrograms[shaderIndex]->getProgram(), "vMatrix" );
    pMatrixUniform[shaderIndex] = glGetUniformLocation( mShaderPrograms[shaderIndex]->getProgram(), "pMatrix" );
}

void RenderWindow::setupTextureShader(int shaderIndex)
{
    mMatrixUniform[shaderIndex] = glGetUniformLocation( mShaderPrograms[shaderIndex]->getProgram(), "mMatrix" );
    vMatrixUniform[shaderIndex] = glGetUniformLocation( mShaderPrograms[shaderIndex]->getProgram(), "vMatrix" );
    pMatrixUniform[shaderIndex] = glGetUniformLocation( mShaderPrograms[shaderIndex]->getProgram(), "pMatrix" );
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

void RenderWindow::createObjectbutton(std::string objectName)
{
    mClick->play();
    if(objectName == "MarioCube" || objectName == "Sphere")
    {
    factory->saveMesh("../GEA2021/Assets/Meshes/" + objectName + ".obj", objectName);   //   temporary fix since all objects are not .obj
    }
    GameObject* newObject = factory->createObject(objectName);

    gsl::Vector3D position = newObject->getTransformComponent()->mMatrix.getPosition();
    gsml::Point2D position2D = std::pair<double, double>(position.getX(), position.getY());
    uint32_t id = newObject->ID;

    mQuadtree.insert(position2D, id, newObject);
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

bool RenderWindow::isColliding(BoxCollisionComponent &Box1, BoxCollisionComponent &Box2)
{
    return ((Box1.max.getX() > Box2.min.getX()) &&
            (Box1.min.getX() < Box2.max.getX()) &&
            (Box1.max.getY() > Box2.min.getY()) &&
            (Box1.min.getY() < Box2.max.getY()) &&
            (Box1.max.getZ() > Box2.min.getZ()) &&
            (Box1.min.getZ() < Box2.max.getZ()));
}

bool RenderWindow::isColliding(BoxCollisionComponent &Box, SphereCollisionComponent &Sphere)
{
    gsl::Vector3D closestPointInBox = gsl::V3Dmin(gsl::V3Dmax(Sphere.center, Box.min),Box.max);
    gsl::Vector3D distanceVector = closestPointInBox - Sphere.center;

    double distanceSquared = sqrt(pow(distanceVector.getX(), 2) + pow(distanceVector.getY(), 2) + pow(distanceVector.getZ(), 2));

    return (distanceSquared < pow(Sphere.radius,2));
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

void RenderWindow::mousePicking(QMouseEvent *event)
{
        int mouseXPixel = event->pos().x();
        int mouseYPixel = event->pos().y(); //y is 0 at top of screen!

        //Since we are going to invert these, I make a copy
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
        projMatrix.inverse();
        gsl::Vector4D ray_eye = projMatrix * ray_clip;
        ray_eye = gsl::Vector4D(ray_eye.x, ray_eye.y, -1.0, 0.0);

        //step 4
        viewMatrix.inverse();
        gsl::Vector4D temp = viewMatrix * ray_eye; //temp save the result
        gsl::Vector3D ray_wor = {temp.x, temp.y, temp.z};
        // don't forget to normalise the vector at some point
        ray_wor.normalize();


        /************************************************************************/
        //Collision detection - in world space coordinates:
        //Writing here as a quick test - probably should be in a CollisionSystem class

        //This is ray vs bounding sphere collision

        for(int i{0}; i < factory->mGameObjects.size(); i++)
        {
            //making the vector from camera to object we test against
            gsl::Vector3D camToObject = factory->mGameObjects[i]->getTransformComponent()->mMatrix.getPosition() - mCurrentCamera->position();

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

            //if distance to ray < objects bounding sphere == we have a collision
            if(distance < 0.5f)
            {
    //            qDebug() << "Collision with object index" << i << distance << "meters away from ray";
                mIndexToPickedObject = i;
                mMainWindow->selectObjectByIndex(mIndexToPickedObject);
                factory->mGameObjects[i]->move(1000.f,0,0);
                break;  //breaking out of for loop - does not check if ray touch several objects


            }

        }

}
void RenderWindow::setPickedObject(int pickedID)
{
    mIndexToPickedObject = pickedID;
}

void RenderWindow::cancelPickedObject()
{
    mIndexToPickedObject = -1;
}

void RenderWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape) //Shuts down whole program
    {
        SoundManager::getInstance()->cleanUp();
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

    if(event->button() == Qt::LeftButton)
    {
        mousePicking(event);
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
