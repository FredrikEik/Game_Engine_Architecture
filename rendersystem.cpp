#include "rendersystem.h"
#include <QTimer>
#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <QOpenGLDebugLogger>
#include <QKeyEvent>
#include <QStatusBar>
#include <QDebug>
#include <QFile>
#include <QJsonObject>
#include <QJsonDocument>
#include <math.h>
#include <iostream>
#include <algorithm>

#include "system.h"
#include "shader.h"
#include "mainwindow.h"
#include "gameobject.h"
#include "xyz.h"
#include "cube.h"
#include "plane.h"
#include "skybox.h"
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
#include "particle.h"
#include "vector4d.h"
#include "level.h"
#include "matrix4x4.h"

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
    else{
        qDebug() << "Context was made";
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
    glActiveTexture(GL_TEXTURE0);
    mTextures[0] = new Texture();
    glActiveTexture(GL_TEXTURE1);
    mTextures[1] = new Texture("hund.bmp");
    glActiveTexture(GL_TEXTURE2);
    mTextures[2] = new Texture("wood.bmp");
    glActiveTexture(GL_TEXTURE3);
    mTextures[3] = new Texture("groundgrass.bmp");
    glActiveTexture(GL_TEXTURE4);
    mTextures[4] = new Texture("stone.bmp");
    glActiveTexture(GL_TEXTURE5);
    mTextures[5] = new Texture("0hp.bmp");
    glActiveTexture(GL_TEXTURE6);
    mTextures[6] = new Texture("1hp.bmp");
    glActiveTexture(GL_TEXTURE7);
    mTextures[7] = new Texture("2hp.bmp");
    glActiveTexture(GL_TEXTURE8);
    mTextures[8] = new Texture("fullhp.bmp");
    glActiveTexture(GL_TEXTURE9);
    mTextures[9] = new Texture("right.bmp",
                                "left.bmp",
                                "top.bmp",
                                "bottom.bmp",
                                "front.bmp",
                                "back.bmp");

    //Set the textures loaded to a texture unit
    glBindTexture(GL_TEXTURE_2D,       mTextures[0]->mGLTextureID);
    glBindTexture(GL_TEXTURE_2D,       mTextures[1]->mGLTextureID);
    glBindTexture(GL_TEXTURE_2D,       mTextures[2]->mGLTextureID);
    glBindTexture(GL_TEXTURE_2D,       mTextures[3]->mGLTextureID);
    glBindTexture(GL_TEXTURE_2D,       mTextures[4]->mGLTextureID);
    glBindTexture(GL_TEXTURE_2D,       mTextures[5]->mGLTextureID);
    glBindTexture(GL_TEXTURE_2D,       mTextures[6]->mGLTextureID);
    glBindTexture(GL_TEXTURE_2D,       mTextures[7]->mGLTextureID);
    glBindTexture(GL_TEXTURE_2D,       mTextures[8]->mGLTextureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, mTextures[9]->mGLTextureID);


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


    //playCamera->setPosition(systemRef->getPlayer()->cameraTarget);

    //********************** Set up ShaderHandler *******************
    mShaderHandler = new ShaderHandler();
    mShaderHandler->init();

    //********************** Set up System *******************
    factory = new Factory();
    systemRef = new System(mMainWindow, this);

    //Editor camera
    editorCamera = new Camera(90, 4/3);
    editorCamera->init();
    editorCamera->setPosition(gsl::Vector3D(0.f, 0.f, 0.f));

    //Play camera
    playCamera = new Camera(90, 4/3);
    playCamera->init();

    //postFBO = new PostProcessing(mShaderPrograms[5]);

    initObjects();
    //********************** Set up quadtree *******************
    //gsml::Point2D nw{-10,-10}, ne{10,-10}, sw{-10, 10}, se{10, 10}; //specifies the quadtree area
    //mQuadtree.init(nw, ne, sw, se);

    //********************** Start the game loop *******************
    //systemRef->startGameLoop();

}

