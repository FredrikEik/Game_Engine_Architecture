#include "rendersystem.h"

#include <QOpenGLContext>
#include <QOpenGLDebugLogger>

#include "ResourceManager/shaderhandler.h"
#include "mainwindow.h"
#include "gameobject.h"
#include "camera.h"
#include "ResourceManager/texturehandler.h"
#include "ResourceManager/gameobjectmanager.h"
#include "soundsystem.h"
#include "coreengine.h"
#include "ResourceManager/meshhandler.h"    //to check linebox

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

    mLogger = Logger::getInstance();

    //Print render version info (what GPU is used):
    //(Have to use cout to see text- qDebug just writes numbers...)
    //Nice to see if you use the Intel GPU or the dedicated GPU on your laptop
    // - can be deleted
    mLogger->logText("The active GPU and API:", LColor::HIGHLIGHT);

    std::string tempString;

    tempString +=  std::string("  Vendor: ") + std::string((char*)glGetString(GL_VENDOR)) + "\n" +
                   std::string("  Renderer: ") + std::string((char*)glGetString(GL_RENDERER)) + "\n" +
                   std::string(" Version: ") + std::string((char*)glGetString(GL_VERSION)) + "\n";
    mLogger->logText(tempString);

    //Get the texture units of your GPU
    int mTextureUnits; //Supported Texture Units (slots) pr shader. - maybe have in header!?
    int textureUnits;
    glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &textureUnits);
    tempString.clear();
    tempString += "  This GPU as " + std::to_string(textureUnits) + " texture units / slots in total, ";
    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &mTextureUnits);
    tempString += "and supports " + std::to_string(mTextureUnits) + " texture units pr shader";
    mLogger->logText(tempString);

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

    //********************** Making the object to be drawn **********************
    //Safe to do here because we know OpenGL is started
    //Probably should be placed elsewhere
    CoreEngine::getInstance()->setUpScene();

    mGameObjectManager = &GameObjectManager::getInstance();

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
    int cullSafe = mIsPlaying ? -1 : 1;
    int startObject = mIsPlaying ? -1 : 1;
    for(int i{startObject}; i < mGameObjects.size(); i++)
    {
        /************** LOD and Frustum culling stuff ***********************/
        gsl::Vector3D cameraPos = mEditorCamera->mPosition;
        gsl::Vector3D gobPos = mGameObjects[i]->mTransform->mMatrix.getPosition();

        if(mUseFrustumCulling && i > cullSafe)
        {
            if(frustumCulling(i))
                continue;
        }


        /*************************************/

        //First object - xyz
        //what shader to use
        //Now mMaterial component holds index into mShaderPrograms!! - probably should be changed
        int shaderIndex = mGameObjects[i]->mMaterial->mShaderProgram;
        ShaderHandler *tempShader = mGameObjectManager->mShaders[shaderIndex];
        glUseProgram(tempShader->mProgram);

        //This block sets up the uniforms for the shader used in the material
        //Also sets up texture if needed.
        int viewMatrix{-1};
        int projectionMatrix{-1};
        int modelMatrix{-1};

        viewMatrix = tempShader->vMatrixUniform;
        projectionMatrix = tempShader->pMatrixUniform;
        modelMatrix = tempShader->mMatrixUniform;

        if(tempShader->mTextureUniform > -1)
        {
            //Now mMaterial component holds texture slot directly - probably should be changed
            glUniform1i(tempShader->mTextureUniform, mGameObjects[i]->mMaterial->mTextureUnit);
        }

        //send data to shader
        if(mIsPlaying)
        {
            glUniformMatrix4fv( viewMatrix, 1, GL_TRUE, mGameCamera->mViewMatrix.constData());
            glUniformMatrix4fv( projectionMatrix, 1, GL_TRUE, mGameCamera->mProjectionMatrix.constData());
        }
        else
        {
            glUniformMatrix4fv( viewMatrix, 1, GL_TRUE, mEditorCamera->mViewMatrix.constData());
            glUniformMatrix4fv( projectionMatrix, 1, GL_TRUE, mEditorCamera->mProjectionMatrix.constData());
        }
        glUniformMatrix4fv( modelMatrix, 1, GL_TRUE, mGameObjects[i]->mTransform->mMatrix.constData());

        //draw the object
        //***Quick hack*** LOD test:
        if(mGameObjects[i]->mMesh->mVertexCount[1] > 0) //mesh has LODs
        {
            gsl::Vector3D distanceVector = gobPos - cameraPos;
            //LOD calculation
            float length = distanceVector.length();

            if (length < 5)
            {
                glBindVertexArray( mGameObjects[i]->mMesh->mVAO[0] );
                glDrawArrays(mGameObjects[i]->mMesh->mDrawType, 0, mGameObjects[i]->mMesh->mVertexCount[0]);
                mVerticesDrawn += mGameObjects[i]->mMesh->mVertexCount[0];
                mObjectsDrawn++;
            }
            else if(length < 20)
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
        if(i == mIndexToPickedObject)
        {
            tempShader = mGameObjectManager->mShaders[0];
//            MeshData lineBox = CoreEngine::getInstance()->mResourceManager->makeLineBox("suzanne.obj");
            MeshData circle = CoreEngine::getInstance()->mGameObjectManager->
                    makeCircleSphere(mGameObjects[i]->mMesh->mColliderRadius, false);
            //Hackety hack - have to get rid of scale in the objects model matrix
            gsl::Matrix4x4 temp(true);
            temp.translate(mGameObjects[i]->mTransform->mMatrix.getPosition());
            glUniformMatrix4fv( tempShader->mMatrixUniform, 1, GL_TRUE, temp.constData());
//            glBindVertexArray( lineBox.mVAO[0] );
//            glDrawElements(lineBox.mDrawType, lineBox.mIndexCount[0], GL_UNSIGNED_INT, nullptr);
            glBindVertexArray( circle.mVAO[0] );
            glDrawElements(circle.mDrawType, circle.mIndexCount[0], GL_UNSIGNED_INT, nullptr);
        }
        glBindVertexArray(0);
}

    //Quick hack test to check if frustum line mesh is OK
    if(true)
    {
        ShaderHandler* tempShader = mGameObjectManager->mShaders[0];
        glUseProgram(tempShader->mProgram);
        MeshData frustum = CoreEngine::getInstance()->mGameObjectManager->makeFrustum(mGameCamera->mFrustum);
        gsl::Matrix4x4 temp(true);
        temp.translate(mGameCamera->mPosition);
        temp.rotateY(-mGameCamera->mYaw);
        temp.rotateX(mGameCamera->mPitch);

        glUniformMatrix4fv( tempShader->mMatrixUniform, 1, GL_TRUE, temp.constData());
        glBindVertexArray( frustum.mVAO[0] );
        glDrawElements(frustum.mDrawType, frustum.mIndexCount[0], GL_UNSIGNED_INT, nullptr);

        //Drawing forward vector of gameCam
/*       gsl::Vector3D tempEnd = mGameCamera->mPosition + mGameCamera->mForward;
        MeshData forwardVector = CoreEngine::getInstance()->mResourceManager->mMeshHandler->
                makeLine(mGameCamera->mPosition, tempEnd, 1.f);
        glBindVertexArray( forwardVector.mVAO[0] );
        temp.setToIdentity();
        glUniformMatrix4fv( mShaderPrograms[0]->mMatrixUniform, 1, GL_TRUE, temp.constData());
        glDrawArrays(forwardVector.mDrawType, 0, forwardVector.mVertexCount[0]);

        //Drawing FOV vector of gameCam on right side
        tempEnd = mGameCamera->mPosition + mGameCamera->mFrustum.mRightPlane;
        MeshData frustumCullRightVector = CoreEngine::getInstance()->mResourceManager->mMeshHandler->
                makeLine(mGameCamera->mPosition, tempEnd, 1.f);
        glBindVertexArray( frustumCullRightVector.mVAO[0] );
        temp.setToIdentity();
        glUniformMatrix4fv( mShaderPrograms[0]->mMatrixUniform, 1, GL_TRUE, temp.constData());
        glDrawArrays(frustumCullRightVector.mDrawType, 0, frustumCullRightVector.mVertexCount[0]);
        */
    }

    //Moves the dog triangle - should be made another way!!!!
    if(mIsPlaying)
    {
//        mGameObjects[1]->mTransform->mMatrix.translate(.001f, .001f, -.001f); //just to move the triangle each frame
        mGameCamera->yaw(0.07f);
        mGameCamera->update();
        mUseFrustumCulling = true;
        mGameCamAsFrustumCulling = true;
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

    glUseProgram(0); //reset shader type before next frame. Got rid of "Vertex shader in program _ is being recompiled based on GL state"
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
    float aspectRatio = static_cast<float>(width()) / height();
    mEditorCamera->mFrustum.mAspectRatio = aspectRatio;
    mEditorCamera->calculateProjectionMatrix();

    mGameCamera->mFrustum.mAspectRatio = aspectRatio;
    mGameCamera->calculateProjectionMatrix();
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

void RenderSystem::toggleBacksideCulling(bool state)
{
    state ? glEnable(GL_CULL_FACE):glDisable(GL_CULL_FACE);
}

void RenderSystem::setPickedObject(int pickedID)
{
    mIndexToPickedObject = pickedID;
}
void RenderSystem::cancelPickedObject()
{
    mIndexToPickedObject = -1;
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
                mLogger->logText(message.message().toStdString(), LColor::DAMNERROR);
        }
    }
    else
    {
        GLenum err = GL_NO_ERROR;
        while((err = glGetError()) != GL_NO_ERROR)
        {
            mLogger->logText("glGetError returns " + std::to_string(err), LColor::DAMNERROR);
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
            mLogger->logText("This system can not use QOpenGLDebugLogger, so we revert to glGetError()");

        if(temp->hasExtension(QByteArrayLiteral("GL_KHR_debug")))
        {
            mLogger->logText("System can log OpenGL errors!");
            mOpenGLDebugLogger = new QOpenGLDebugLogger(this);
            if (mOpenGLDebugLogger->initialize()) // initializes in the current context
                mLogger->logText("Started OpenGL debug logger!");
        }
    }
}

bool RenderSystem::frustumCulling(int gobIndex)
{
    Camera *cullCamera{nullptr};
    (mGameCamAsFrustumCulling) ? cullCamera = mGameCamera : cullCamera = mEditorCamera;

    //Vector from position of cam to object;
    gsl::Vector3D vectorToObject = mGameObjects[gobIndex]->mTransform->mMatrix.getPosition()
            - cullCamera->mPosition;

    //radius of object sphere
    float gobRadius = mGameObjects[gobIndex]->mMesh->mColliderRadius;
    //Mesh data is not scaled so have to calculate for that
    //Todo:: The system will break if scaling is not uniform
    gobRadius *= mGameObjects[gobIndex]->mTransform->mScale.x;

    //if radius is not set == very small
    if(gobRadius <= 0.000001f)
        return false;

    //length of the vectorToObject onto frustum normal
    float tempDistance{0.0f};

    //Shortcut to frustum
    Frustum &frustum = cullCamera->mFrustum;

    //Avoid culling objects still slightly in frame
    float padding{0.2f};

    //Project vector down to frustum normals:
    //Right plane:
    tempDistance = frustum.mRightPlane * vectorToObject;
    if(tempDistance > (gobRadius + padding))
        return true;

    //left plane:
    tempDistance = frustum.mBottomPlane * vectorToObject;
    if(tempDistance > (gobRadius + padding))
        return true;

    //near plane:
    tempDistance = frustum.mTopPlane * vectorToObject;
    if(tempDistance > (gobRadius + padding))
        return true;

    //far plane:
    tempDistance = frustum.mTopPlane * vectorToObject;
    if(tempDistance > (gobRadius + padding))
        return true;

    return false;
}

//Directly based on https://antongerdelan.net/opengl/raycasting.html
void RenderSystem::mousePickingRay(QMouseEvent *event)
{
    int mouseXPixel = event->pos().x();
    int mouseYPixel = event->pos().y(); //y is 0 at top of screen!

    //Since we are going to invert these, I make a copy
    gsl::Matrix4x4 projMatrix = mEditorCamera->mProjectionMatrix;
    gsl::Matrix4x4 viewMatrix = mEditorCamera->mViewMatrix;

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

    for(int i{0}; i < mGameObjects.size(); i++)
    {
        //making the vector from camera to object we test against
        gsl::Vector3D camToObject = mGameObjects[i]->mTransform->mMatrix.getPosition() - mEditorCamera->mPosition;

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
        if(distance < mGameObjects[i]->mMesh->mColliderRadius)
        {
//            qDebug() << "Collision with object index" << i << distance << "meters away from ray";
            mIndexToPickedObject = i;
            mMainWindow->selectObjetByIndex(mIndexToPickedObject);
            break;  //breaking out of for loop - does not check if ray touch several objects
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
        mMainWindow->on_actionPlay_triggered(!mIsPlaying);
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
    if(event->key() == Qt::Key_F)
    {
        input.F = true;
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
    if(event->key() == Qt::Key_F)
    {
        input.F = false;
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

    //This should be organized better - just getting it working for now
    if(event->button() == Qt::LeftButton)
        mousePickingRay(event);
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
            mEditorCamera->setCameraSpeed(0.001f);
        if (numDegrees.y() > 1)
            mEditorCamera->setCameraSpeed(-0.001f);
    }
    event->accept();
}

void RenderSystem::mouseMoveEvent(QMouseEvent *event)
{
    this->requestActivate();    //should grab keyboard focus

    Input &input = CoreEngine::getInstance()->mInput;

    if (input.RMB)
    {
        //Using mMouseXYlast as deltaXY so we don't need extra variables
        mMouseXlast = event->pos().x() - mMouseXlast;
        mMouseYlast = event->pos().y() - mMouseYlast;

        if (mMouseXlast != 0)
            mEditorCamera->yaw(mEditorCamera->mCameraRotateSpeed * mMouseXlast);
        if (mMouseYlast != 0)
            mEditorCamera->pitch(mEditorCamera->mCameraRotateSpeed * mMouseYlast);
    }
    mMouseXlast = event->pos().x();
    mMouseYlast = event->pos().y();
}
