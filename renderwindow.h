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
#include "skybox.h"
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

// This inherits from QWindow to get access to the Qt functionality and
// OpenGL surface.
// We also inherit from QOpenGLFunctions, to get access to the OpenGL functions
// This is the same as using glad and glw from general OpenGL tutorials
class RenderWindow : public QWindow, protected QOpenGLFunctions_4_1_Core
{
    Q_OBJECT
public:
    RenderWindow(const QSurfaceFormat &format, MainWindow *mainWindow);
    ~RenderWindow() override;

    QOpenGLContext *context() { return mContext; }

    void exposeEvent(QExposeEvent *) override;

    void toggleWireframe(bool buttonState);

    RenderSystem *RenderSys = new RenderSystem();
    resourceSystem * ResSys = new resourceSystem();
    EntitySystem * entitySys = new EntitySystem(this);
    collisionSystem* collisionSys = new collisionSystem();
    JSONSystem * JSS = new JSONSystem();

    std::vector<int> entities;

    std::vector<MeshComponent*> meshCompVec;
    std::vector<TransformComponent*> transformCompVec;
    std::vector<MaterialComponent*> MaterialCompVec;
    std::vector<DetailsComponent*> DeetsVector;

    //Some sounds...
    SoundSource* mExplosionSound{};
    SoundSource* mLaserSound{};
    SoundSource* mStereoSound{};
    SoundSource* mSong{};


    bool isPaused = false;
    bool isPhysicsEnabled = false;

    //PHYSICS
    PhysicsSystem* Physics = new PhysicsSystem();
    gsl::Vector3D MakeGSLvec3D(QVector3D vec);
    QVector3D MakeQvec3D(gsl::Vector3D vec);

    //player camera
    void togglePlayerCamera();

private slots:
    void render();

private:

    void killZ(TransformComponent* Transform, gsl::Vector3D  SpawnPoint);
    int physicsCount = 0;
    void CalcDeltaTime();
    float DeltaTime =0.0;
    std::chrono::steady_clock::time_point oldTime;

    void init();

    void checkForGLerrors();

    void calculateFramerate();

    void startOpenGLDebugger();

    //frustum culling - taken from ole.experiment
    bool frustumCulling(int Index);

    void setCameraSpeed(float value);

    void handleInput();

    void switchProgram(int shaderIndex);
    void switchLOD(int shaderIndex);
    void drawFrostum();

    int viewMatrix{-1};
    int projectionMatrix{-1};
    int modelMatrix{-1};

    void setupPlainShader(int shaderIndex);
    GLint mMatrixUniform{-1};
    GLint vMatrixUniform{-1};
    GLint pMatrixUniform{-1};

    void setupTextureShader(int shaderIndex);
    GLint mMatrixUniform1{-1};
    GLint vMatrixUniform1{-1};
    GLint pMatrixUniform1{-1};
    GLint mTextureUniform{-1};

    //Phong Shader Variables
    void setupPhongShader(int index);
    GLint mTextureUniform2{-1};

    GLint mMmatrixUniform2{-1};
    GLint mVmatrixUniform2{-1};
    GLint mPmatrixUniform2{-1};

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

    //skybox
    void setupSkyboxshader(int shaderIndex);
    GLint mMatrixUniform3{-1};
    GLint vMatrixUniform3{-1};
    GLint pMatrixUniform3{-1};
    GLint POSUniform3{-1};
    GLint skyboxUniform3{-1};
      class Texture *textures_faces[gsl::NumberOfTextures]{nullptr};//We can hold some textures ;
    //skybox* SkyBox;


    class Texture *mTextures[gsl::NumberOfTextures]{nullptr}; //We can hold some textures

    class Shader *mShaderPrograms[gsl::NumberOfShaders]{nullptr};    //holds pointer the GLSL shader programs

    Camera *mCurrentCamera{nullptr};
    float mAspectratio{1.f};

    //playerCamera switcher
    Camera *mPlayerCamera{nullptr};
    Camera *mEditorCamera{nullptr};
    bool bIsPlayerCamera = false;
    TransformComponent* CurrentPlayer = new TransformComponent();
    float posX = 0.0f;
    float PosZ = 0.0f;

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

    //raycasting
    void RayCasting(QMouseEvent *event);
    void RayCastSphereCollision(QVector3D RayVec);

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
