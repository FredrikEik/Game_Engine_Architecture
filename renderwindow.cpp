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


/*!
 * \brief RenderWindow::init
 *
 */
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
    mTextures[2] = new Texture("SpaceInvaders1.bmp");
    mTextures[3] = new Texture("SpaceInvaders2.bmp");
    mTextures[4] = new Texture("SpaceInvaders3.bmp");
    mTextures[5] = new Texture("SpaceInvaders4.bmp");
    mTextures[6] = new Texture("SpaceInvaderBoss1.bmp");
    mTextures[7] = new Texture("SpaceInvadersBoss2.bmp");
    mTextures[8] = new Texture("skybox", true); //bitmap
    mTextures[9] = new Texture("truck.obj"); //bitmap
    //Set the textures loaded to a texture unit
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, mTextures[0]->mGLTextureID);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, mTextures[1]->mGLTextureID);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, mTextures[2]->mGLTextureID);
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, mTextures[3]->mGLTextureID);
    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_2D, mTextures[4]->mGLTextureID);
    glActiveTexture(GL_TEXTURE5);
    glBindTexture(GL_TEXTURE_2D, mTextures[5]->mGLTextureID);
    glActiveTexture(GL_TEXTURE6);
    glBindTexture(GL_TEXTURE_2D, mTextures[6]->mGLTextureID);
    glActiveTexture(GL_TEXTURE7);
    glBindTexture(GL_TEXTURE_2D, mTextures[7]->mGLTextureID);
    glActiveTexture(GL_TEXTURE8);
    glBindTexture(GL_TEXTURE_CUBE_MAP, mTextures[8]->mGLTextureID); //cubemap
    glActiveTexture(GL_TEXTURE9);
    glBindTexture(GL_TEXTURE_2D, mTextures[9]->mGLTextureID);
    //Start the Qt OpenGL debugger
    //Really helpfull when doing OpenGL
    //Supported on most Windows machines
    //reverts to plain glGetError() on Mac and other unsupported PCs
    // - can be deleted
    startOpenGLDebugger();

    //general OpenGL stuff:
    glEnable(GL_DEPTH_TEST);            //enables depth sorting - must then use GL_DEPTH_BUFFER_BIT in glClear
    //glEnable(GL_CULL_FACE);       //draws only front side of models - usually what you want - test it out!
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

    mShaderPrograms[3] = new Shader((gsl::ShaderFilePath + "Skyboxshader.vert").c_str(),
                                    (gsl::ShaderFilePath + "Skyboxshader.frag").c_str());
    qDebug() << "Skybox shader program id: " << mShaderPrograms[3]->getProgram();

    setupPlainShader(0);
    setupTextureShader(1);
    setupPhongShader(2);
    setupSkyboxshader(3);


    //********************** Making the object to be drawn **********************

    /****************** THIS SHOULD USE A RESOURCE MANAGER / OBJECT FACTORY!!!!! ******************************************/
    /***** should not use separate classes init() - function ****************/

    ////*************************************start**////////////
    JSS->JSONSystemInit(this);
    ResSys->ResourceSystemInit(RenderSys);

    ///PURE ECS TEST
    entitySys->construct("cube.obj", QVector3D(0.0f,-3.0f,0.0f),3,8,-1);
    meshCompVec[0]->IsCollidable = false;
    entitySys->construct("XYZ", QVector3D(0.0f,0.0f,0.0f),0,0,-1,GL_LINES);

    entitySys->construct("Suzanne.obj", QVector3D(-5.0f,0.0f,0.0f),2,0);
    entitySys->construct("plane.obj", QVector3D(-5.0f,0.0f,0.0f),2,0);
    entitySys->construct("bowlSurface.obj", QVector3D(0.0f,0.0f,0.0f),2,1);
    entitySys->construct("sphere.obj", QVector3D(15.0f,15.0f,15.0f),2,1);
    entitySys->construct("sphere.obj", QVector3D(5.0f,0.0f,0.0f),2,0);
    entitySys->construct("Suzanne.obj", QVector3D(0.0f,0.0f,0.0f),1,1);
    entitySys->construct("head.obj", QVector3D(0.0f,0.0f,0.0f),2,0);

    Physics->InitPhysicsSystem(meshCompVec[4],ResSys->getVertexDataByName("bowlSurface.obj"));
    entitySys->construct("SpaceInvader1.obj", QVector3D(0.0f + 20 ,0.0f,-20.f), 2,2);
    entitySys->construct("SpaceInvader2.obj", QVector3D(0.0f + 40 ,0.0f,-20.f), 2,3);
    entitySys->construct("SpaceInvader3.obj", QVector3D(0.0f + 60 ,0.0f,-20.f), 2,4);
    entitySys->construct("SpaceInvader4.obj", QVector3D(0.0f + 80 ,0.0f,-20.f), 2,5);
    entitySys->construct("SpaceInvaderBoss1.obj", QVector3D(0.0f + 110 ,0.0f,-20.f), 2,6);
    entitySys->construct("SpaceInvaderBoss2.obj", QVector3D(0.0f + 140 ,0.0f,-20.f), 2,7);





    //Suzannes - using default material:
    for(int i{0}; i < 30; i++)
    {
        for(int j{0}; j < 30; j++)
        {
            entitySys->construct("SpaceInvader1.obj", QVector3D(0.0f + 20*i ,0.0f,-20.f*j), 2,2);
            //temp->mTransform->mMatrix.translate(1.f*i, 0.f, -2.f*j);

        }
    }
    //JSS->SaveLevel("Test");


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

    //********************** Set up camera **********************
    mCurrentCamera = new Camera(50.f, 0.1f,300.f);//(50.f, 0.1f,300.f); //test case (20.f, 20.1f,300.f)
    mCurrentCamera->setPosition(gsl::Vector3D(200.f, 100.f, 200.f));

    mPlayerCamera = new Camera(20.f, 0.1f,300.f);//(50.f, 0.1f,300.f); //test case (20.f, 20.1f,300.f)
    mPlayerCamera->setPosition(gsl::Vector3D(200.f, 100.f, 200.f));

    mEditorCamera = mCurrentCamera;//(50.f, 0.1f,300.f); //test case (20.f, 20.1f,300.f)
    mEditorCamera->setPosition(gsl::Vector3D(200.f, 100.f, 200.f));






    //player

    if(transformCompVec[2] != nullptr){
        CurrentPlayer = transformCompVec[2];
    }


    //LASDATA


    for(int i{0}; i < 5; i++)
    {
        for(int j{0}; j < 5; j++)
        {
            entitySys->construct("sphere.obj", QVector3D( 200 + 8*(i) ,100.0f,200 + 8*(j)),2,1);
        }
    }
    //for(int i = 0; i < transformCompVec.size(); i++){
    //    transformCompVec[i]->PosOverTime.push_back(transformCompVec[i]->mMatrix.getPosition());
    //}



    //physics code
    oldTime = std::chrono::high_resolution_clock::now();


    mSong->pause();
    mMainWindow->updateViewPort();
}

