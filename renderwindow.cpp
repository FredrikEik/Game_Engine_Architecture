#include "renderwindow.h"
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
#include "vector4d.h"
#include "level.h"
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


    mShaderPrograms[0] = new Shader((gsl::ShaderFilePath + "plainvertex.vert").c_str(),
                                    (gsl::ShaderFilePath + "plainfragment.frag").c_str());
                                     qDebug() << "Plain shader program id: " << mShaderPrograms[0]->getProgram();
    mShaderPrograms[1] = new Shader((gsl::ShaderFilePath + "textureshader.vert").c_str(),
                                    (gsl::ShaderFilePath + "textureshader.frag").c_str());
                                     qDebug() << "Texture shader program id: " << mShaderPrograms[1]->getProgram();
    mShaderPrograms[2] = new Shader((gsl::ShaderFilePath + "skyboxvertex.vert").c_str(),
                                    (gsl::ShaderFilePath + "skyboxfragment.frag").c_str());
                                     qDebug() << "Texture shader program id: " << mShaderPrograms[2]->getProgram();


                                     setupPlainShader(0);
                                     setupTextureShader(1);
                                     setupSkyboxShader(2);

    //**********************  Texture stuff: **********************
    //Returns a pointer to the Texture class. This reads and sets up the texture for OpenGL
    //and returns the Texture ID that OpenGL uses from Texture::id()
    glActiveTexture(GL_TEXTURE0);
    mTextures[0] = new Texture();
    glActiveTexture(GL_TEXTURE1);
    mTextures[1] = new Texture("hund.bmp");
    glActiveTexture(GL_TEXTURE2);
    mTextures[2] = new Texture("right.bmp",
                               "left.bmp",
                               "top.bmp",
                               "bottom.bmp",
                               "front.bmp",
                               "back.bmp");

    //mTextures
    //Set the textures loaded to a texture unit

    glBindTexture(GL_TEXTURE_2D, mTextures[0]->mGLTextureID);
    glBindTexture(GL_TEXTURE_2D, mTextures[1]->mGLTextureID);
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
    /*mShaderPrograms[0] = new Shader((gsl::ShaderFilePath + "plainvertex.vert").c_str(),
/*    mShaderPrograms[0] = new Shader((gsl::ShaderFilePath + "plainvertex.vert").c_str(),
                                (gsl::ShaderFilePath + "plainfragment.frag").c_str());
    qDebug() << "Plain shader program id: " << mShaderPrograms[0]->getProgram();

    mShaderPrograms[1] = new Shader((gsl::ShaderFilePath + "textureshader.vert").c_str(),
                                    (gsl::ShaderFilePath + "textureshader.frag").c_str());
    qDebug() << "Texture shader program id: " << mShaderPrograms[1]->getProgram();

    mShaderPrograms[2] = new Shader((gsl::ShaderFilePath + "lightShader.vert").c_str(),
                                    (gsl::ShaderFilePath + "lighShader.frag").c_str());
    qDebug() << "Light shader shader program id: " << mShaderPrograms[2]->getProgram();



    setupPlainShader(0);
    setupTextureShader(1);*/

    //********************** Saving meshes to be drawn **********************
    setupTextureShader(1);
    setupLightShader(2);
    //********************** Making the object to be drawn **********************

    factory->saveMesh("../GEA2021/Assets/Meshes/mariocube.obj", "MarioCube");
    factory->saveMesh("../GEA2021/Assets/Meshes/sphere.obj", "Sphere");



    //GameObject *skybox = new Skybox("../GEA2021/Assets/skybox.obj");







    //********************** Set up camera **********************
    glDisable(GL_CULL_FACE);
    /*mTestFrustumCamera = new Camera(45.0f, 4/3);
    mTestFrustumCamera->init();
    //mTestFrustumCamera->setPosition(gsl::Vector3D(0.f, 0.f, 0.f));
*/
    mCurrentCamera = new Camera(90, 4/3);
    mCurrentCamera->init();
    mCurrentCamera->setPosition(gsl::Vector3D(0.f, 0.f, 0.f));
	
    mShaderPrograms[1] = new Shader((gsl::ShaderFilePath + "textureshader.vert").c_str(),
                                    (gsl::ShaderFilePath + "textureshader.frag").c_str());
                                     qDebug() << "Texture shader program id: " << mShaderPrograms[1]->getProgram();
    mShaderPrograms[2] = new Shader((gsl::ShaderFilePath + "lightShader.vert").c_str(),
                                    (gsl::ShaderFilePath + "lightShader.frag").c_str());
                                     qDebug() << "Light shader program id: " << mShaderPrograms[2]->getProgram();


    setupPlainShader(0);
    setupTextureShader(1);
    setupLightShader(2);

    //********************** Set up quadtree *******************
    gsml::Point2D nw{-10,-10}, ne{10,-10}, sw{-10, 10}, se{10, 10}; //specifies the quadtree area
    mQuadtree.init(nw, ne, sw, se);

    //********************** Sound set up **********************

    //SoundSource* mMario{};
    //SoundSource* mVideoGameLand{};
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

    //mVideoGameLand->play();
    //mVideoGameLand2->play();





    //mMario->play(); //doesnt work
    //mExplosionSound->play();
    //mExplosionSound->setPosition(Vector3(200.0f, 30.0f, -1000.0f));


