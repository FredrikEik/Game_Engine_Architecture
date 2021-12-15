#ifndef RENDERWINDOW_H
#define RENDERWINDOW_H

#include <QWindow>
#include <QOpenGLFunctions_4_1_Core>
#include <QTimer>
#include <QElapsedTimer>

#include "input.h"
#include "constants.h"
#include "components.h"
#include "rendersystem.h"
#include "resourcesystem.h"
#include "entitysystem.h"
#include "collisionsystem.h"
#include "physicssystem.h"

#include "soundmanager.h"
#include "soundsource.h"
#include "vector3.h"
#include "jsonsystem.h"
#include <ctime>    //for physics
#include <chrono>   //for sleep_for
#include <thread>   //for sleep_for

class QOpenGLContext;
class Shader;
class MainWindow;
class VisualObject;
class Camera;
class Texture;

//! This inherits from QWindow to get access to the Qt functionality and
//! OpenGL surface.
//! We also inherit from QOpenGLFunctions, to get access to the OpenGL functions
//! This is the same as using glad and glw from general OpenGL tutorials
//! @class RenderWindow - Our giant render class that has mix of OOP and ECS
class RenderWindow : public QWindow, protected QOpenGLFunctions_4_1_Core
{
    Q_OBJECT
public:
    //! init function Open GL spesific
    RenderWindow(const QSurfaceFormat &format, MainWindow *mainWindow);
    ~RenderWindow() override;

    QOpenGLContext *context() { return mContext; }

    void exposeEvent(QExposeEvent *) override;
    //! Turns Wireframe on or off
    void toggleWireframe(bool buttonState);

    //!  RenderSys - Pointer to current RenderSystem
    RenderSystem *RenderSys = new RenderSystem();

    //! ResSys - Pointer to current Resource System
    resourceSystem * ResSys = new resourceSystem();

    //!  entitySys - Pointer to current Entitysystem
    //!  takes in a pointer to RenderWindow
    EntitySystem * entitySys = new EntitySystem(this);

    //!  collisionSys - Pointer to Collision System
    collisionSystem* collisionSys = new collisionSystem();

    //!  JSS - Pointer to JSON System
    JSONSystem * JSS = new JSONSystem();

    //!  entities vector array holding all the entitie Indexes
    std::vector<int> entities;

    //! @var meshCompVec - Pointer array of type MeshComponent*. Holds pointers to every meshComponent, sorted by Entity Index
    std::vector<MeshComponent*> meshCompVec;

    //! @var transformCompVec - Pointer array of type TransformComponent*. Holds pointers to every TransformComponent, sorted by Entity Index
    std::vector<TransformComponent*> transformCompVec;

    //! @var MaterialCompVec - Pointer array of type MaterialComponent*. Holds pointers to every MaterialComponent, sorted by Entity Index
    std::vector<MaterialComponent*> MaterialCompVec;

    //! @var DeetsVector - Pointer array of type DetailsComponent*. Holds pointers to every DetailsComponent, sorted by Entity Index
    std::vector<DetailsComponent*> DeetsVector;

    //Some sounds...
    //! @var mExplosionSound - Pointer to explosion sound file
    //! @var mLaserSound - Pointer to Laser sound file
    //! @var mStereoSound - Pointer to Stereo sound component
    //! @var mSong  - Pointer to song sound file
    SoundSource* mExplosionSound{};
    SoundSource* mLaserSound{};
    SoundSource* mStereoSound{};
    SoundSource* mSong{};

    //! @var isPaused - for pausing of game
    //! @var isPhysicsEnabled - Turn on or off physics
    //! @var RecordBSplines - True == starts recording B-Spline positions
    bool isPaused = false;
    bool isPhysicsEnabled = false;
    bool RecordBSplines = false;

    //!PHYSICS
    //! @var Physics - pointer to PhysicsSystem
    //! @fn MakeGLSvec3D - Converts vector from QVector3D to gsl::Vector3D
    //! @fn MakeQvec3D - Converts vector from gsl::Vector3D to QVector3D
    PhysicsSystem* Physics = new PhysicsSystem();
    gsl::Vector3D MakeGSLvec3D(QVector3D vec);
    QVector3D MakeQvec3D(gsl::Vector3D vec);

