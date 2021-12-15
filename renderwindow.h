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
//! @class RenderWindow - Our giant render class that has mix of OOP and ECS.
class RenderWindow : public QWindow, protected QOpenGLFunctions_4_1_Core
{
    Q_OBJECT
public:
    //! init function Open GL spesific.
    RenderWindow(const QSurfaceFormat &format, MainWindow *mainWindow);
    ~RenderWindow() override;

    QOpenGLContext *context() { return mContext; }

    void exposeEvent(QExposeEvent *) override;

    /**
     * @brief toggleWireframe - Toggles wireframe on or off.
     * @param buttonState - button state gotten from UI.
     */
    void toggleWireframe(bool buttonState);

    /**
     * @brief RenderSys - Pointer to current RenderSystem.
     */
    RenderSystem *RenderSys = new RenderSystem();

    /**
     * @brief ResSys - Pointer to current Resource System.
     */
    resourceSystem * ResSys = new resourceSystem();
    /**
     * @brief entitySys - Pointer to current Entitysystem, takes in a pointer to RenderWindow.
     */
    EntitySystem * entitySys = new EntitySystem(this);
    /**
     * @brief collisionSys - Pointer to Collision System.
     */
    collisionSystem* collisionSys = new collisionSystem();
    /**
     * @brief JSS - Pointer to JSON System.
     */
    JSONSystem * JSS = new JSONSystem();
    /**
     * @brief entities - entities vector array holding all the entitie Indexes.
     */
    std::vector<int> entities;
    /**
     * @brief meshCompVec - Pointer array of type MeshComponent*. Holds pointers to every meshComponent, sorted by Entity Index.
     */
    std::vector<MeshComponent*> meshCompVec;
    /**
     * @brief transformCompVec - Pointer array of type TransformComponent*. Holds pointers to every TransformComponent, sorted by Entity Index.
     */
    std::vector<TransformComponent*> transformCompVec;
    /**
     * @brief MaterialCompVec - Pointer array of type MaterialComponent*. Holds pointers to every MaterialComponent, sorted by Entity Index..
     */
    std::vector<MaterialComponent*> MaterialCompVec;
    /**
     * @brief DeetsVector - Pointer array of type DetailsComponent*. Holds pointers to every DetailsComponent, sorted by Entity Index.
     */
    std::vector<DetailsComponent*> DeetsVector;

    /**
     * @brief mExplosionSound - Pointer to explosion sound file.
     */
    SoundSource* mExplosionSound{};
    /**
     * @brief mLaserSound - Pointer to Laser sound file.
     */
    SoundSource* mLaserSound{};
    /**
     * @brief mStereoSound - Pointer to Stereo sound component.
     */
    SoundSource* mStereoSound{};
    /**
     * @brief mSong  - Pointer to song sound file.
     */
    SoundSource* mSong{};

    /**
     * @brief isPaused - for pausing of game.
     */
    bool isPaused = false;
    /**
     * @brief isPhysicsEnabled - Turn on or off physics.
     */
    bool isPhysicsEnabled = false;
    /**
     * @brief RecordBSplines - True == starts recording B-Spline positions.
     */
    bool RecordBSplines = false;

    /**
     * @brief Physics - pointer to PhysicsSystem.
     */
    PhysicsSystem* Physics = new PhysicsSystem();
    /**
     * @brief MakeGSLvec3D - Converts vector from QVector3D to gsl::Vector3D.
     * @param vec
     * @return
     */
    gsl::Vector3D MakeGSLvec3D(QVector3D vec);
    /**
     * @brief MakeQvec3D - Converts vector from gsl::Vector3D to QVector3D.
     * @param vec
     * @return
     */
    QVector3D MakeQvec3D(gsl::Vector3D vec);

    /**
     * @brief togglePlayerCamera - toggels between player camera and editor camera.
     */
    void togglePlayerCamera();

private slots:
    /**
     * @brief render - Main render loop. This is where the magic happens, every loop of this function is considered a "frame".
     */
    void render();

private:
    //! framecount
    int frameCountForPointIntake = 0;


    /**
     * @brief killZ Reference to UnrealEngine 4 KillZ. Respawns objects that fall out of bounds.
     * @param Transform Transformcomponent of object we want to check.
     */
    void killZ(TransformComponent* Transform);

    /**
     * @brief CalcDeltaTime Calculates accuratley the delta time of renderwindow.
     * @var DeltaTime Delta time is the results of CalcDeltaTime. Using Clock to calculate Delta time.
     * @var oldTime - Reference to time of clock last rende.
     */
    void CalcDeltaTime();
    float DeltaTime =0.0;
    std::chrono::steady_clock::time_point oldTime;

    /**
     * @brief init Used to intitialise boiler plate code for OpenGL and objects we want to use in our scene.
     */
    void init();

    void checkForGLerrors();
    void calculateFramerate();
    void startOpenGLDebugger();


    /** frustum culling - taken from ole.experiment - Modified by deniz.
     * @brief frustumCulling -  Main function for frustum culling.
     * @param Index - Takes in entity index for objects.
     * @return bool isDrawable. returns false if object is outside of frustum.
     */
    bool frustumCulling(unsigned long long Index);

    /**
     * @brief setCameraSpeed Sets camera speed.
     * @param value new value for speed.
     */
    void setCameraSpeed(float value);

    /**
     * @brief handleInput Update function for inputs and movement.
     */
    void handleInput();