/*
GameObject *temp=nullptr;
    for(int i{0}; i < 50; i++)
    {
        for(int j{0}; j < 10; j++)
        {
            temp = factory->createObject("Cube");
            temp->getTransformComponent()->mMatrix.setPosition(2.f*i,0.f,2.f*j);
            temp->getSphereCollisionComponent()->center = gsl::Vector3D(2.f*i,0.f,2.f*j);
            //TODO: Scaling have to be made easier and more automatic than this!
        }
    }
    */

    level.loadLevel("../GEA2021/Saves/testLevel.json");

    //Save level test
    /*
    std::multimap<std::string, struct SpawnSettings> objectMap;
    for(int i = 0; i < factory->mGameObjects.size(); i++)
    {
        SpawnSettings settings;
        std::string objectType = factory->mGameObjects[i]->mObjectType;
        settings.initialPos =  gsl::Vector3D{0,0,0};//factory->mGameObjects[i]->getTransformComponent()->mMatrix.getPosition();
        settings.initialScale = gsl::Vector3D{1,1,1};
        settings.initialRot = gsl::Vector3D{0,0,0};
        //gsl::Vector3D scale = t.mMatrix.getScale();
        //gsl::Vector3D rot = t.mMatrix.getRotator();
        objectMap.insert(std::pair<std::string, struct SpawnSettings>(objectType, settings));
    }
    level.saveLevelAs("savedLevel", objectMap);
    */
    factory->createObject("Skybox");
    player = factory->createObject("Sphere");
    player->getTransformComponent()->mMatrix.setPosition(0.f,0.6f,0.f);
    player->getTransformComponent()->mMatrix.setScale(0.1f,0.1f,0.1f);


    factory->openLevel(level);


GameObject *temp=nullptr;
        for(int i{0}; i < 50; i++)
        {
            for(int j{0}; j < 10; j++)
            {
                temp = factory->createObject("Cube");
                temp->getTransformComponent()->mMatrix.setPosition(2.f*i,0.f,2.f*j);
                temp->getSphereCollisionComponent()->center = gsl::Vector3D(2.f*i,0.f,2.f*j);
                //TODO: Scaling have to be made easier and more automatic than this!
            }
        }

            mMainWindow->updateOutliner(factory->mGameObjects);
             hjelpeObjekt = factory->createObject("Cube");

}