    //! @fn togglePlayerCamera() toggels between player camera and editor camera
    void togglePlayerCamera();

private slots:
    //! @fn render - Main render loop. This is where the magic happens
    void render();

private:
    //! framecount
    int frameCountForPointIntake = 0;

    //! @fn KillZ - Reference to UnrealEngine 4 KillZ. Respawns objects that fall out of bounds.
    //! @param Transform - Transformcomponent of object we want to check
    //! @param SpawnPoint - Where we want to spawn the object if it is out of bounds
    void killZ(TransformComponent* Transform);
    //! @fn CalcDeltaTime Calculates accuratley the delta time of renderwindow
    //! @var DeltaTime Delta time is the results of CalcDeltaTime. Using Clock to calculate Delta time.
    //! @var oldTime - Reference to time of clock last render
    void CalcDeltaTime();
    float DeltaTime =0.0;
    std::chrono::steady_clock::time_point oldTime;

    //! @fn init Used to intitialise boiler plate code for OpenGL and objects we want to use in our scene.
    void init();
    //! @fn checkForGLerrors error handler
    void checkForGLerrors();
    //! @fn calculateFramerate Old code to calculate framerate
    void calculateFramerate();
    //! @fn startOpenGLDebugger debugger
    void startOpenGLDebugger();

    //!frustum culling - taken from ole.experiment - Modified by deniz
    //! @fn frustumCulling - Main function for frustum culling
    //! @param Index - Takes in entity index for objects
    //! @return bool isDrawable. returns false if object is outside of frustum
    bool frustumCulling(unsigned long long Index);
    //!@fn setCameraSpeed - Sets camera speed
    //! @param value - new value for speed
    void setCameraSpeed(float value);
    //! @fn handleInput - Update function for inputs and movement
    void handleInput();
    //! @fn switchPrograms - switcher for shader programs
    //! @param ShaderIndex - Index for shader you want to use
    void switchProgram(unsigned long long shaderIndex);
    //! @fn switchLOD - Checks distance from camera and switches to appropreate LOD
    //! @param Index - Entity Index for object we want to swith LOD on
    void switchLOD(unsigned long long Index);
    //! @fn drawFrostum - creates a test frustum for visualisation
    void drawFrostum();

    //! @var viewMatrix index
    //! @var projectionMatrix index
    //! @var modelMatrix index
    int viewMatrix{-1};
    int projectionMatrix{-1};
    int modelMatrix{-1};

    //! @fn setupPlainShader Switches to plainshader program
    //! @var mMatrixUniform index for model matrix uniform
    //! @var vMatrixUniform index for view matrix uniform
    //! @var pMatrixUniform index for projection matrix uniform
    void setupPlainShader(unsigned long long shaderIndex);
    GLint mMatrixUniform{-1};
    GLint vMatrixUniform{-1};
    GLint pMatrixUniform{-1};
    //! @fn setupTextureShader Switches to TextureShader program
    //! @var mMatrixUniform1 index for model matrix uniform
    //! @var vMatrixUniform1 index for view matrix uniform
    //! @var pMatrixUniform1 index for projection matrix uniform
    //! @var mTextureUniform index for Texture uniform
    void setupTextureShader(unsigned long long shaderIndex);
    GLint mMatrixUniform1{-1};
    GLint vMatrixUniform1{-1};
    GLint pMatrixUniform1{-1};
    GLint mTextureUniform{-1};
    //!
    //! \fn setupPhongShader Switches to TextureShader program
    //! @var mMmatrixUniform2 index for model matrix uniform
    //! @var mVmatrixUniform2 index for view matrix uniform
    //! @var mPmatrixUniform2 index for projection matrix uniform
    //! @var mTextureUniform2 index for Texture uniform
    //!
    //! @var mLightColorUniform index for Light Color uniform
    //! @var mObjectColorUniform index for Object Color uniform
    //! @var mAmbientLightStrengthUniform index for Ambient Light strenght uniform
    //! @var mLightPositionUniform index for Light Position uniform
    //! @var mCameraPositionUniform index for Camera position uniform
    //! @var mSpecularStrengthUniform index for Specular Strength uniform
    //! @var mSpecularExponentUniform index for Specular Exponent uniform
    //! @var mLightPowerUniform index for Light Power uniform
    //! @var mUsingTextureUniform index for Using exture uniform
    //!
    //Phong Shader Variables
    void setupPhongShader(unsigned long long index);