void RenderSystem::initObjects()
{

    //Skybox
    skybox = factory->createObject(gsl::SKYBOX);
    skybox->getTransformComponent()->mMatrix.setRotation(-180, 0, 0);
    skybox->getTransformComponent()->mMatrix.setScale(50,50,50);
    gameObjects.push_back(skybox);

    //Surface
    triangleSurface = factory->createObject(gsl::TRIANGLESURFACE);
    gameObjects.push_back(triangleSurface);

    //Player
    player = factory->createObject(gsl::PLAYER);
    player->getTransformComponent()->mMatrix.setScale(0.1f,0.1f,0.1f);
    player->getTransformComponent()->mMatrix.setPosition(0.f,0.6f,0.f);
    dynamic_cast<Player*>(player)->setSurfaceToWalkOn(triangleSurface);
    dynamic_cast<Player*>(player)->setPlayerCamera(playCamera);
    gameObjects.push_back(player);

    //Lights - Only one lightsource work for now :(
    GameObject* light1 = factory->createObject(gsl::LIGHT);
    GameObject* light2 = factory->createObject(gsl::LIGHT);
    gameObjects.push_back(light1);
    gameObjects.push_back(light2);
    mShaderHandler->lightRefs.push_back(dynamic_cast<Light*>(light1));
    mShaderHandler->lightRefs.push_back(dynamic_cast<Light*>(light2));

    //Helper Object
    helperObject = factory->createObject(gsl::CUBE);
    helperObjectMesh = new MeshComponent;
    gameObjects.push_back(helperObject);

    mParticles = new particle();
    mParticles->init();

    HUD = factory->createObject(gsl::BILLBOARD);
    gameObjects.push_back(HUD);

    mMainWindow->updateOutliner(gameObjects);
}

void RenderSystem::initTextures()
{



}


// Called each frame - doing the rendering
void RenderSystem::render()
{
    editorCamera->update(editorCamera->FOV, editorCamera->aRatio);
    playCamera->update(playCamera->FOV, playCamera->aRatio);

    mContext->makeCurrent(this); //must be called every frame (every time mContext->swapBuffers is called)

    initializeOpenGLFunctions();    //must call this every frame it seems...

    //to clear the screen for each redraw
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //glEnable(GL_CULL_FACE);
    glUseProgram(0); //reset shader type before rendering

    if(postFBO)
       postFBO->bindFramebuffer();

    //Draws the objects
    if(gameObjects.size() > 0)
    {
        for(int i{0}; i < gameObjects.size(); i++)
        {
            GameObject* gameObject = gameObjects[i];
            Camera* currentCamera;
            if(systemRef->getEditorMode())
            {
                currentCamera = editorCamera;
                mShaderHandler->cameraRef = editorCamera;
            }
            else
            {
                currentCamera = playCamera;
                mShaderHandler->cameraRef = playCamera;
            }

            mShaderHandler->sendDataToShader(gameObject);
            if(toggleFrustumCulling
                    && gameObject->mObjectType != gsl::SKYBOX
                    && gameObject->mObjectType != gsl::TRIANGLESURFACE
                    && gameObject->mObjectType != gsl::PLAYER)
            {
                if(systemRef->isCollidingWithFrustum(*gameObject->getSphereCollisionComponent()))
                {
                    // qDebug() << "Object inside frustum";
                    gameObjects[i]->checkLodDistance((gameObject->getTransformComponent()->mMatrix.getPosition() -
                                                                 currentCamera->getFrustumComponent()->mMatrix.getPosition()),
                                                                currentCamera->getFrustumComponent()->farPlaneLength/2);
                    gameObjects[i]->draw();
                    objectsDrawn++;
                }
            }
            else
            {
                gameObjects[i]->checkLodDistance((gameObject->getTransformComponent()->mMatrix.getPosition() -
                                                             currentCamera->getFrustumComponent()->mMatrix.getPosition()),
                                                            currentCamera->getFrustumComponent()->farPlaneLength/2);
                gameObjects[i]->draw();
            }

            glUseProgram(mShaderHandler->mShaderPrograms[gsl::Shaders::PARTICLESHADER]->getProgram());
            glUniformMatrix4fv( glGetUniformLocation(mShaderHandler->mShaderPrograms[gsl::Shaders::PARTICLESHADER]->getProgram(), "vMatrix"), 1, GL_TRUE, currentCamera->mViewMatrix.constData());
            glUniformMatrix4fv( glGetUniformLocation(mShaderHandler->mShaderPrograms[gsl::Shaders::PARTICLESHADER]->getProgram(), "pMatrix"), 1, GL_TRUE, currentCamera->mProjectionMatrix.constData());
            mParticles->update(0.16, currentCamera);

            if (i==mIndexToPickedObject)
            {
                helperObjectMesh = gameObject->getMeshComponent();
                helperObjectMesh->mDrawType = GL_LINE_STRIP;
                helperObject->setMeshComponent(helperObjectMesh);

                if (helperObject != gameObject)
                {
                    gsl::Vector3D tempPosition;
                    gsl::Vector3D tempScale;
                    tempPosition = gameObject->getTransformComponent()->mMatrix.getPosition();
                    helperObject->getTransformComponent()->mMatrix.setPosition(tempPosition.x, tempPosition.y, tempPosition.z);
                    tempScale = gameObject->getTransformComponent()->mMatrix.getScale();
                    helperObject->getTransformComponent()->mMatrix.setScale(tempScale.x*1.2f, tempScale.y*1.2f, tempScale.z*1.2f);
                }
                else if (helperObject == gameObject)
                {
                    mIndexToPickedObject = NULL;
                }
                gameObject->setMeshComponent(helperObjectMesh);
            }
            if(systemRef->getEditorMode())
            {
                playCamera->draw();
            }

        }
    }

    if(postFBO)
            postFBO->unbindCurrentFramebuffer();

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


    //qDebug() << "Rendered objects: ";
    //qDebug() << objectsDrawn;
    objectsDrawn = 0;

    if(playerHP == 0)
    {
        systemRef->resetLevel();
    }
}