// Called each frame - doing the rendering
void RenderWindow::render()
{
    //Keyboard / mouse input
    handleInput();

    mCurrentCamera->update(mCurrentCamera->FOV, mCurrentCamera->aRatio);

    mTimeStart.restart(); //restart FPS clock
    mContext->makeCurrent(this); //must be called every frame (every time mContext->swapBuffers is called)

    initializeOpenGLFunctions();    //must call this every frame it seems...

    //to clear the screen for each redraw
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(0); //reset shader type before rendering


    //Light* lightRef = static_cast<Light*>(factory->mGameObjects["Light"]);

    //Draws the objects

    //mCurrentCamera->draw();
    //mTestFrustumCamera->draw();

    //This should be in a loop! <- Ja vi må loope dette :/
    if(factory->mGameObjects.size() > 0)
    {

        for(int i{0}; i < factory->mGameObjects.size(); i++)

		{	


            unsigned int shaderProgramIndex = factory->mGameObjects[i]->getMaterialComponent()->mShaderProgram;
			glUseProgram(mShaderPrograms[shaderProgramIndex]->getProgram()); // What shader program to use
			//send data to shader

            if(shaderProgramIndex == 1)
            {
                glUniform1i(mTextureUniform[shaderProgramIndex], factory->mGameObjects[i]->getMaterialComponent()->mTextureUnit);
            }
            if(shaderProgramIndex == 2)
            {
               glUniform1i(mSkyboxUniform, factory->mGameObjects[i]->getMaterialComponent()->mTextureUnit);
            }


            glUniform1i(mTextureUniform[shaderProgramIndex], factory->mGameObjects[i]->getMaterialComponent()->mTextureUnit);
			glUniformMatrix4fv( vMatrixUniform[shaderProgramIndex], 1, GL_TRUE, mCurrentCamera->mViewMatrix.constData());
			glUniformMatrix4fv( pMatrixUniform[shaderProgramIndex], 1, GL_TRUE, mCurrentCamera->mProjectionMatrix.constData());
            glUniformMatrix4fv( mMatrixUniform[shaderProgramIndex], 1, GL_TRUE, factory->mGameObjects[i]->getTransformComponent()->mMatrix.constData());
            //factory->mGameObjects[i]->draw();
            if(!bPause)
            {
                factory->mGameObjects[i]->move(0.0f, 0.0f, -0.025f);
            }

            if(factory->mGameObjects[i]->mObjectName == "Skybox") //Makes skybox follow player
            {

                factory->mGameObjects[i]->getTransformComponent()->mMatrix.setPosition(mCurrentCamera->mPosition.x, mCurrentCamera->mPosition.y, mCurrentCamera->mPosition.z);
            }

            if(toggleFrustumCulling)
			{
            gsl::Vector3D rightPlaneToObjectVector = mCurrentCamera->nearPlaneBottomRight - factory->mGameObjects[i]->getSphereCollisionComponent()->center;
            float rightPlaneHeightToObject = gsl::Vector3D::dot(rightPlaneToObjectVector, mCurrentCamera->rightPlaneNormal);
            if(rightPlaneHeightToObject + factory->mGameObjects[i]->getSphereCollisionComponent()->radius >= 0)
            {
                gsl::Vector3D leftPlaneToObjectVector = mCurrentCamera->nearPlaneTopLeft - factory->mGameObjects[i]->getSphereCollisionComponent()->center;
                float leftPlaneHeightToObject = gsl::Vector3D::dot(leftPlaneToObjectVector, mCurrentCamera->leftPlaneNormal);
                if(leftPlaneHeightToObject + factory->mGameObjects[i]->getSphereCollisionComponent()->radius >= 0)
                /*{
                    gsl::Vector3D nearPlaneToObjectVector = mCurrentCamera->nearPlaneBottomRight - factory->mGameObjects[i]->getSphereCollisionComponent()->center;
                    float nearPlaneHeightToObject = gsl::Vector3D::dot(nearPlaneToObjectVector, mCurrentCamera->nearPlaneNormal);
                    if(nearPlaneHeightToObject + factory->mGameObjects[i]->getSphereCollisionComponent()->radius >= 0)
                    {
                        gsl::Vector3D farPlaneToObjectVector = mCurrentCamera->farPlaneBottomLeft - factory->mGameObjects[i]->getSphereCollisionComponent()->center;
                        float farPlaneHeightToObject = gsl::Vector3D::dot(farPlaneToObjectVector, mCurrentCamera->farPlaneNormal);
                        if(farPlaneHeightToObject + factory->mGameObjects[i]->getSphereCollisionComponent()->radius >= 0)
                        {
                            gsl::Vector3D topPlaneToObjectVector = mCurrentCamera->nearPlaneTopRight - factory->mGameObjects[i]->getSphereCollisionComponent()->center;
                            float topPlaneHeightToObject = gsl::Vector3D::dot(topPlaneToObjectVector, mCurrentCamera->topPlaneNormal);
                            if(topPlaneHeightToObject + factory->mGameObjects[i]->getSphereCollisionComponent()->radius >= 0)
                            {
                                gsl::Vector3D bottomPlaneToObjectVector = mCurrentCamera->nearPlaneBottomLeft - factory->mGameObjects[i]->getSphereCollisionComponent()->center;
                                float bottomPlaneHeightToObject = gsl::Vector3D::dot(bottomPlaneToObjectVector, mCurrentCamera->bottomPlaneNormal);
                                if(bottomPlaneHeightToObject + factory->mGameObjects[i]->getSphereCollisionComponent()->radius >= 0)
                                */
                                {
                                   // qDebug() << "Object inside frustum";
                                    factory->mGameObjects[i]->draw();
                                }
                            }
                        /*}
                    }
                }
            }*/
                if(dynamic_cast<Camera*>(factory->mGameObjects[i]) != nullptr)
                {
                    factory->mGameObjects[i]->draw();
                }
			}
            else
            {
                factory->mGameObjects[i]->draw();
            }

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


            if (i==mIndexToPickedObject) {

                //driver å må lage noe hjelpe objekt.

            hjelpeObjektMesh = new MeshComponent;
            hjelpeObjektMesh = factory->mGameObjects[i]->getMeshComponent();
            hjelpeObjektMesh->mDrawType = GL_LINES;
            hjelpeObjekt->setMeshComponent(hjelpeObjektMesh);

            if (hjelpeObjekt != factory->mGameObjects[i]){
            gsl::Vector3D tempPosition;
            gsl::Vector3D tempScale;
            tempPosition = factory->mGameObjects[i]->getTransformComponent()->mMatrix.getPosition();
            hjelpeObjekt->getTransformComponent()->mMatrix.setPosition(tempPosition.x, tempPosition.y, tempPosition.z);
            tempScale = factory->mGameObjects[i]->getTransformComponent()->mMatrix.getScale();
            hjelpeObjekt->getTransformComponent()->mMatrix.setScale(tempScale.x*1.2f, tempScale.y*1.2f, tempScale.z*1.2f);
            if (tempScale.x > 50 || tempScale.y > 50 || tempScale.z > 50 ){

                hjelpeObjekt->getTransformComponent()->mMatrix.setScale(1.2f, 1.2f, 1.2f);

              }
            }
            else if (hjelpeObjekt == factory->mGameObjects[i]){
                mIndexToPickedObject = 0;
            }

                //factory->mGameObjects[i]->setMeshComponent(hjelpeObjektMesh);
            }


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
    mTextureUniform[shaderIndex] = glGetUniformLocation(mShaderPrograms[shaderIndex]->getProgram(), "textureSampler");
   // factory->mGameObjects[shaderIndex]->getMaterialComponent()->mTextureUniform = glGetUniformLocation(mShaderPrograms[shaderIndex]->getProgram(), "textureSampler");
}

void RenderWindow::setupSkyboxShader(int shaderIndex)
{
    mMatrixUniform[shaderIndex] = glGetUniformLocation( mShaderPrograms[shaderIndex]->getProgram(), "mMatrix" );
    vMatrixUniform[shaderIndex] = glGetUniformLocation( mShaderPrograms[shaderIndex]->getProgram(), "vMatrix" );
    pMatrixUniform[shaderIndex] = glGetUniformLocation( mShaderPrograms[shaderIndex]->getProgram(), "pMatrix" );
    mSkyboxUniform = glGetUniformLocation(mShaderPrograms[shaderIndex]->getProgram(), "cubeSampler");
}

void RenderWindow::setupLightShader(int shaderIndex)

{
    mMatrixUniform[shaderIndex] = glGetUniformLocation( mShaderPrograms[shaderIndex]->getProgram(), "mMatrix" );
    vMatrixUniform[shaderIndex] = glGetUniformLocation( mShaderPrograms[shaderIndex]->getProgram(), "vMatrix" );
    pMatrixUniform[shaderIndex] = glGetUniformLocation( mShaderPrograms[shaderIndex]->getProgram(), "pMatrix" );
    //mTextureUniform = glGetUniformLocation(mShaderPrograms[shaderIndex]->getProgram(), "cubeSampler");
    mSkyboxUniform = glGetUniformLocation(mShaderPrograms[shaderIndex]->getProgram(), "cubeSampler");
    mTextureUniform[shaderIndex] = glGetUniformLocation(mShaderPrograms[shaderIndex]->getProgram(), "textureSampler");

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
    mClick->play();
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
    GameObject* newObject = factory->createObject(objectName);
    gsl::Vector3D position = newObject->getTransformComponent()->mMatrix.getPosition();
    gsml::Point2D position2D = std::pair<double, double>(position.getX(), position.getY());
    uint32_t id = newObject->ID;

    mQuadtree.insert(position2D, id, newObject);
      mMainWindow->updateOutliner(factory->mGameObjects);
}

void RenderWindow::playPausebutton(const QSurfaceFormat &format)
{
    bPause = !bPause;

    mClick->play();

    if(bPause)
    {
        mVideoGameLand->stop();
        reset(format);
    }
    else
    {
                mVideoGameLand->play();

        if (mIndexToPickedObject > -1){
        hjelpeObjekt->getTransformComponent()->mMatrix.setScale(1,1,1);
        hjelpeObjektMesh->mDrawType = GL_TRIANGLES;
        hjelpeObjekt->setMeshComponent(hjelpeObjektMesh);
        mIndexToPickedObject = -1;
        }




    }
}

void RenderWindow::reset(const QSurfaceFormat &format)
{
//    factory->mGameObjects.clear();
//    mRenderTimer->stop();


//    //Make the gameloop timer:
//    mRenderTimer = new QTimer(this);

//    mMainWindow->init();
//    RenderWindow::init();

//    mRenderTimer->start(16);    //starts the timer
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
    if(editorMode)
    {
    if(mInput.RMB) //Sett in character movement HER!
    {
        if(mInput.W)
            mCurrentCamera->setSpeed(-mCameraSpeed*2);
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
    else if(!editorMode)
    {
        //FREE FLY
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

void RenderWindow::spawnHelpObject()
{


}

//void RenderWindow::moveHelpObjectToSelected()
//{

//}

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
            if(distance < 0.5f && editorMode && factory->mGameObjects[i]->mObjectName != "Skybox")
            {
    //            qDebug() << "Collision with object index" << i << distance << "meters away from ray";
                mIndexToPickedObject = i;
                mMainWindow->selectObjectByIndex(mIndexToPickedObject);
                //factory->mGameObjects[i]->move(1000.f,0,0);

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
    if(event->key() == Qt::Key_R)
    {
         reset(format());
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
    if (event->key() == Qt::Key_Tab)

    {
        bool check = !bPause;
        mMainWindow->on_PlayPause_clicked(check);
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
    QCursor c = mMainWindow->cursor();
    if (editorMode)
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
    c.setShape(Qt::ArrowCursor);
    setCursor(c);
    }
    else if(!editorMode) //SKRIV OM TIL Å VÆRE 3D PERSON CAMERA
    {
            QPoint windowCenter(mMainWindow->x() + mMainWindow->width() / 2,
                                mMainWindow->y() + mMainWindow->height() / 2);

            //Using mMouseXYlast as deltaXY so we don't need extra variables

            mMouseXlast = windowCenter.x() - c.pos().x();
            mMouseYlast = windowCenter.y() - c.pos().y();

             mCurrentCamera->yaw(-mMouseXlast * mouseSpeed);
             mCurrentCamera->pitch(-mMouseYlast * mouseSpeed);
             c.setPos(QPoint(windowCenter.x(), windowCenter.y()));
             c.setShape(Qt::BlankCursor);
             setCursor(c);

    }


}