    GLint mMmatrixUniform2{-1};
    GLint mVmatrixUniform2{-1};
    GLint mPmatrixUniform2{-1};
    GLint mTextureUniform2{-1};

    //Light Shader Variables
    GLint mLightColorUniform{-1};
    GLint mObjectColorUniform{-1};
    GLint mAmbientLightStrengthUniform{-1};
    GLint mLightPositionUniform{-1};
    GLint mCameraPositionUniform{-1};
    GLint mSpecularStrengthUniform{-1};
    GLint mSpecularExponentUniform{-1};
    GLint mLightPowerUniform{-1};
    GLint mUsingTextureUniform{-1};


    //!
    //! @fn setupSkyboxshader Switches to TextureShader program
    //! @var mMmatrixUniform3 index for model matrix uniform
    //! @var mVmatrixUniform3 index for view matrix uniform
    //! @var mPmatrixUniform3 index for projection matrix uniform
    //! @var POSUniform3 index for position uniform
    //! @var skyboxUniform3 index for Texture uniform
    //!
    //!
    ///skybox
    void setupSkyboxshader(unsigned long long shaderIndex);
    GLint mMatrixUniform3{-1};
    GLint vMatrixUniform3{-1};
    GLint pMatrixUniform3{-1};
    GLint POSUniform3{-1};
    GLint skyboxUniform3{-1};
    //! @param textures_faces contains pointers to the skybox textures
    class Texture *textures_faces[gsl::NumberOfTextures]{nullptr};//We can hold some textures ;


    //! @param mTextures - contains pointers to all of our textures
    class Texture *mTextures[gsl::NumberOfTextures]{nullptr}; //We can hold some textures
    //! @param mShaderPrograms - pointers for all shader systems
    class Shader *mShaderPrograms[gsl::NumberOfShaders]{nullptr};    //holds pointer the GLSL shader programs
    //! @param mCurrentCamera - pointer to the current camera that outputs to screen
    Camera *mCurrentCamera{nullptr};
    //! @var mAspectratio - holds aspect ratio of screen
    float mAspectratio{1.f};

    //playerCamera switcher
    //! camera pointers
    //! @param mPlayerCamera contains pointer to player cam and its settings
    //! @param mEditorCamera contains pointer to Editor cam and its settings
    //! @param bIsPlayerCamera tells if you are in game cam or in editor cam
    //! @param CurrentPlayer holds transformcomponent for player object
    Camera *mPlayerCamera{nullptr};
    Camera *mEditorCamera{nullptr};
    bool bIsPlayerCamera = false;
    TransformComponent* CurrentPlayer = new TransformComponent();
    float posX = 0.0f;
    float PosZ = 0.0f;
    //!@param mInput is our input handler
    Input mInput;
    float mCameraSpeed{0.15f};
    float mCameraRotateSpeed{0.25f};
    int mMouseXlast{0};
    int mMouseYlast{0};

    QOpenGLContext *mContext{nullptr};
    bool mInitialized;

    QTimer *mRenderTimer{nullptr};           //timer that drives the gameloop
    QElapsedTimer mTimeStart;       //time variable that reads the calculated FPS

    MainWindow *mMainWindow{nullptr};        //points back to MainWindow to be able to put info in StatusBar

    class QOpenGLDebugLogger *mOpenGLDebugLogger{nullptr};

    /** raycasting
    * @fn RayCasting(QMouseEvent *event) creates a ray when in editor mode, this shows travel of mouse click
    *
    * @fn RayCastSphereCollision(QVector3D RayVec) checks for collision after casting a ray
    *
    **/
    void RayCasting(QMouseEvent *event);
    void RayCastSphereCollision(QVector3D RayVec);

    //!Gameplay
    //!
    std::vector<unsigned long long> bullets;
    std::vector<unsigned long long> Enemies;
    void LoadBullet();
    bool bBulletLoaded = false;
    void ShootBullet();
    void moveEnemies();
    void BallReset(TransformComponent *Transform);
    void initialEnemyPos();
    float xDir = -1.0f;
    float EnemyTravelLenght = 0.0f;
    int enemyCount = 0;

protected:
    //The QWindow that we inherit from has these functions to capture mouse and keyboard.
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
};

#endif // RENDERWINDOW_H
