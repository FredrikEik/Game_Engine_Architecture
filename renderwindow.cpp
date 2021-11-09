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
#include "GameObject.h"
#include "camera.h"
#include "constants.h"
#include "texture.h"
#include "Components.h"
#include "ResourceManager.h"
#include "constants.h"
#include "soundmanager.h"
#include "coreengine.h"

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

    frustum = new Frustum();
}

RenderWindow::~RenderWindow()
{

}

// Sets up the general OpenGL stuff and the buffers needed to render a triangle
void RenderWindow::init()
{
    mResourceManager = &ResourceManager::getInstance();

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

    mShaderPrograms[2] = new Shader((gsl::ShaderFilePath + "phong.vert").c_str(),
                                    (gsl::ShaderFilePath + "phong.frag").c_str());
    qDebug() << "Phong shader program id: " << mShaderPrograms[2]->getProgram();

    setupPlainShader(0);
    setupTextureShader(1);
    setupPhongShader(2);

    CoreEngine::getInstance()->SetUpScene();
    CoreEngine::getInstance()->startGameLoopTimer();

    mMainWindow->makeObjList(mGameObjects);
}

// Called each frame - doing the rendering
void RenderWindow::render()
{
    mTimeStart.restart(); //restart FPS clock
    mContext->makeCurrent(this); //must be called every frame (every time mContext->swapBuffers is called)

    initializeOpenGLFunctions();    //must call this every frame it seems...

    //to clear the screen for each redraw
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(0);

    //Draws the objects
    for( int i = 0; i < mGameObjects.size(); i++)
    {
    /** Cube stuff */
        if(mGameObjects[i]->mName == "cube.obj")
        {
            checkForCollisions(mGameObjects[i]);
            mGameObjects[i]->move(moveX, moveY, moveZ);
            setPlayerMovement(0,0,0); //resets movement. (stops constant movement after buttonpress)
        }
        else if(mGameObjects[i]->mName == "bullet.obj")
        {
            //This is temporary to push bullets forward
            //TODO: Use the players vector to move bullets forward in relation
            //      to the players forwardVector from when the bullet was shot.
            gsl::Vector3D pos = mGameObjects[i]->transform->mMatrix.getPosition();
            mGameObjects[i]->transform->mMatrix.setPosition(pos.x, pos.y, pos.z - 0.2f);
        }

    /** Shader Program */
        glUseProgram(mShaderPrograms[mGameObjects[i]->material->mShaderProgram]->getProgram());

        if(mGameObjects[i]->material->mShaderProgram == 0) /** PlainShader */
        {
            glUniformMatrix4fv( vMatrixUniform, 1, GL_TRUE, mCurrentCamera->mViewMatrix.constData());
            glUniformMatrix4fv( pMatrixUniform, 1, GL_TRUE, mCurrentCamera->mProjectionMatrix.constData());
            glUniformMatrix4fv( mMatrixUniform, 1, GL_TRUE, mGameObjects[i]->transform->mMatrix.constData());
        }
        else if(mGameObjects[i]->material->mShaderProgram == 1) /** TextureShader */
        {
            glUniform1i(mTextureUniform, mGameObjects[i]->material->mTextureUnit);
            glUniformMatrix4fv( vMatrixUniform1, 1, GL_TRUE, mCurrentCamera->mViewMatrix.constData());
            glUniformMatrix4fv( pMatrixUniform1, 1, GL_TRUE, mCurrentCamera->mProjectionMatrix.constData());
            glUniformMatrix4fv( mMatrixUniform1, 1, GL_TRUE, mGameObjects[i]->transform->mMatrix.constData());
        }
        else if(mGameObjects[i]->material->mShaderProgram == 2) /** PhongShader */
        {
//            glUniform1i(mPhongTextureUniform,TextureIndex);
            glUniformMatrix4fv( vMatrixUniform2, 1, GL_TRUE, mCurrentCamera->mViewMatrix.constData());
            glUniformMatrix4fv( pMatrixUniform2, 1, GL_TRUE, mCurrentCamera->mProjectionMatrix.constData());
            glUniformMatrix4fv( mMatrixUniform2, 1, GL_TRUE, mGameObjects[i]->transform->mMatrix.constData());

            glUniform3f(mCameraPositionUniform, mCurrentCamera->position().x, mCurrentCamera->position().y, mCurrentCamera->position().z);
            glUniform3f(mLightPositionUniform, -50.f, 50.f, 50.f);
        }


    /** Calculate FRUSTUM */


        gsl::Vector3D ObjectPos = mGameObjects[i]->transform->mMatrix.getPosition();

        //Check if mForward.x & .y = 0. This fixes the problem where nothing renders at the start
        //This happened because all "objDistanceFromPlane" floats = 0, because vectors got multiplied by 0.

        if(bFrustumEnabled && mGameObjects[i]->mName != "camera.obj" &&
            (mCurrentCamera->getmForward().x != 0.f || mCurrentCamera->getmForward().y != 0.f))
        {
            mGameObjects[i]->mesh->renderObject = false;

            gsl::Vector3D CameraToObject = ObjectPos - mCurrentCamera->position();

            float distanceFromFarPlane = (CameraToObject * mCurrentCamera->getmForward()) / mCurrentCamera->getmForward().length();

            if(distanceFromFarPlane > frustum->farPlane) continue;

            gsl::Vector3D leftPlaneNormal = -mCurrentCamera->getmRight();
            leftPlaneNormal.rotateY(frustum->FOV);
            float ObjDistanceFromLeftPlane = (CameraToObject * leftPlaneNormal) / leftPlaneNormal.length();

            if(ObjDistanceFromLeftPlane > 0) continue;

            gsl::Vector3D rightPlaneNormal = mCurrentCamera->getmRight();
            rightPlaneNormal.rotateY(-frustum->FOV);
            float ObjDistanceFromRightPlane = (CameraToObject * rightPlaneNormal) / rightPlaneNormal.length();

            if(ObjDistanceFromRightPlane > 0) continue;

            gsl::Vector3D topPlaneNormal = -mCurrentCamera->getmForward();
            topPlaneNormal.rotateX(-frustum->FOV);
            float ObjDistanceFromTopPlane = (CameraToObject * topPlaneNormal) / topPlaneNormal.length();

            if(ObjDistanceFromTopPlane > 0) continue;

            gsl::Vector3D bottomPlaneNormal = -mCurrentCamera->getmForward();
            bottomPlaneNormal.rotateX(frustum->FOV);
            float ObjDistanceFromBottomPlane = (CameraToObject * bottomPlaneNormal) / bottomPlaneNormal.length();

            if(ObjDistanceFromBottomPlane > 0) continue;

            mGameObjects[i]->mesh->renderObject = true;
        }

    /** Draw FRUSTUM */
        if(mGameObjects[i]->mName == "camera.obj" && bFrustumEnabled) //Frustum needs to be drawn after camera is drawn.
        {                                                             //(So the frustum starts in the same position as the camera)
            float tanFOV = tanf(frustum->FOV/2);
            float widthNear = tanFOV * frustum->nearPlane;
            float widthFar = tanFOV * frustum->farPlane;

            float heightNear = widthNear / mAspectratio;
            float heightFar = widthFar / mAspectratio;

            gsl::Vector3D topRightNear = gsl::Vector3D(widthNear, heightNear, -frustum->nearPlane);
            gsl::Vector3D topLeftNear = gsl::Vector3D(-widthNear, heightNear, -frustum->nearPlane);
            gsl::Vector3D bottomRightNear = gsl::Vector3D(widthNear, -heightNear, -frustum->nearPlane);
            gsl::Vector3D bottomLeftNear = gsl::Vector3D(-widthNear, -heightNear, -frustum->nearPlane);

            gsl::Vector3D topRightFar = gsl::Vector3D(widthFar, heightFar, -frustum->farPlane);
            gsl::Vector3D topLeftFar = gsl::Vector3D(-widthFar, heightFar, -frustum->farPlane);
            gsl::Vector3D bottomRightFar = gsl::Vector3D(widthFar, -heightFar, -frustum->farPlane);
            gsl::Vector3D bottomLeftFar = gsl::Vector3D(-widthFar, -heightFar, -frustum->farPlane);


            Mesh frustumLines;

            frustumLines.mVertices[0].insert( frustumLines.mVertices[0].end(),
            {  //Nearplane vertexes:
               Vertex{ bottomLeftNear.x,  bottomLeftNear.y,  bottomLeftNear.z,  1, 0, 0,   0.f, 0.f},
               Vertex{ bottomRightNear.x, bottomRightNear.y, bottomRightNear.z, 1, 0, 0,   0.f, 0.f},
               Vertex{ topRightNear.x,    topRightNear.y,    topRightNear.z,    1, 0, 0,   0.f, 0.f},
               Vertex{ topLeftNear.x,     topLeftNear.y,     topLeftNear.z,     1, 0, 0,   0.f, 0.f},
               //Farplane vertexes:
               Vertex{ bottomLeftFar.x,   bottomLeftFar.y,   bottomLeftFar.z,   1, 0, 0,   0.f, 0.f},
               Vertex{ bottomRightFar.x,  bottomRightFar.y,  bottomRightFar.z,  1, 0, 0,   0.f, 0.f},
               Vertex{ topRightFar.x,     topRightFar.y,     topRightFar.z,     1, 0, 0,   0.f, 0.f},
               Vertex{ topLeftFar.x,      topLeftFar.y,      topLeftFar.z,      1, 0, 0,   0.f, 0.f},
            });

            frustumLines.mIndices[0].insert( frustumLines.mIndices[0].end(),
            {
                0, 1, 1, 2, 2, 3, 3, 0,       //front rectangle
                4, 5, 5, 6, 6, 7, 7, 4,       //back rectangle
                0, 4, 3, 7,                   //left side
                1, 5, 2, 6                    //right side
            });

            frustumLines.mDrawType = GL_LINES;

            mResourceManager->init(frustumLines, 0);

            glBindVertexArray(frustumLines.mVAO[0]);
            glDrawElements(frustumLines.mDrawType, frustumLines.mIndices[0].size(), GL_UNSIGNED_INT, nullptr);
        }

    /** LOD */
        float  distanceToObject = (ObjectPos - mCurrentCamera->position()).length();

        //first: LOD for each object enabled? Second: is LOD enabled in general? (is it toggled in mainwindow)
        if(mGameObjects[i]->mesh->bLodEnabled && bLODToggleEnabled){
            if(distanceToObject > 50)
                mGameObjects[i]->mesh->lodLevel = 2;
            else if(distanceToObject > 20)
                mGameObjects[i]->mesh->lodLevel = 1;
            else
                mGameObjects[i]->mesh->lodLevel = 0;
        }
        else
            mGameObjects[i]->mesh->lodLevel = 0;


        int tempLod = mGameObjects[i]->mesh->lodLevel;
        glBindVertexArray( mGameObjects[i]->mesh->mVAO[tempLod]);
        glDrawArrays(mGameObjects[i]->mesh->mDrawType, 0, mGameObjects[i]->mesh->mVertices[tempLod].size());
        glBindVertexArray(0);
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

    glUseProgram(0);
}

int RenderWindow::getIndexToPickedObject()
{
    return indexToPickedObject;
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
    mTextureUniform = glGetUniformLocation( mShaderPrograms[shaderIndex]->getProgram(), "textureSampler" );
}

void RenderWindow::setupPhongShader(int shaderIndex)
{
    mMatrixUniform2 = glGetUniformLocation( mShaderPrograms[shaderIndex]->getProgram(), "mMatrix" );
    vMatrixUniform2 = glGetUniformLocation( mShaderPrograms[shaderIndex]->getProgram(), "vMatrix" );
    pMatrixUniform2 = glGetUniformLocation( mShaderPrograms[shaderIndex]->getProgram(), "pMatrix" );

    mLightColorUniform = glGetUniformLocation( mShaderPrograms[shaderIndex]->getProgram(), "lightColor" );
    mObjectColorUniform = glGetUniformLocation( mShaderPrograms[shaderIndex]->getProgram(), "objectColor" );
    mAmbientLightStrengthUniform = glGetUniformLocation( mShaderPrograms[shaderIndex]->getProgram(), "ambientStrength" );
    mLightPositionUniform = glGetUniformLocation( mShaderPrograms[shaderIndex]->getProgram(), "lightPosition" );
    mSpecularStrengthUniform = glGetUniformLocation( mShaderPrograms[shaderIndex]->getProgram(), "specularStrength" );
    mSpecularExponentUniform = glGetUniformLocation( mShaderPrograms[shaderIndex]->getProgram(), "specularExponent" );
    mLightPowerUniform = glGetUniformLocation( mShaderPrograms[shaderIndex]->getProgram(), "lightPower" );
    mCameraPositionUniform = glGetUniformLocation( mShaderPrograms[shaderIndex]->getProgram(), "cameraPosition" );
}

void RenderWindow::mousePicking(QMouseEvent *event)
{
    if(mCoreEngine->isPlaying())
    {
        //Should have some code to calculate the cubes forwardVector.
        //Im not quite sure how to do it, and i've spent too much time trying to find out.
        //Moving on for now, but this should be done before oblig 3 delivery.

        mCoreEngine->shootBullet(/* bulletDirection vector */);


    }
    else // isplaying == true
    {
        int mousePixelX = event->pos().x();
        int mousePixelY = event->pos().y();

        float x = (2.0f * mousePixelX) / width() - 1.0f;
        float y = 1.0f - (2.0f * mousePixelY) / height();

        gsl::Matrix4x4 projMatrix = mCurrentCamera->mProjectionMatrix;
        gsl::Matrix4x4 viewMatrix = mCurrentCamera->mViewMatrix;

        gsl::Vector4D ray_clip{x, y, -1.0, 1.0};

        projMatrix.inverse();
        gsl::Vector4D ray_eye = projMatrix * ray_clip;
        ray_eye = gsl::Vector4D(ray_eye.x, ray_eye.y, -1.0, 0.0);

        viewMatrix.inverse();
        gsl::Vector4D temp = viewMatrix * ray_eye;
        gsl::Vector3D ray_wor{temp.x, temp.y, temp.z};
        ray_wor.normalize();

        for(int i{0}; i < mGameObjects.size(); i++)
        {
            gsl::Vector3D ObjectPos = mGameObjects[i]->transform->mMatrix.getPosition();
            gsl::Vector3D CameraToObject = ObjectPos - mCurrentCamera->position();

            gsl::Vector3D planeNormal = gsl::Vector3D::cross(ray_wor, CameraToObject); //Cross-product
            gsl::Vector3D rayNormal = gsl::Vector3D::cross(planeNormal, ray_wor);      //Cross-product
            rayNormal.normalize();

            float distance = gsl::Vector3D::dot(CameraToObject, rayNormal);

            //Absolute distance
            distance = abs(distance);

            //if distance to ray < objects bounding sphere == collision
            if(distance < mGameObjects[i]->mesh->sphereRadius)
            {
                qDebug() << "You clicked" << QString::fromStdString(mGameObjects[i]->mName) << "in gameObjects[" << i << "]";

                if(mGameObjects[i]->mName == "camera.obj")
                    indexToPickedObject = -1;
                else
                    indexToPickedObject = i;


            /** Put arrow above selected object */
                gsl::Vector3D pos = mGameObjects[i]->transform->mMatrix.getPosition();
                mCoreEngine->MoveSelectionArrow(pos);

                break;  //breaking out of for loop - does not check if the ray is touching several objects
            }
            else{ //if no object is selected, move arrow out of sight.
                indexToPickedObject = -1;
            }
        }
    }
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

    //calculate aspect ration and set projection matrix
    mAspectratio = static_cast<float>(width()) / height();
    //    qDebug() << mAspectratio;
    mCoreEngine = CoreEngine::getInstance();
    mCoreEngine->initCameraProjectionMatrixes(mAspectratio);
    //    qDebug() << mCurrentCamera.mProjectionMatrix;
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
            mMainWindow->statusBar()->showMessage(
                    "Entities rendered: " + QString::number(mGameObjects.size())+ "    |    " +
                    "Vertices: " + QString::number(getVertexCount()) + "    |    " +
                    "Player Colliding: " + QString::number(bPlayerColliding) + "    |    " +
                    " Time pr FrameDraw: " + QString::number(nsecElapsed/1000000.f, 'g', 4) + " ms    |    " +
                    "FPS (approximated): " + QString::number(1E9 / nsecElapsed, 'g', 7)
            );
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

void RenderWindow::toggleLOD()
{
    if(bLODToggleEnabled)
        bLODToggleEnabled = false;
    else
        bLODToggleEnabled = true;
}

void RenderWindow::toggleFrustum()
{
    if(bFrustumEnabled)
        bFrustumEnabled = false;
    else
        bFrustumEnabled = true;
}

Input RenderWindow::getInput()
{
    return mInput;
}

void RenderWindow::setPlayerMovement(float x, float y, float z)
{
    moveX = x;
    moveY = y;
    moveZ = z;
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

void RenderWindow::checkForCollisions(GameObject* player) //Checks all other objects..
{
    bPlayerColliding = false;

    gsl::Vector3D objToPlayer{0,0,0};
    for( int i = 0; i < mGameObjects.size(); i++){
        if(mGameObjects[i] != player && mGameObjects[i]->mesh->collisionsEnabled == true)
        {
            objToPlayer = mGameObjects[i]->transform->mMatrix.getPosition() - player->transform->mMatrix.getPosition();

            if( objToPlayer.length() <= (player->mesh->sphereRadius + mGameObjects[i]->mesh->sphereRadius) )
            { //Objects are colliding
                bPlayerColliding = true;
            }
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

void RenderWindow::addToGameObjects(GameObject *obj)
{
    mGameObjects.push_back(obj);
}
std::vector<GameObject*> RenderWindow::getGameObjects()
{
    return mGameObjects;
}

std::vector<GameObject*>* RenderWindow::getGameObjectsPtr()
{
    return &mGameObjects;
}

double RenderWindow::getVertexCount() //Tror ikke den teller vertices helt riktig..
{
    int vertexCount = 0;
    for( int i = 0; i < mGameObjects.size(); i++)
    {
        if(mGameObjects[i]->mesh->renderObject == true)
        {
            if(mGameObjects[i]->mesh->lodLevel == 0)
                vertexCount += mGameObjects[i]->mesh->mVertices[0].size();

            else if(mGameObjects[i]->mesh->lodLevel == 1)
                vertexCount += mGameObjects[i]->mesh->mVertices[1].size();

            else if(mGameObjects[i]->mesh->lodLevel == 2)
                vertexCount += mGameObjects[i]->mesh->mVertices[2].size();
        }
    }
    return vertexCount;
}

int RenderWindow::getSelectedObject()
{
    return indexToPickedObject;
}

void RenderWindow::setToCurrentCamera(Camera *cam)
{
    mCurrentCamera = cam;
}

Camera *RenderWindow::getCurrentCamera()
{
    return mCurrentCamera;
}

void RenderWindow::keyPressEvent(QKeyEvent *event)
{
//    Input &input = CoreEngine::getInstance()->mInput;

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
//    Input &input = CoreEngine::getInstance()->mInput;

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
//    Input &input = CoreEngine::getInstance()->mInput;

    if (event->button() == Qt::RightButton)
        mInput.RMB = true;
    if (event->button() == Qt::LeftButton){
        mInput.LMB = true;
        mousePicking(event);
    }
    if (event->button() == Qt::MiddleButton)
        mInput.MMB = true;
}

void RenderWindow::mouseReleaseEvent(QMouseEvent *event)
{
//    Input &input = CoreEngine::getInstance()->mInput;

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

//    Input &input = CoreEngine::getInstance()->mInput;

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
    if (mInput.RMB && mCurrentCamera != mCoreEngine->getGameCamera())// if right mouse button is held && Editorcamera == current
    {
        //Using mMouseXYlast as deltaXY so we don't need extra variables
        mMouseXlast = event->pos().x() - mMouseXlast;
        mMouseYlast = event->pos().y() - mMouseYlast;

        if (mMouseXlast != 0)
            mCurrentCamera->yaw(mCameraRotateSpeed * mMouseXlast);
        if (mMouseYlast != 0)
            mCurrentCamera->pitch(mCameraRotateSpeed * mMouseYlast);

        mMouseXlast = event->pos().x();
        mMouseYlast = event->pos().y();
    }
    mMouseXlast = event->pos().x();
    mMouseYlast = event->pos().y();
}