//This function is called from Qt when window is exposed (shown)
// and when it is resized
//exposeEvent is a overridden function from QWindow that we inherit from
void RenderSystem::exposeEvent(QExposeEvent *)
{
    //if not already initialized - run init() function
    if (!mInitialized)
    {
        init();
    }
    else{
        qDebug() << "RenderSystem already initialized.";
    }


    //This is just to support modern screens with "double" pixels (Macs and some 4k Windows laptops)
    const qreal retinaScale = devicePixelRatio();

    //Set viewport width and height
    glViewport(0, 0, static_cast<GLint>(width() * retinaScale), static_cast<GLint>(height() * retinaScale));

    //If the window actually is exposed to the screen we start the main loop
    //isExposed() is a function in QWindow
    if (isExposed())
    {

    }

    //calculate aspect ration and set projection matrix
    mAspectratio = static_cast<float>(width()) / height();
    editorCamera->mProjectionMatrix.perspective(45.f, mAspectratio, 0.1f, 100.f);
    playCamera->mProjectionMatrix.perspective(45.f, mAspectratio, 0.1f, 100.f);
    //    qDebug() << mAspectratio;
    //    qDebug() << mCamera.mProjectionMatrix;
}

//The way this is set up is that we start the clock before doing the draw call,
// and check the time right after it is finished (done in the render function)
//This will approximate what framerate we COULD have.
//The actual frame rate on your monitor is limited by the vsync and is probably 60Hz
void RenderSystem::calculateFramerate()
{
    long nsecElapsed = systemRef->timeStart.nsecsElapsed();
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
void RenderSystem::toggleWireframe(bool buttonState)
{
    if(systemRef != nullptr)
    {
        systemRef->clickSound->play();
    }
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

void RenderSystem::createObjectbutton(int objectType)
{
    if(systemRef != nullptr)
    {
        systemRef->clickSound->play();
        gameObjects.push_back(factory->createObject(gsl::ObjectType(objectType)));
        mMainWindow->updateOutliner(gameObjects);
    }
}
void RenderSystem::playPausebutton(const QSurfaceFormat &format)
{
    systemRef->toggleEditorMode();
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


void RenderSystem::mousePicking(QMouseEvent *event)

{
    if(systemRef != nullptr)
    {
        int mouseXPixel = event->pos().x();
        int mouseYPixel = event->pos().y(); //y is 0 at top of screen!

        //Since we are going to invert these, I make a copy
        gsl::Matrix4x4 projMatrix = editorCamera->mProjectionMatrix;
        gsl::Matrix4x4 viewMatrix = editorCamera->mViewMatrix;

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

        for(int i{0}; i <gameObjects.size(); i++)
        {
            //making the vector from camera to object we test against
            gsl::Vector3D camToObject = gameObjects[i]->getTransformComponent()->mMatrix.getPosition() - editorCamera->position();

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
             if(distance < 0.5f && systemRef->getEditorMode() && gameObjects[i]->mObjectName != "Skybox")
            {
                mIndexToPickedObject = i;
                mMainWindow->selectObjectByIndex(mIndexToPickedObject);
                break;  //breaking out of for loop - does not check if ray touch several objects
            }
        }
    }
}
void RenderSystem::setPickedObject(int pickedID)
{
    mIndexToPickedObject = pickedID;
}

void RenderSystem::cancelPickedObject()
{
    mIndexToPickedObject = -1;
}

void RenderSystem::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape) //Shuts down whole program
    {
        SoundManager::getInstance()->cleanUp();
        mMainWindow->close();
    }

    if(systemRef != nullptr)
    {
        //send input to system
        if(event->key() == Qt::Key_W)
        {
            systemRef->getInput()->W = true;
        }
        if(event->key() == Qt::Key_S)
        {
            systemRef->getInput()->S = true;
        }
        if(event->key() == Qt::Key_D)
        {
           systemRef->getInput()->D = true;
        }
        if(event->key() == Qt::Key_A)
        {
            systemRef->getInput()->A = true;
        }
        if(event->key() == Qt::Key_Q)
        {
            systemRef->getInput()->Q = true;
        }
        if(event->key() == Qt::Key_E)
        {
            systemRef->getInput()->E = true;
        }
        if(event->key() == Qt::Key_Z)
        {
        }
        if(event->key() == Qt::Key_X)
        {
            if(!systemRef->getEditorMode())
            {
                systemRef->getInput()->X = true;
                static_cast<Player*>(player)->takeDamage(playerHP, systemRef->getInput());
            }
        }
        if(event->key() == Qt::Key_Up)
        {
            systemRef->getInput()->UP = true;
        }
        if(event->key() == Qt::Key_Down)
        {
            systemRef->getInput()->DOWN = true;
        }
        if(event->key() == Qt::Key_Left)
        {
            systemRef->getInput()->LEFT = true;
        }
        if(event->key() == Qt::Key_Right)
        {
            systemRef->getInput()->RIGHT = true;
        }
        if(event->key() == Qt::Key_U)
        {
        }
        if(event->key() == Qt::Key_O)
        {
        }
        if(event->key() == Qt::Key_R)
        {
             systemRef->resetLevel();
        }
    }

}

void RenderSystem::keyReleaseEvent(QKeyEvent *event)
{
    if(systemRef != nullptr)
    {
        if(event->key() == Qt::Key_W)
        {
            systemRef->getInput()->W = false;
        }
        if(event->key() == Qt::Key_S)
        {
            systemRef->getInput()->S = false;
        }
        if(event->key() == Qt::Key_D)
        {
            systemRef->getInput()->D = false;
        }
        if(event->key() == Qt::Key_A)
        {
            systemRef->getInput()->A = false;
        }
        if(event->key() == Qt::Key_Q)
        {
            systemRef->getInput()->Q = false;
        }
        if(event->key() == Qt::Key_E)
        {
            systemRef->getInput()->E = false;
        }
        if(event->key() == Qt::Key_Z)
        {
        }
        if(event->key() == Qt::Key_X)
        {
            if (systemRef->getEditorMode())
                        systemRef->getInput()->X = false;
        }
        if(event->key() == Qt::Key_Up)
        {
            systemRef->getInput()->UP = false;
        }
        if(event->key() == Qt::Key_Down)
        {
            systemRef->getInput()->DOWN = false;
        }
        if(event->key() == Qt::Key_Left)
        {
            systemRef->getInput()->LEFT = false;
        }
        if(event->key() == Qt::Key_Right)
        {
            systemRef->getInput()->RIGHT = false;
        }
        if(event->key() == Qt::Key_U)
        {
        }
        if(event->key() == Qt::Key_O)
        {
        }
        if (event->key() == Qt::Key_Tab)
        {
            bool check = !systemRef->getEditorMode();
            mMainWindow->on_PlayPause_clicked(check);
        }
    }
}

void RenderSystem::mousePressEvent(QMouseEvent *event)
{
    if(systemRef != nullptr)
    {
        if (event->button() == Qt::RightButton)
            systemRef->getInput()->RMB = true;
        if (event->button() == Qt::LeftButton)
            systemRef->getInput()->LMB = true;
        if (event->button() == Qt::MiddleButton)
            systemRef->getInput()->MMB = true;

        if(event->button() == Qt::LeftButton)
        {
            mousePicking(event);
        }
    }
}

void RenderSystem::mouseReleaseEvent(QMouseEvent *event)
{
    if(systemRef != nullptr)
    {
        if (event->button() == Qt::RightButton)
            systemRef->getInput()->RMB = false;
        if (event->button() == Qt::LeftButton)
            systemRef->getInput()->RMB = false;
        if (event->button() == Qt::MiddleButton)
            systemRef->getInput()->RMB = false;
    }
}

void RenderSystem::wheelEvent(QWheelEvent *event)
{
    QPoint numDegrees = event->angleDelta() / 8;

    if(systemRef != nullptr)
    {
        //if RMB, change the speed of the camera
        if (systemRef->getInput()->RMB == true)
        {
            if (numDegrees.y() < 1)

            if (numDegrees.y() > 1)
                systemRef->setCameraSpeed(-0.001f);
        }
        event->accept();
    }
}

void RenderSystem::mouseMoveEvent(QMouseEvent *event)
{
    if(systemRef != nullptr)
    {
        QCursor c = mMainWindow->cursor();
        if (systemRef->getEditorMode())
        {

            if (systemRef->getInput()->RMB)
            {
                //Using mMouseXYlast as deltaXY so we don't need extra variables
                mMouseXlast = event->pos().x() - mMouseXlast;
                mMouseYlast = event->pos().y() - mMouseYlast;

                if (mMouseXlast != 0)
                    editorCamera->yaw(systemRef->getCameraRotateSpeed() * mMouseXlast);
                if (mMouseYlast != 0)
                    editorCamera->pitch(systemRef->getCameraRotateSpeed() * mMouseYlast);
            }
            mMouseXlast = event->pos().x();
            mMouseYlast = event->pos().y();
            c.setShape(Qt::ArrowCursor);
            setCursor(c);
        }
        //    else if(!editorMode) //SKRIV OM TIL Å VÆRE 3D PERSON CAMERA
        //    {
        //            QPoint windowCenter(mMainWindow->x() + mMainWindow->width() / 2,
        //                                mMainWindow->y() + mMainWindow->height() / 2);

        //            //Using mMouseXYlast as deltaXY so we don't need extra variables

        //            mMouseXlast = windowCenter.x() - c.pos().x();
        //            mMouseYlast = windowCenter.y() - c.pos().y();

        //             mCurrentCamera->yaw(-mMouseXlast * mouseSpeed);
        //             mCurrentCamera->pitch(-mMouseYlast * mouseSpeed);
        //             c.setPos(QPoint(windowCenter.x(), windowCenter.y()));
        //             c.setShape(Qt::BlankCursor);
        //             setCursor(c);

        //    }
    }
}