    /**
     * @brief switchProgram - switcher for shader programs.
     * @param shaderIndex - Index for shader you want to use.
     */
    void switchProgram(unsigned long long shaderIndex);

    /**
     * @brief switchLOD - Checks distance from camera and switches to appropreate LOD.
     * @param Index - Entity Index for object we want to swith LOD on.
     */
    void switchLOD(unsigned long long Index);

    /**
     * @brief drawFrostum - creates a test frustum for visualisation.
     */
    void drawFrostum();



    int viewMatrix{-1};
    int projectionMatrix{-1};
    int modelMatrix{-1};

    //! @fn setupPlainShader Switches to plainshader program

    /**
     * @brief setupPlainShader Switches to plainshader program.
     * @param shaderIndex  entity id
     */
    void setupPlainShader(unsigned long long shaderIndex);
    GLint mMatrixUniform{-1};
    GLint vMatrixUniform{-1};
    GLint pMatrixUniform{-1};
    //! @fn setupTextureShader Switches to TextureShader program

    /**
     * @brief setupTextureShader switches to TextureShader program
     * @param shaderIndex entity id
     */
    void setupTextureShader(unsigned long long shaderIndex);
    GLint mMatrixUniform1{-1};
    GLint vMatrixUniform1{-1};
    GLint pMatrixUniform1{-1};
    GLint mTextureUniform{-1};



    //Phong Shader Variables
    /**
     * @brief setupPhongShader Switches to TextureShader program.
     * @param index EntityID
     */
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


     * @brief setupSkyboxshader Switches to TextureShader program.
     * @param shaderIndex EntityID.
     */
    void setupSkyboxshader(unsigned long long shaderIndex);
    GLint mMatrixUniform3{-1};
    GLint vMatrixUniform3{-1};
    GLint pMatrixUniform3{-1};
    GLint POSUniform3{-1};
    GLint skyboxUniform3{-1};

    class Texture *textures_faces[gsl::NumberOfTextures]{nullptr};//! @param textures_faces contains pointers to the skybox textures.

    class Texture *mTextures[gsl::NumberOfTextures]{nullptr};//! @param mTextures - contains pointers to all of our textures.

    class Shader *mShaderPrograms[gsl::NumberOfShaders]{nullptr};    //! @param mShaderPrograms - pointers for all shader systems.

    Camera *mCurrentCamera{nullptr};//! @param mCurrentCamera - pointer to the current camera that outputs to screen.

    float mAspectratio{1.f}; //! @var mAspectratio - holds aspect ratio of screen.

    Camera *mPlayerCamera{nullptr};                                   //! @param mPlayerCamera contains pointer to player cam and its settings.
    Camera *mEditorCamera{nullptr};                                   //! @param mEditorCamera contains pointer to Editor cam and its settings.
    bool bIsPlayerCamera = false;                                     //! @param bIsPlayerCamera tells if you are in game cam or in editor cam.
    TransformComponent* CurrentPlayer = new TransformComponent();     //! @param CurrentPlayer holds transformcomponent for player object.
    float posX = 0.0f;
    float PosZ = 0.0f;

    Input mInput; //!@param mInput is our input handler
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


    /**
     * @brief RayCasting creates a ray when in editor mode, this shows travel of mouse click.
     * @param event event pointer.
     */
    void RayCasting(QMouseEvent *event);
    /**
     * @brief RayCastSphereCollision  checks for collision after casting a ray.
     * @param RayVec Direction Vector from screen to worldspace.
     */
    void RayCastSphereCollision(QVector3D RayVec);

    std::vector<unsigned long long> bullets;
    std::vector<unsigned long long> Enemies;
    /**
     * @brief LoadBullet  Loads bulelt infront of muzzle of turret.
     */
    void LoadBullet();
    bool bBulletLoaded = false;
    /**
     * @brief ShootBullet Shoots bullet and sets corresponding values.
     */
    void ShootBullet();
    /**
     * @brief moveEnemies Moves enemies in a zigzag pattern - like in space invaders.
     */
    void moveEnemies();
    /**
     * @brief BallReset Moves balls back into magazine.
     * @param Transform Transformcomponent of the ball we want to reset.
     */
    void BallReset(TransformComponent *Transform);
    /**
     * @brief initialEnemyPos - Sets and moves enemies into their initial positions. To restart the game.
     */
    void initialEnemyPos();
    float xDir = -1.0f;
    float EnemyTravelLenght = 0.0f;
    int enemyCount = 0;

protected:
    //The QWindow that we inherit from has these functions to capture mouse and keyboard.

    /**
     * @brief mousePressEvent
     * @param event
     */
    void mousePressEvent(QMouseEvent *event) override;
    /**
     * @brief mouseReleaseEvent
     * @param event
     */
    void mouseReleaseEvent(QMouseEvent *event) override;
    /**
     * @brief mouseMoveEvent
     * @param event
     */
    void mouseMoveEvent(QMouseEvent *event) override;
    /**
     * @brief keyPressEvent
     * @param event
     */
    void keyPressEvent(QKeyEvent *event) override;
    /**
     * @brief keyReleaseEvent
     * @param event
     */
    void keyReleaseEvent(QKeyEvent *event) override;
    /**
     * @brief wheelEvent
     * @param event
     */
    void wheelEvent(QWheelEvent *event) override;
};

#endif // RENDERWINDOW_H