// Called each frame - doing the job of the RenderSystem!!!!!
void RenderWindow::render()
{


    mMainWindow->updateDetails();

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

    unsigned long long eSize = entities.size();
    for(unsigned long long i = 0; i < eSize; i++)
    {
        if(entities[i] == meshCompVec[i]->entity && entities[i] == transformCompVec[i]->entity && entities[i] == MaterialCompVec[i]->entity)
        {

            //killZ :D

            killZ(transformCompVec[i], gsl::Vector3D(200  ,100.0f,200 ));

            if(entities[i] == 0) glDepthMask(GL_FALSE); //depthmask for skybox off

            //frustum culling
            frustumCulling(i);

            glUseProgram(mShaderPrograms[MaterialCompVec[i]->mShaderProgram]->getProgram());
            switchProgram(i);   //assignes propper values for programs
            switchLOD(i);       //switches to correct lod


            if(isPhysicsEnabled && transformCompVec[i]->isPhysicsEnabled)
            {

                Physics->move(DeltaTime,transformCompVec[i], meshCompVec[i]->collisionRadius);

            }

            RenderSys->draw(meshCompVec[i],
                            MaterialCompVec[i],
                            transformCompVec[i],
                            viewMatrix,
                            projectionMatrix,
                            modelMatrix,
                            mCurrentCamera);

            if(entities[i] == 0)  glDepthMask(GL_TRUE); //debtmask for skybox on

        }


    }


    if(bIsPlayerCamera)
    {
        mCurrentCamera->setPosition(CurrentPlayer->mMatrix.getPosition() + gsl::Vector3D(0.0f,10.0f,30.0f));
    }
    else
    {
        drawFrostum();      //frustum culling lines! This is a visualisation of frostum

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

    CalcDeltaTime();

    glUseProgram(0); //reset shader type before next frame. Got rid of "Vertex shader in program _ is being recompiled based on GL state"
}

void RenderWindow::killZ(TransformComponent *Transform, gsl::Vector3D SpawnPoint)
{
    if(Transform->isPhysicsEnabled && (Transform->mMatrix.getPosition().getY() < -1000.0f))
    {
        Transform->Velocity = gsl::Vector3D(0.0f,0.0f,0.0f); //reset velocity
        Transform->mMatrix.setPosition(SpawnPoint.getX(), SpawnPoint.getY(), SpawnPoint.getZ());
    }
}

gsl::Vector3D RenderWindow::MakeGSLvec3D(QVector3D vec)
{
    gsl::Vector3D temp;
    temp.setX(vec.x());
    temp.setY(vec.y());
    temp.setZ(vec.z());
    return temp;
}

QVector3D RenderWindow::MakeQvec3D(gsl::Vector3D vec)
{
    QVector3D temp;
    temp.setX(vec.getX());
    temp.setY(vec.getY());
    temp.setZ(vec.getZ());
    return temp;
}
void RenderWindow::CalcDeltaTime()
{
    auto newTime = std::chrono::high_resolution_clock::now();

    float elapsed_time_ms = std::chrono::duration<double, std::milli>(newTime-oldTime).count();
    oldTime = newTime;

    DeltaTime = elapsed_time_ms/1000.f;
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

void RenderWindow::setupPhongShader(int index)
{
    mTextureUniform2 = glGetUniformLocation( mShaderPrograms[index]->getProgram(), "uTexture" );
    mUsingTextureUniform = glGetUniformLocation( mShaderPrograms[index]->getProgram(), "usingTextures" );

    mMmatrixUniform2 = glGetUniformLocation( mShaderPrograms[index]->getProgram(), "mMatrix" );
    mVmatrixUniform2 = glGetUniformLocation( mShaderPrograms[index]->getProgram(), "vMatrix" );
    mPmatrixUniform2 = glGetUniformLocation( mShaderPrograms[index]->getProgram(), "pMatrix" );

    mLightColorUniform = glGetUniformLocation( mShaderPrograms[index]->getProgram(), "lightColor" );
    mObjectColorUniform = glGetUniformLocation( mShaderPrograms[index]->getProgram(), "objectColor" );
    mAmbientLightStrengthUniform = glGetUniformLocation( mShaderPrograms[index]->getProgram(), "ambientStrength" );
    mLightPositionUniform = glGetUniformLocation( mShaderPrograms[index]->getProgram(), "lightPosition" );
    mSpecularStrengthUniform = glGetUniformLocation( mShaderPrograms[index]->getProgram(), "specularStrength" );
    mSpecularExponentUniform = glGetUniformLocation( mShaderPrograms[index]->getProgram(), "specularExponent" );
    mLightPowerUniform = glGetUniformLocation( mShaderPrograms[index]->getProgram(), "lightStrength" );
    mCameraPositionUniform = glGetUniformLocation( mShaderPrograms[index]->getProgram(), "cameraPosition" );

}

void RenderWindow::setupSkyboxshader(int shaderIndex)
{
    vMatrixUniform3 = glGetUniformLocation( mShaderPrograms[shaderIndex]->getProgram(), "view" );
    pMatrixUniform3 = glGetUniformLocation( mShaderPrograms[shaderIndex]->getProgram(), "projection" );
    mMatrixUniform3 = glGetUniformLocation( mShaderPrograms[shaderIndex]->getProgram(), "mMatrix" );
    POSUniform3 = glGetUniformLocation( mShaderPrograms[shaderIndex]->getProgram(), "POS" );
    skyboxUniform3 = glGetUniformLocation( mShaderPrograms[shaderIndex]->getProgram(), "skybox" );
}


void RenderWindow::togglePlayerCamera()
{
    if(mPlayerCamera && mEditorCamera)
    {
        if(bIsPlayerCamera)
        {
            mCurrentCamera = mPlayerCamera;
            bIsPlayerCamera = false;


        }
        else
        {
            mCurrentCamera = mEditorCamera;
            bIsPlayerCamera = true;


        }

    }
}

void RenderWindow::RayCasting(QMouseEvent *event)
{
    ///https://antongerdelan.net/opengl/raycasting.html - source - + help from laila and synnøve
    int mouseXPixel = event->pos().x();
    int mouseYPixel = event->pos().y(); //y is 0 at top of screen!

    gsl::Matrix4x4 projMatrix = mCurrentCamera->Cam.mProjectionMatrix;
    gsl::Matrix4x4 viewMatrix = mCurrentCamera->Cam.mViewMatrix;

    //step 1
    float x = (2.0f * mouseXPixel) / width() - 1.0f;
    float y = 1.0f - (2.0f * mouseYPixel) / height();
    float z = 1.0f;
    QVector3D ray_nds = QVector3D(x, y, z);

    //step 2
    QVector4D ray_clip = QVector4D(ray_nds.x(), ray_nds.y(), -1.0, 1.0);

    //step 3
    projMatrix.inverse();
    QVector4D ray_eye = projMatrix.multiplyWithQVector4D(ray_clip);
    ray_eye = QVector4D(ray_eye.x(), ray_eye.y(), -1.0, 0.0);


    //step 4
    viewMatrix.inverse();
    QVector4D temp = viewMatrix.multiplyWithQVector4D(ray_eye);
    QVector3D ray_wor = {temp.x(), temp.y(), temp.z()};
    ray_wor.normalize();



    qDebug() << ray_wor<<"x: "<<mouseXPixel << "  Y: " <<mouseYPixel;
    RayCastSphereCollision(ray_wor);
}

void RenderWindow::RayCastSphereCollision(QVector3D RayVec)
{
    QVector3D CamPos;
    CamPos.setX(mCurrentCamera->Cam.mPosition.getX());
    CamPos.setY(mCurrentCamera->Cam.mPosition.getY());
    CamPos.setZ(mCurrentCamera->Cam.mPosition.getZ());

    float radiusOfcollider = 0;
    bool collided = false;
    int eSize = entities.size();
    float lenght = 0;
    // work out components of quadratic
    for(int i = 0; i < eSize; i++)
    {
        if(meshCompVec[i]->IsCollidable)
        {

            //get position vector into qvec3d
            QVector3D position;
            position.setX( transformCompVec[i]->mMatrix.getPosition().getX());
            position.setY( transformCompVec[i]->mMatrix.getPosition().getY());
            position.setZ( transformCompVec[i]->mMatrix.getPosition().getZ());
            //find collision radius for object
            for(int j = 0; j < eSize; j++)
            {
                if(transformCompVec[i]->entity == meshCompVec[j]->entity)
                {
                    radiusOfcollider = meshCompVec[j]->collisionRadius;
                }
            }

            QVector3D v = QVector3D(position.x(), position.y(),position.z()) - CamPos;
            lenght = v.length();
            long double a = QVector3D::dotProduct(RayVec,RayVec);
            long double b = 2.0 * QVector3D::dotProduct(v, RayVec);
            long double c = QVector3D::dotProduct(v, v) - radiusOfcollider * radiusOfcollider;
            long double b_squared_minus_4ac = b * b + (-4.0) * a * c;

            if (b_squared_minus_4ac == 0)
            {
                // One real root //colliding
                //return true;
                mMainWindow->setSelectedItem(transformCompVec[i]->entity);
                collided = true;
                entitySys->construcRay(RayVec,CamPos,lenght);
                qDebug() <<"COL1"<<CamPos;
                break;
            }
            else if (b_squared_minus_4ac > 0)
            {
                // Two real roots  //colliding
                long double x1 = (-b - sqrt(b_squared_minus_4ac)) / (2.0 * a);
                long double x2 = (-b + sqrt(b_squared_minus_4ac)) / (2.0 * a);

                if (x1 >= 0.0 || x2 >= 0.0)
                {
                    mMainWindow->setSelectedItem(transformCompVec[i]->entity);//return true;
                    collided = true;
                    entitySys->construcRay(RayVec,CamPos,lenght);
                    qDebug() <<"COL2"<<CamPos;
                    break;
                }
                if (x1 < 0.0 || x2 >= 0.0)
                {
                    mMainWindow->setSelectedItem(transformCompVec[i]->entity);//return true;
                    collided = true;
                    entitySys->construcRay(RayVec,CamPos,lenght);
                    qDebug() <<"COL3"<<CamPos;
                    break;
                }
            }
        }
    }

    if(!collided)
    {qDebug() <<"NO COLLISION";}
    entitySys->construcRay(RayVec,CamPos,lenght);
    // No real roots //NOT colliding
    //return false;

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

bool RenderWindow::frustumCulling(int Index)
{

    //tatt fra ole.experiment frostum culling
    mCurrentCamera->calculateFrustumVectors();

    //vector from position of cam to object;
    gsl::Vector3D vectorToObject =transformCompVec[Index]->mMatrix.getPosition()
            - mCurrentCamera->Cam.mPosition;

    //radius of object sphere
    float gobRadius = meshCompVec[Index]->collisionRadius;
    //Mesh data is not scaled so have to calculate for that
    //TODO: The system will break if scaling is not uniform...
    //gobRadius *= mGameObjects[gobIndex]->mTransform->mScale.x;
    //CURRENTLY NO SCALE

    //if radius is not set == very small
    if(gobRadius <= 0.000001f)
    {
        meshCompVec[Index]->isDrawable = true;
        return false;
    }

    //length of vectorToObject onto frustum normal
    float tempDistance{0.f};

    //shortcut to frustum
    Frustum &frustum = mCurrentCamera->mFrustum;

    //the collider sphere seems to be a little to small, so adding this
    //padding to not cull them to early
    float padding{0.2f}; //

    //Project vector down to frustum normals:

    //top plane:
    tempDistance = frustum.mToptPlane * vectorToObject;    // * here is dot product
    if(tempDistance > (gobRadius + padding))
    {
        meshCompVec[Index]->isDrawable = false;
        return true;
    }

    //bottom plane:
    tempDistance = frustum.mBottomPlane * vectorToObject;    // * here is dot product
    if(tempDistance > (gobRadius + padding))
    {
        meshCompVec[Index]->isDrawable = false;
        return true;
    }
    //Right plane:
    tempDistance = frustum.mRightPlane * vectorToObject;    // * here is dot product
    if(tempDistance > (gobRadius + padding))
    {
        meshCompVec[Index]->isDrawable = false;
        return true;

    }


    //Left plane:
    tempDistance = frustum.mLeftPlane * vectorToObject;    // * here is dot product
    if(tempDistance > (gobRadius + padding))
    {
        meshCompVec[Index]->isDrawable = false;
        return true;
    }

    //insert the rest of planes here

    meshCompVec[Index]->isDrawable = true;
    return false;
}

void RenderWindow::setCameraSpeed(float value)
{
    mCameraSpeed += value;

    //Keep within some min and max values
    if(mCameraSpeed < 0.01f)
        mCameraSpeed = 0.01f;
    if (mCameraSpeed > 1.f)
        mCameraSpeed = 1.f;

    qDebug() << "Camera Speed: " << mCurrentCamera->getCameraSpeed();
}

void RenderWindow::handleInput()
{
    //Camera
    mCurrentCamera->setSpeed(0.f);  //cancel last frame movement
    if(mInput.RMB && !bIsPlayerCamera)
    {
        if(mInput.W)
        {
            mCurrentCamera->setSpeed(-mCameraSpeed);

        }
        if(mInput.S)
        {
            mCurrentCamera->setSpeed(mCameraSpeed);


        }
        if(mInput.D)
        {

            mCurrentCamera->moveRight(mCameraSpeed);

        }
        if(mInput.A)
        {
            mCurrentCamera->moveRight(-mCameraSpeed);

        }
        if(mInput.Q)
            mCurrentCamera->updateHeigth(-mCameraSpeed);
        if(mInput.E)
            mCurrentCamera->updateHeigth(mCameraSpeed);
    }
    else
    {
        if(bIsPlayerCamera)
        {
            if(mInput.W)
            {
                mCurrentCamera->setSpeed(-mCameraSpeed);
                PosZ = -1.0f;
                CurrentPlayer->mMatrix.translateZ(PosZ);
            }
            if(mInput.S)
            {
                mCurrentCamera->setSpeed(mCameraSpeed);
                PosZ = 1.0f;
                CurrentPlayer->mMatrix.translateZ(PosZ);

            }
            if(mInput.D)
            {
                posX = 1.0f;
                CurrentPlayer->mMatrix.translateX(posX);
                mCurrentCamera->moveRight(mCameraSpeed);

            }
            if(mInput.A)
            {
                posX = -1.0f;
                CurrentPlayer->mMatrix.translateX(posX);
                mCurrentCamera->moveRight(-mCameraSpeed);

            }
        }
    }

}

void RenderWindow::switchProgram(int shaderIndex)
{

    if(entities[shaderIndex] == 0)
    {
        glDepthMask(GL_FALSE);
        transformCompVec[0]->mMatrix.setPosition(mCurrentCamera->Cam.mPosition.getX(),mCurrentCamera->Cam.mPosition.getY(),mCurrentCamera->Cam.mPosition.getZ());
    }

    if (MaterialCompVec[shaderIndex]->mShaderProgram == 0)
    {
        viewMatrix = vMatrixUniform;
        projectionMatrix = pMatrixUniform;
        modelMatrix = mMatrixUniform;
    }
    else if (MaterialCompVec[shaderIndex]->mShaderProgram == 1)
    {
        viewMatrix = vMatrixUniform1;
        projectionMatrix = pMatrixUniform1;
        modelMatrix = mMatrixUniform1;
        //Now mMaterial component holds texture slot directly - probably should be changed
        glUniform1i(mTextureUniform, MaterialCompVec[shaderIndex]->mTextureUnit);
    }
    else if (MaterialCompVec[shaderIndex]->mShaderProgram == 2)
    {

        viewMatrix = mVmatrixUniform2;
        projectionMatrix = mPmatrixUniform2;
        modelMatrix = mMmatrixUniform2;
        glUniform1f(mUsingTextureUniform, true); // turns on texture
        glUniform3f(mCameraPositionUniform, mCurrentCamera->Cam.mPosition.getX(), mCurrentCamera->Cam.mPosition.getY(), mCurrentCamera->Cam.mPosition.getZ()); // pos camera
        glUniform3f(mLightPositionUniform, mCurrentCamera->Cam.mPosition.getX(), mCurrentCamera->Cam.mPosition.getY(), mCurrentCamera->Cam.mPosition.getZ()); // pos lightsource! booiiiii
        glUniform1i(mTextureUniform2,  MaterialCompVec[shaderIndex]->mTextureUnit);
    }
    else if (MaterialCompVec[shaderIndex]->mShaderProgram == 3)
    {

        viewMatrix = vMatrixUniform3;
        projectionMatrix = pMatrixUniform3;
        modelMatrix = mMatrixUniform3;
        glUniform3f(POSUniform3,mCurrentCamera->Cam.mPosition.getX(), mCurrentCamera->Cam.mPosition.getY(), mCurrentCamera->Cam.mPosition.getZ());//THIS BABOON IS POS IN SHADER NOT MODEL
        glUniform1i(skyboxUniform3, MaterialCompVec[shaderIndex]->mTextureUnit);

    }
}

void RenderWindow::switchLOD(int shaderIndex)
{
    if(meshCompVec[shaderIndex]->LODEnabled){
        //LOD SWITCHER
        //calc length between obj and camera.
        //use length to switch LOD level
        //use length and LOD level to switch only one time
        gsl::Vector3D LODlength = transformCompVec[shaderIndex]->mMatrix.getPosition() - mCurrentCamera->position();

        if(LODlength.length() < 10.0f)
        {
            //qDebug() << "LOD level 0";
            //LOD LEVEL 0
            meshCompVec[shaderIndex]->LODLevel = 0;
        }
        else if(LODlength.length() < 20.0f)
        {
            //qDebug() << "LOD level 1";
            //LOD LEVEL 1
            meshCompVec[shaderIndex]->LODLevel = 1;
        }
        else if(LODlength.length() > 20.0f)
        {
            //qDebug() << "LOD level 2";
            //LOD LEVEL 2
            meshCompVec[shaderIndex]->LODLevel = 2;
        }
    }
    else{
        meshCompVec[shaderIndex]->LODLevel = 0;
    }
}

void RenderWindow::drawFrostum()
{
    if(mCurrentCamera->mFrustum.isDrawable)
    {

        glUseProgram(mShaderPrograms[0]->getProgram()); // using plain shader program
        meshData* frustum = ResSys->makeFrustum(mCurrentCamera->mFrustum, RenderSys);
        gsl::Matrix4x4 temp(true);
        temp.translate(mCurrentCamera->Cam.mPosition);
        temp.rotateY(-mCurrentCamera->Cam.mYaw);
        temp.rotateX(-mCurrentCamera->Cam.mPitch);

        initializeOpenGLFunctions();    //must call this every frame it seems...

        glUniformMatrix4fv( vMatrixUniform, 1, GL_TRUE, mCurrentCamera->Cam.mViewMatrix.constData());
        glUniformMatrix4fv( pMatrixUniform, 1, GL_TRUE, mCurrentCamera->Cam.mProjectionMatrix.constData());
        glUniformMatrix4fv( mMatrixUniform, 1, GL_TRUE,temp.constData());

        //draw the object
        glBindVertexArray( frustum->VAO );
        glDrawArrays(frustum->DrawType, 0, frustum->meshVert.size());
        glBindVertexArray(0);

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
        setCameraSpeed(0.1f);
    }
    if(event->key() == Qt::Key_X)
    {
        setCameraSpeed(-0.1f);
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
    {
        RayCasting(event);
        mInput.LMB = true;
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

void RenderWindow::wheelEvent(QWheelEvent *event)
{
    QPoint numDegrees = event->angleDelta() / 8;

    //if RMB, change the speed of the camera
    if (mInput.RMB)
    {
        if (numDegrees.y() < 1)
            setCameraSpeed(0.1f);
        if (numDegrees.y() > 1)
            setCameraSpeed(-0.1f);
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
